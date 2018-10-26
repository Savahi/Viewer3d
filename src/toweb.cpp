#include "stdafx.h"

using namespace Spider3d;

static std::string ganttJSON = "gantt.php";
static std::string usersPHP = "users.php";
static const char *cpOutputPathKey = "GanttFilesDir";
static const char *cpInputPathKey = "TextFilesDir";
static const char *cpLanguageKey = "Language";
static int loadIni( const char *configFile, std::map<std::string, std::string>& configParameters );

static std::string fileProject("proj.txt");
static std::string fileOperations("oper.txt");
static std::string fileLinks("link.txt");
static std::string fileUsers("user.txt");

static std::string phpExitScript = "<?php exit(); ?>";
static std::string phpAuthScript = "<?php require('auth.php'); if( isAuthRequired() ) { auth(true); } ?>";

static void outputProject( std::ofstream& fsOutput, Project& project );
static void outputGantt ( std::ofstream& fsOutput, Gantt& gantt );
static void outputLinks( std::ofstream& fsOutput, Links& links );

static void getFieldTypeAndWidth( int iFlags, std::string fieldName, std::string& type, int& fieldWidth );

int main( int argc, char* argv[] ) {
    int iStatus;
    std::string empty("");

    if( argc < 2 ) {
        std::cout << "A required argument is missing!\nUse: " << argv[0] << " <full-path-to-ini-file>.\nExiting...\n";
        return 0;
    }

    Project project;
    Gantt gantt;
    Links links;

    std::map<std::string, std::string> configParameters;
    loadIni( argv[1], configParameters );
    if( configParameters.find(cpOutputPathKey)==configParameters.end() || 
        configParameters.find(cpInputPathKey)==configParameters.end() ) {
        std::cout << "The configuration file " << argv[1] << " is invalid.\nExiting...\n";
    }

    loadProject( project, (configParameters[cpInputPathKey] + fileProject).c_str() );

    loadGantt( gantt, (configParameters[cpInputPathKey] + fileOperations).c_str() );

    loadLinks( links, (configParameters[cpInputPathKey] + fileLinks).c_str() );

    // Writing output file
    std::ofstream fsOutput;
    std::string outputFile = configParameters[cpOutputPathKey] + "/" + ganttJSON;
    fsOutput.open( outputFile.c_str() );    
    if( fsOutput.fail() ) {
        std::cout << "Can't write into the " << outputFile << ".\nExiting..." << std::endl; 
    } else {
        fsOutput << phpAuthScript << std::endl; // To prevent direct access to the file via http.
        fsOutput << "{";

        outputProject( fsOutput, project ); 
        fsOutput << "," << std::endl;
        outputGantt( fsOutput, gantt );
        fsOutput << "," << std::endl;
        outputLinks( fsOutput, links ); 
        fsOutput << "," << std::endl << "\"lang\":\"" << toLower(configParameters[cpLanguageKey]) << "\"";
        fsOutput << std::endl << "}";
        fsOutput.close();
    }

    // Writing "users" file
    std::ofstream fsUsers;
    fsUsers.open( (configParameters[cpOutputPathKey] + "/" + usersPHP).c_str() );    
    if( fsUsers.fail() ) {
        std::cout << "Can't write users file.\nExiting..." << std::endl; 
    } else {
        Table table;
        iStatus = loadTable( table, (configParameters[cpInputPathKey] + fileUsers).c_str() );
        if( iStatus == -1 ) {
            fsUsers << "NOAUTH";
        } else {
            fsUsers << phpExitScript << std::endl; // To prevent direct access to the file via http.
            int iLogin = table.fieldsPositions["Code"];
            int iName = table.fieldsPositions["Name"];
            int iPassword = table.fieldsPositions["Password"];
            for( int i = 0 ; i < table.size() ; i++ ) {
                fsUsers << table.lines[i].fields[iLogin] << "\t" << table.lines[i].fields[iPassword] << "\t";
                fsUsers << table.lines[i].fields[iName] << std::endl;
            }
        }
        fsUsers.close();        
    }

    return 0;
}


static void outputProject( std::ofstream& fsOutput, Project& project ) {
    fsOutput << std::endl << "\"proj\": { ";
    fsOutput << "\"Code\":\"" << project.sCode << "\", \"Name\":\"" << project.sName << "\"";
    fsOutput << ",\"ProjVer\":\"" << project.sProjVer << "\",\"CurTime\":\"" << project.sCurTime << "\"";
    fsOutput << ",\"Notes\":\"" << project.sNotes << "\"";
    fsOutput << "}";
}


static void outputGantt ( std::ofstream& fsOutput, Gantt& gantt ) {
    fsOutput << "\"operations\": [";
    bool bFirst = true;
    for( int i = 0 ; i < gantt.operations.size() ; i++ ) {

        int iAsapStart = gantt.fieldsPositions["AsapStart"];
        int iFactStart = gantt.fieldsPositions["FactStart"];
        if( isEmpty( gantt.operations[i].fields[iAsapStart] ) && isEmpty( gantt.operations[i].fields[iFactStart] ) ) {
            continue;
        }
        int iAsapFin = gantt.fieldsPositions["AsapFin"];
        int iFactFin = gantt.fieldsPositions["FactFin"];
        if( isEmpty( gantt.operations[i].fields[iAsapFin] ) && isEmpty( gantt.operations[i].fields[iFactFin] ) ) {
            continue;
        }

        if( !bFirst ) {
            fsOutput << ",";
        }
        bFirst = false;
        fsOutput << std::endl << " {";

        for( int iField = 0 ; iField < gantt.operations[i].fields.size() ; iField++ ) {
            if( iField > 0 ) {
                fsOutput << ",";
            }
            fsOutput << "\"" << gantt.fieldsNames[iField] << "\":";

            if( gantt.fieldsNames[iField] == "Level" ) { // If "Level"...   
                if( isDigitsOnly( gantt.operations[i].fields[iField] ) ) {  // If phase ("digits only")...
                    fsOutput << gantt.operations[i].fields[iField]; // ...outputing without quotes.
                    continue;
                } 
            }

            if( !gantt.operations[i].fields[iField].empty() ) {
                fsOutput << "\"" << gantt.operations[i].fields[iField] << "\"";
            } else {
                fsOutput << "null";
            }            
        }
        fsOutput << "}" ;
    }
    fsOutput << std::endl << "],";

    fsOutput << std::endl << "\"table\": [" << std::endl << "{ \"name\":\"[]\", \"ref\":\"expandColumn\", \"width\":2, \"visible\":true, \"type\":null }";

    for( int i = 0 ; i < gantt.fieldsNames.size() ; i++ ) {
        long int iFlags = gantt.fieldsFlags[ gantt.fieldsNames[i] ];
        std::string visible;
        if( iFlags & FIELD_HIDDEN ) {
            continue;    
        }
        std::string type;
        int iWidth;
        getFieldTypeAndWidth( iFlags, gantt.fieldsNames[i], type, iWidth ); 
        fsOutput << "," << std::endl <<  "{\"name\":\"" << gantt.fieldsTitles[ gantt.fieldsNames[i] ];
        fsOutput << "\",\"ref\":\"" << gantt.fieldsNames[i] << "\",\"visible\":true,\"width\":" << iWidth << ",\"type\":\"" << type << "\"}";
    }
    fsOutput << std::endl << "],";
    
    fsOutput << std::endl << "\"editables\": [ ";

    for( int i = 0, counter = 0 ; i < gantt.fieldsNames.size() ; i++ ) {
        long int iFlags = gantt.fieldsFlags[ gantt.fieldsNames[i] ];
        if( !( iFlags & FIELD_EDITABLE ) ) {
            continue;
        } 
        if( counter > 0 ) {
            fsOutput << ",";
        }
        std::string type;
        int iWidth;
        getFieldTypeAndWidth( iFlags, gantt.fieldsNames[i], type, iWidth ); 
        fsOutput << std::endl;
        fsOutput << " { \"ref\":\"" << gantt.fieldsNames[i] << "\",\"name\":\"" << gantt.fieldsTitles[ gantt.fieldsNames[i] ] << "\"";
        fsOutput << ",\"type\":\"" << type << "\"}";
        counter++;
    }
    fsOutput << std::endl << "]";
}


static void outputLinks( std::ofstream& fsOutput, Links& links ) {
    fsOutput << "\"links\": [";
    for( int i = 0 ; i < links.size() ; i++ ) {
        if( i > 0 ) {
            fsOutput << ",";
        }
        fsOutput << std::endl << " {";
        fsOutput << "\"PredCode\":\"" << links.links[i].sPredCode << "\",";
        fsOutput << "\"SuccCode\":\"" << links.links[i].sSuccCode << "\",";

        std::string type;
        if( links.links[i].bTypeSF2 ) {
            if( links.links[i].iTypeSF2 == LINK_SS ) {
                type = "SS";
            } else if( links.links[i].iTypeSF2 == LINK_FF ) {
                type = "FF";
            } else if( links.links[i].iTypeSF2 == LINK_SF ) {
                type = "SF";
            } else {
                type = "FS";                
            }
        } else {
            type = "FS";            
        }
        fsOutput << "\"TypeSF2\":\"" << type << "\"";
        fsOutput << "}" ;
    }
    fsOutput << std::endl << "]";
}

static int loadIni( const char *configFile, std::map<std::string, std::string>& configParameters ) {
    std::ifstream infile( configFile );
    std::string line;

    if( infile.is_open() ) {
        while( std::getline( infile, line ) )   {
            std::istringstream iss_line( line );
            std::string key;
            if( std::getline( iss_line, key, '=' ) ) {
                std::string value;
                if( std::getline(iss_line, value ) ) {
                    configParameters[key] = value;
                }
            }
        }
        infile.close();
        return 0;
    }
    return -1;
}


static void getFieldTypeAndWidth( int iFlags, std::string fieldName, std::string& type, int& iWidth ) {
    int masked = iFlags & FIELD_TYPE_MASK; 
    if( masked == FIELD_STRING ) {
        if( fieldName == "Notes" ) {
            type = "text";
        } else {
            type = "string";
        }
    } else if( masked == FIELD_FLOAT ) {
        type = "float";
    } else if( masked == FIELD_INT ) {
        type = "int";
    } else if( masked == FIELD_TIME ) {
        type = "datetime";
    }
    iWidth = iFlags & FIELD_WIDTH_MASK;
}
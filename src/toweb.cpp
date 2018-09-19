#include "stdafx.h"

using namespace Spider3d;

static std::string ganttJSON = "gantt.json";
static const char *cpOutputPathKey = "GanttFilesDir";
static const char *cpInputPathKey = "TextFilesDir";
static const char *cpLanguageKey = "Language";
static int loadIni( const char *configFile, std::map<std::string, std::string>& configParameters );

static std::string fileProject("proj.txt");
static std::string fileOperations("oper.txt");
static std::string fileLinks("link.txt");

static void outputProject( std::ofstream& fsOutput, Project& project );
static void outputGantt ( std::ofstream& fsOutput, Gantt& gantt );
static void outputLinks( std::ofstream& fsOutput, Links& links );

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
        fsOutput << "{";

        outputProject( fsOutput, project ); 
        fsOutput << ",\r\n";
        outputGantt( fsOutput, gantt );
        fsOutput << ",\r\n";
        outputLinks( fsOutput, links ); 
        fsOutput << ",\r\n\"lang\":\"" << toLower(configParameters[cpLanguageKey]) << "\"";
        fsOutput << "\r\n}";
    }
    fsOutput.close();
    
    return 0;
}


static void outputProject( std::ofstream& fsOutput, Project& project ) {
    fsOutput << "\r\n\"proj\": { ";
    fsOutput << "\"Code\":\"" << project.sCode << "\", \"Name\":\"" << project.sName << "\"";
    fsOutput << ",\"ProjVer\":" << project.sProjVer << ",\"CurTime\":\"" << project.sCurTime << "\"";
    fsOutput << ",\"Notes\":\"" << project.sNotes << "\"";
    fsOutput << "}";
}


static void outputGantt ( std::ofstream& fsOutput, Gantt& gantt ) {
    fsOutput << "\"operations\": [";
    for( int i = 0 ; i < gantt.operations.size() ; i++ ) {
        if( i > 0 ) {
            fsOutput << ",";
        }
        fsOutput << "\r\n{";

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
    fsOutput << "\r\n],";

    fsOutput << "\r\n\"table\": [ { \"name\":\"[]\", \"ref\":\"expandColumn\", \"width\":20, \"visible\":true }";

    for( int i = 0 ; i < gantt.fieldsNames.size() ; i++ ) {
        long int iFlags = gantt.fieldsFlags[ gantt.fieldsNames[i] ];
        std::string visible;
        if( iFlags & FIELD_HIDDEN ) {
            visible = "false";
        } else {
            visible = "true";
        }
        fsOutput << ",\r\n {\"name\":\"" << gantt.fieldsTitles[ gantt.fieldsNames[i] ];
        fsOutput << "\",\"ref\":\"" << gantt.fieldsNames[i] << "\",\"visible\":" << visible << ",\"width\":40}";
    }
    fsOutput << "\r\n],";
    
    fsOutput << "\r\n\"editables\": [ ";

    for( int i = 0, counter = 0 ; i < gantt.fieldsNames.size() ; i++ ) {
        long int iFlags = gantt.fieldsFlags[ gantt.fieldsNames[i] ];
        if( !( iFlags & FIELD_EDITABLE ) ) {
            continue;
        } 
        if( counter > 0 ) {
            fsOutput << ",";
        }
        std::string type;
        if( iFlags & FIELD_STRING ) {
            if( gantt.fieldsNames[i] == "Notes" ) {
                type = "text";
            } else {
                type = "string";
            }
        } else if( iFlags & FIELD_FLOAT ) {
            type = "float";
        } else if( iFlags & FIELD_INT ) {
            type = "int";
        }
        fsOutput << "\r\n{ \"ref\":\"" << gantt.fieldsNames[i] << "\",\"type\":\"" << type << "\"}";
        counter++;
    }
    fsOutput << "\r\n]";
}


static void outputLinks( std::ofstream& fsOutput, Links& links ) {
    fsOutput << "\"links\": [";
    for( int i = 0 ; i < links.size() ; i++ ) {
        if( i > 0 ) {
            fsOutput << ",";
        }
        fsOutput << "\r\n{";
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
    fsOutput << "\r\n]";
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
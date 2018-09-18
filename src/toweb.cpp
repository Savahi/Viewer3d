#include "stdafx.h"

using namespace Spider3d;

static std::string ganttJSON = "gantt.json";
static const char *cpOutputPathKey = "GanttFilesDir";
static const char *cpInputPathKey = "TextFilesDir";
static const char *cpLanguageKey = "Language";
static int loadIni( const char *configFile, std::map<std::string, std::string>& configParameters );

static std::string fileProject("proj.txt");
static std::string fileOperations("oper.txt");
static std::string fileOpLinks("link.txt");

static void outputProject( std::ofstream& fsOutput, Project& project );
static void outputGantt ( std::ofstream& fsOutput, Gantt& gantt );
static void outputOpLinks( std::ofstream& fsOutput, OpLinks& opLinks );

int main( int argc, char* argv[] ) {
    int iStatus;
    std::string empty("");

    if( argc < 2 ) {
        std::cout << "A required argument is missing!\nUse: " << argv[0] << " <full-path-to-ini-file>.\nExiting...\n";
        return 0;
    }

    Project project;
    Gantt gantt;
    OpLinks opLinks;

    std::map<std::string, std::string> configParameters;
    loadIni( argv[1], configParameters );
    if( configParameters.find(cpOutputPathKey)==configParameters.end() || 
        configParameters.find(cpInputPathKey)==configParameters.end() ) {
        std::cout << "The configuration file " << argv[1] << " is invalid.\nExiting...\n";
    }

    loadProject( project, (configParameters[cpInputPathKey] + fileProject).c_str() );

    loadGantt( gantt, (configParameters[cpInputPathKey] + fileOperations).c_str() );

    loadOpLinks( opLinks, (configParameters[cpInputPathKey] + fileOpLinks).c_str() );

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
        outputOpLinks( fsOutput, opLinks ); 
        fsOutput << ",\r\n\"language\":\"" << configParameters[cpLanguageKey] << "\"";
        fsOutput << "\r\n}";
    }
    fsOutput.close();
    
    return 0;
}


static void outputProject( std::ofstream& fsOutput, Project& project ) {
        fsOutput << "\r\n\"proj\": { ";
        fsOutput << "\"code\":\"" << project.sCode << "\", \"name\":\"" << project.sName << "\"";
        fsOutput << ",\"projVer\":" << project.iProjVer << ",\"curTime\":\"" << project.sCurTime << "\"";
        fsOutput << "}";
}


static void outputGantt ( std::ofstream& fsOutput, Gantt& gantt ) {
    fsOutput << "\r\n\"operations\": [";
    for( int i = 0 ; i < gantt.operations.size() ; i++ ) {
        if( i > 0 ) {
            fsOutput << ",";
        }
        fsOutput << "\r\n{";

        for( int iField = 0 ; iField < gantt.operations[i].fields.size() ; iField++ ) {
            if( iField > 0 ) {
                fsOutput << ",";
            }
            fsOutput << "\"" << gantt.fieldsNames[i] << "\":";

            if( !gantt.operations[i].fields[iField].empty() ) {
                fsOutput << "\"" << gantt.operations[i].fields[iField] << "\"";
            } else {
                fsOutput << ":null";
            }            
        }
        fsOutput << "}" ;
    }
    fsOutput << "\r\n],";

    fsOutput << "\r\n\"table\": [ { \"name\":\"[]\", \"ref\":\"expandColumn\", \"width\":20 }";

    for( int i = 0 ; i < gantt.fieldsNames.size() ; i++ ) {
        fsOutput << ",\r\n { \"name\":\"" << gantt.fieldsTitles[ gantt.fieldsNames[i] ] << "\",\"ref\":\"" << gantt.fieldsNames[i] << "\",\"width\":40}";
    }
    fsOutput << "\r\n],";
    
    fsOutput << "\r\n\"editable\": [ ";

    for( int i = 0, counter = 0 ; i < gantt.fieldsNames.size() ; i++ ) {
        long int iFlags = gantt.fieldsFlags[ gantt.fieldsNames[i] ];
        if( !( iFlags & 1 ) ) {
            continue;
        } 
        if( counter > 0 ) {
            fsOutput << ",";
        }
        fsOutput << ",\r\n { \"ref\":\"" << gantt.fieldsNames[i] << "\",\"type\":\"text\"}";
        counter++;
    }
    fsOutput << "\r\n]";
}


static void outputOpLinks( std::ofstream& fsOutput, OpLinks& opLinks ) {

    fsOutput << "\r\n\"links\": [";
    for( int i = 0 ; i < opLinks.number() ; i++ ) {
        if( i > 0 ) {
            fsOutput << ",";
        }
        fsOutput << "\r\n{";
        fsOutput << "\"PredCode\":\"" << opLinks.mOpLinks[i].sPredCode << "\",";
        fsOutput << "\"SuccCode\":\"" << opLinks.mOpLinks[i].sSuccCode << "\",";
        fsOutput << "\"TypeSF\":\"" << opLinks.mOpLinks[i].sTypeSF << "\",";
        fsOutput << "\"LagType\":\"" << opLinks.mOpLinks[i].sLagType << "\",";
        fsOutput << "\"LagUnit\":\"" << opLinks.mOpLinks[i].sLagUnit << "\",";
        if( opLinks.mOpLinks[i].bLag ) {
            fsOutput << "\"Lag\":" << opLinks.mOpLinks[i].fLag << "";
        } else {
            fsOutput << "\"Lag\":null";
        }
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
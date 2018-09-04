#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "helpers.hpp"
#include "toweb.hpp"

using namespace Spider3d;

static const char *cpOutputPathKey = "GanttFilesDir";
static const char *cpInputPathKey = "TextFilesDir";
static int loadIni( const char *configFile, std::map<std::string, std::string>& configParameters );

static std::string fileOps("oper.txt");
static std::string fileOpLinks("link.txt");
static std::string fileAssignments("ass.txt");
static std::string fileResources("res.txt");
static std::string fileCosts("cost.txt");
static std::string fileOpCosts("oper_cost.txt");

int main( int argc, char* argv[] ) {
    int iStatus;

    if( argc < 2 ) {
        std::cout << "The required argument is missing!\nUse: " << argv[0] << " <full-path-to-ini-file>.\nExiting...\n";
        return 0;
    }

    Operations operations;
    OpLinks opLinks;
    OpResAssignments opResAssignments;
    Resources resources;

    std::map<std::string, std::string> configParameters;
    loadIni( argv[1], configParameters );
    if( configParameters.find(cpOutputPathKey)==configParameters.end() || 
        configParameters.find(cpInputPathKey)==configParameters.end() ) {
        std::cout << "The configuration file " << argv[1] << " is invalid.\nExiting...\n";
    }

    loadOperations( operations, (configParameters[cpInputPathKey] + fileOps).c_str() );

    loadOpLinks( opLinks, (configParameters[cpInputPathKey] + fileOpLinks).c_str() );

    loadResources( resources, (configParameters[cpInputPathKey] + fileResources).c_str() );

    loadOpResAssignments( opResAssignments, (configParameters[cpInputPathKey] + fileAssignments).c_str() );

    // Writing output file
    std::ofstream fsOutput;
    fsOutput.open( configParameters[cpOutputPathKey].c_str() );    
    if( fsOutput.fail() ) {
        std::cout << "Can't write into the " << configParameters[cpOutputPathKey] << ".\nExiting..." << std::endl; 
        return 0;
    }
    fsOutput.close();
    
    return 0;
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
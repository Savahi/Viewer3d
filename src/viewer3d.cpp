#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "helpers.hpp"
#include "models.hpp"
#include "operations.hpp"
#include "viewer3d.hpp"

using namespace Spider3d;

static int loadIni( const char *configFile, std::map<std::string, std::string>& configParameters ) {
    std::ifstream infile( configFile );
    std::string line;

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
    return 0;
}


int main( int argc, char* argv[] ) {
    int iStatus;

    if( argc < 2 ) {
        std::cout << "The required argument is missing!\nUse: " << argv[0] << " <source.csv>.\nExiting...\n";
        return 0;
    }

    std::map<std::string, std::string> configParameters;
    loadIni( argv[1], configParameters );
    if( configParameters.find("models")==configParameters.end() || 
        configParameters.find("operations")==configParameters.end() ||
        configParameters.find("types")==configParameters.end() ) {
        std::cout << "The configuration file " << argv[1] << " is invalid.\nExiting...\n";
    }

    Models models;
    Operations operations;

    loadOperations( operations, configParameters["operations"].c_str() );
    if( operations.number() == 0 ) {
        std::cout << "No operations found!\n.Exiting...\n";
        return 0;
    }

    loadModels( models, configParameters["models"].c_str() );
    if( models.number() == 0 ) {
        std::cout << "No models found!\n.Exiting...\n";
        return 0;
    }

    display( models, operations, argc, argv );
    
    return 0;
}

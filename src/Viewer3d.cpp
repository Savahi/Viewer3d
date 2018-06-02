#include <string>
#include <iostream>
include <fstream>
#include <sstream>
#include <map>
#include "Helpers.hpp"
#include "Model3d.hpp"
#include "Operations.hpp"

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
    if( configParameters.find("objects")==configParameters.end() || 
        configParameters.find("operations")==configParameters.end() ||
        configParameters.find("types")==configParameters.end() ) {
        std::cout << "The configuration file " << argv[1] << " is invalid.\nExiting...\n";
    }

    Model3d model;
    Operations operations;

    loadOperations( operations, configParameters["operations"].c_str() );
    if( operations.totalNumber() == 0 ) {
        cout << "No operations found!\n.Exiting...\n";
        return 0;
    }

    loadModel( model, configParameters["objects"].c_str() );
    if( model.objectsNumber() == 0 ) {
        cout << "No objects found!\n.Exiting...\n";
        return 0;
    }

    model.display( argc, argv );
    
    return 0;
}

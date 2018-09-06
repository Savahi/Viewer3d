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

static std::string fileProject("proj.txt");
static std::string fileOperations("oper.txt");
static std::string fileOpLinks("link.txt");

static void outputProject( std::ofstream& fsOutput, Project& project );
static void outputOperations ( std::ofstream& fsOutput, Operations& operations );
static void outputOpLinks( std::ofstream& fsOutput, OpLinks& opLinks );

int main( int argc, char* argv[] ) {
    int iStatus;
    std::string empty("");

    if( argc < 2 ) {
        std::cout << "A required argument is missing!\nUse: " << argv[0] << " <full-path-to-ini-file>.\nExiting...\n";
        return 0;
    }

    Project project;
    Operations operations;
    OpLinks opLinks;

    std::map<std::string, std::string> configParameters;
    loadIni( argv[1], configParameters );
    if( configParameters.find(cpOutputPathKey)==configParameters.end() || 
        configParameters.find(cpInputPathKey)==configParameters.end() ) {
        std::cout << "The configuration file " << argv[1] << " is invalid.\nExiting...\n";
    }

    loadProject( project, (configParameters[cpInputPathKey] + fileProject).c_str() );

    loadOperations( operations, (configParameters[cpInputPathKey] + fileOperations).c_str() );

    loadOpLinks( opLinks, (configParameters[cpInputPathKey] + fileOpLinks).c_str() );

    // Writing output file
    std::ofstream fsOutput;
    std::string outputFile = configParameters[cpOutputPathKey] + "/gantt.js";
    fsOutput.open( outputFile.c_str() );    
    if( fsOutput.fail() ) {
        std::cout << "Can't write into the " << outputFile << ".\nExiting..." << std::endl; 
    } else {
        fsOutput << "{";

        outputProject( fsOutput, project ); 
        outputOperations( fsOutput, operations );
        outputOpLinks( fsOutput, opLinks ); 
        
        fsOutput << "\r\n};";
    }
    fsOutput.close();
    
    return 0;
}


static void outputProject( std::ofstream& fsOutput, Project& project ) {
        fsOutput << "\r\nproj: { ";
        fsOutput << "code:'" << project.sCode << "', name:'" << project.sName << "'";
        fsOutput << ", projVer:" << project.iProjVer << ", curTime:'" << project.sCurTime << "'";
        fsOutput << "},";
}

static void outputOpLinks( std::ofstream& fsOutput, OpLinks& opLinks ) {

    fsOutput << "\r\nopLinks: [";
    for( int i = 0 ; i < opLinks.number() ; i++ ) {
        fsOutput << "\r\n{";
        fsOutput << "predCode:'" << opLinks.mOpLinks[i].sPredCode << "',";
        fsOutput << "succCode:'" << opLinks.mOpLinks[i].sSuccCode << "',";
        fsOutput << "typeSF:'" << opLinks.mOpLinks[i].sTypeSF << "',";
        fsOutput << "lagType:'" << opLinks.mOpLinks[i].sLagType << "',";
        fsOutput << "lagUnit:'" << opLinks.mOpLinks[i].sLagUnit << "',";
        if( opLinks.mOpLinks[i].bLag ) {
            fsOutput << " lag:" << opLinks.mOpLinks[i].fLag << ",";
        } else {
            fsOutput << " lag:null,";
        }
        fsOutput << "}," ;
    }
    fsOutput << "\r\n],";
}

static void outputOperations ( std::ofstream& fsOutput, Operations& operations ) {
    fsOutput << "\r\noperations: [";
    for( int i= 0 ; i < operations.number() ; i++ ) {
        fsOutput << "\r\n{";
        if( !operations.mOperations[i].sLevel.empty() ) {
            fsOutput << " level:" << operations.mOperations[i].iLevel << ",";
        } else {
            fsOutput << " level:null,";
        }
        fsOutput << "code:'" << operations.mOperations[i].sCode << "',";
        fsOutput << "name:'" << operations.mOperations[i].sName << "',";
        fsOutput << "actualStart:'" << operations.mOperations[i].sActualStart << "',";
        fsOutput << "actualFinish:'" << operations.mOperations[i].sActualFinish << "',";            
        fsOutput << "asapStart:'" << operations.mOperations[i].sAsapStart << "',";
        fsOutput << "asapFinish:'" << operations.mOperations[i].sAsapFinish << "',";            
        fsOutput << "compareStart:'" << operations.mOperations[i].sCompareStart << "',";
        fsOutput << "compareFinish:'" << operations.mOperations[i].sCompareFinish << "',";            
        if( operations.mOperations[i].bCritical ) {
            fsOutput << " critical:" << operations.mOperations[i].iCritical << ",";
        } else {
            fsOutput << " critical:null,";
        }
        if( operations.mOperations[i].bCostTotal ) {
            fsOutput << " costTotal:" << operations.mOperations[i].fCostTotal << ",";
        } else {
            fsOutput << " costTotal:null,";
        }
        if( operations.mOperations[i].bVolSum ) {
            fsOutput << " volSum:" << operations.mOperations[i].fVolSum << ",";
        } else {
            fsOutput << " volSum:null,";
        }
        if( operations.mOperations[i].bDurSumD ) {
            fsOutput << " durSumD:" << operations.mOperations[i].fDurSumD << ",";
        } else {
            fsOutput << " durSumD:null,";
        }
        fsOutput << "}," ;
    }
    fsOutput << "\r\n],";
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
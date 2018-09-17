#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "helpers.hpp"
#include "toweb.hpp"

using namespace Spider3d;

static std::string ganttJSON = "gantt.json";
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
    std::string outputFile = configParameters[cpOutputPathKey] + "/" + ganttJSON;
    fsOutput.open( outputFile.c_str() );    
    if( fsOutput.fail() ) {
        std::cout << "Can't write into the " << outputFile << ".\nExiting..." << std::endl; 
    } else {
        fsOutput << "{";

        outputProject( fsOutput, project ); 
        outputOperations( fsOutput, operations );
        outputOpLinks( fsOutput, opLinks ); 
        
        fsOutput << "\r\n}";
    }
    fsOutput.close();
    
    return 0;
}


static void outputProject( std::ofstream& fsOutput, Project& project ) {
        fsOutput << "\r\n\"proj\": { ";
        fsOutput << "\"code\":\"" << project.sCode << "\", \"name\":\"" << project.sName << "\"";
        fsOutput << ",\"projVer\":" << project.iProjVer << ",\"curTime\":\"" << project.sCurTime << "\"";
        fsOutput << "},";
}


static void outputOperations ( std::ofstream& fsOutput, Operations& operations ) {
    fsOutput << "\r\n\"operations\": [";
    for( int i = 0 ; i < operations.number() ; i++ ) {
        if( i > 0 ) {
            fsOutput << ",";
        }
        fsOutput << "\r\n{";
        if( !operations.mOperations[i].sLevel.empty() ) {
            fsOutput << "\"Level\":" << operations.mOperations[i].iLevel << ",";
        } else {
            fsOutput << "\"Level\":null,";
        }
        fsOutput << "\"Code\":\"" << operations.mOperations[i].sCode << "\",";
        fsOutput << "\"Name\":\"" << operations.mOperations[i].sName << "\",";
        fsOutput << "\"Start\":\"" << operations.mOperations[i].sStart << "\",";
        fsOutput << "\"Fin\":\"" << operations.mOperations[i].sFinish << "\",";            
        fsOutput << "\"FactStart\":\"" << operations.mOperations[i].sActualStart << "\",";
        fsOutput << "\"FactFin\":\"" << operations.mOperations[i].sActualFinish << "\",";            
        fsOutput << "\"AsapStart\":\"" << operations.mOperations[i].sAsapStart << "\",";
        fsOutput << "\"AsapFin\":\"" << operations.mOperations[i].sAsapFinish << "\",";            
        fsOutput << "\"Start_COMP\":\"" << operations.mOperations[i].sCompareStart << "\",";
        fsOutput << "\"Fin_COMP\":\"" << operations.mOperations[i].sCompareFinish << "\",";            
        if( operations.mOperations[i].bCritical ) {
            fsOutput << "\"f_Ccritical\":" << operations.mOperations[i].iCritical << ",";
        } else {
            fsOutput << "\"f_Critical\":null,";
        }
        if( operations.mOperations[i].bCostTotal ) {
            fsOutput << "\"CostTotal\":" << operations.mOperations[i].fCostTotal << ",";
        } else {
            fsOutput << "\"CostTotal\":null,";
        }
        if( operations.mOperations[i].bVolSum ) {
            fsOutput << "\"VolSum\":" << operations.mOperations[i].fVolSum << ",";
        } else {
            fsOutput << "\"VolSum\":null,";
        }
        if( operations.mOperations[i].bDurSumD ) {
            fsOutput << "\"DurSumD\":" << operations.mOperations[i].fDurSumD << ",";
        } else {
            fsOutput << "\"DurSumD\":null,";
        }
        fsOutput << "\"Notes\":\"" << operations.mOperations[i].sNotes << "\"";
        fsOutput << "}" ;
    }
    fsOutput << "\r\n],";
}


static void outputOpLinks( std::ofstream& fsOutput, OpLinks& opLinks ) {

    fsOutput << "\r\n\"opLinks\": [";
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
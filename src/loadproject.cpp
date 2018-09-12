#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "helpers.hpp"
#include "project.hpp"

namespace Spider3d {

    static int fieldsToReadNum = 4;
    static const char *fieldsToRead[] = { "Code", "Name", "ProjVer", "CurTime" };

    int loadProject( Project& project, const char *cpFile ) {
        int iReturn = -1;
        int nScanned;

        std::vector<std::string> fieldsNames;
        for( int i= 0 ; i < fieldsToReadNum ; i++ ) {
            fieldsNames.push_back( fieldsToRead[i] );
        }
        std::map<std::string,int> fieldsPositions;

        std::ifstream infile( cpFile );
        if( infile.is_open() ) {

            int numHeaderParsed = parseFileHeader( infile, fieldsNames, fieldsPositions );
            if( numHeaderParsed != -1 ) {
                std::map<std::string,std::string> fieldsParsed;    
                int numParsed = parseFileLine( infile, fieldsPositions, fieldsParsed );
                if( numParsed != -1 ) {
                    project.sCode = fieldsParsed["Code"];
                    project.sName = fieldsParsed["Name"];
                    project.sProjVer = fieldsParsed["projVer"];
                    int nScanned = sscanf( project.sProjVer.c_str(), "%d", &project.iProjVer );
                    if( nScanned == 1 ) {
                        project.bProjVer = true;
                    } else {
                        project.iProjVer = 0;                    
                        project.bProjVer = false;
                    }
                    project.sCurTime = fieldsParsed["CurTime"];
                    iReturn = 0;
                }
            }
            infile.close();
        }
        return iReturn;
    }
}
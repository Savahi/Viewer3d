#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.hpp"
#include "resources.hpp"

namespace Spider3d {

    static int iLevelPos, iCodePos, iNamePos, iTypePos, iNumberPos;

    static char caLevel[] = "Level";
    static char caCode[] = "Code";
    static char caName[] = "Name";
    static char caType[] = "Type";
    static char caNumber[] = "Number";

    static int nFields = 5;
    static char *cpaFields[] = { caLevel, caCode, caName, caType, caNumber };
    static int  *ipaFields[] = { &iLevelPos, &iCodePos, &iNamePos, &iTypePos, &iNumberPos };

    static int iLevelIndex, iCodeIndex, iNameIndex, iTypeIndex, iNumberIndex;
    static int *ipaFieldIndexes[] = { &iLevelIndex, &iCodeIndex, &iNameIndex, &iTypeIndex, &iNumberIndex };

    int loadResources( Resources& resources, const char *cpFile ) {
        FILE *fp;
        char *cpLine;
        int iStatus;

        fp = fopen( cpFile, "rb" );
        if( fp != NULL ) {
            iStatus = parseFileHeader( fp, nFields, cpaFields, ipaFields );
            if( iStatus != -1 ) {
                while(1) {
                    cpLine = parseFileLine( fp, nFields, ipaFields, ipaFieldIndexes, &iStatus );
                    if( cpLine == NULL ) {
                        break;
                    }
                    Resource resource;
                    int iLevel;
                    int nScanned = sscanf( &cpLine[iLevelIndex], "%d", &resource.iLevel );
                    resource.bLevel = ( nScanned == 1 ) ? true : false;

                    resource.sCode = std::string( trimString( &cpLine[iCodeIndex] ) );
                    resource.sName = std::string( trimString( &cpLine[iNameIndex] ) );
                    resource.sType = std::string( trimString( &cpLine[iTypeIndex] ) );
                    int iNumber;
                    nScanned = sscanf( &cpLine[iNumberIndex], "%d", &resource.iNumber );
                    resource.bNumber = ( nScanned == 1 ) ? true : false;

                    resources.add( resource );

                    free(cpLine);
                }
            }
        }
        return 0;
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.hpp"
#include "oplinks.hpp"

namespace Spider3d {

    static int iPredCodePos, iSuccCodePos, iTypeSFPos, iLagTypePos, iLagUnitPos, iLagPos;

    static char caPredCode[] = "PredCode";
    static char caSuccCode[] = "SuccCode";
    static char caTypeSF[] = "TypeSF";
    static char caLagType[] = "LagType";
    static char caLagUnit[] = "LagUnit";
    static char caLag[] = "Lag";

    static int nFields = 6;
    static char *cpaFields[] = { caPredCode, caSuccCode, caTypeSF, caLagType, caLagUnit, caLag };
    static int  *ipaFields[] = { &iPredCodePos, &iSuccCodePos, &iTypeSFPos, &iLagTypePos, &iLagUnitPos, &iLagPos };

    static int iPredCodeIndex, iSuccCodeIndex, iTypeSFIndex, iLagTypeIndex, iLagUnitIndex, iLagIndex;
    static int *ipaFieldIndexes[] = { &iPredCodeIndex, &iSuccCodeIndex, &iTypeSFIndex, &iLagTypeIndex, &iLagUnitIndex, &iLagIndex };

    int loadOpLinks( OpLinks& opLinks, const char *cpFile ) {
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
                    OpLink opLink;
                    opLink.sPredCode = std::string( trimString( &cpLine[iPredCodeIndex] ) );
                    opLink.sSuccCode = std::string( trimString( &cpLine[iSuccCodeIndex] ) );
                    opLink.sTypeSF = std::string( trimString( &cpLine[iTypeSFIndex] ) );
                    opLink.sLagType = std::string( trimString( &cpLine[iLagTypeIndex] ) );
                    opLink.sLagUnit = std::string( trimString( &cpLine[iLagUnitIndex] ) );
                    int iLag;
                    int nScanned = sscanf( &cpLine[iLagIndex], "%d", &opLink.iLag );
                    opLink.bLag = ( nScanned == 1 ) ? true : false;

                    opLinks.add( opLink );
                    free(cpLine);
                }
            }
        }
        return 0;
    }
}
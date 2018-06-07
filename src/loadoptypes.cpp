#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.hpp"
#include "optypes.hpp"

namespace Spider3d {

    static int _iCodePos, _iNamePos, _iColorPos;

    static char caCode[] = "Code";
    static char caName[] = "Name";
    static char caColor[] = "Color";

    int nFields = 3;
    static char *cpaFields[] = { caCode, caName, caColor };
    static int  *ipaFields[] = { &_iCodePos, &_iNamePos, &_iColorPos };

    static int _iCodeIndex, _iNameIndex, _iColorIndex;
    static int *ipaFieldIndexes[] = { &_iCodeIndex, &_iNameIndex, &_iColorIndex };

    int loadOpTypes( OpTypes& opTypes, const char *cpFile ) {

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
                    if( iStatus == 0 ) {
                        OpType opType;
                        opType.sCode = std::string( trimString( &cpLine[_iCodeIndex] ) );
                        opType.sName = std::string( &cpLine[_iNameIndex] );
                        int nScanned = sscanf( &cpLine[_iColorIndex], "%f %f %f", &opType.fR, &opType.fG, &opType.fB );
                        if( nScanned == 3 ) {
                            opTypes.add( opType );
                        }
                    }
                    free(cpLine);
                }
            }
        }
        return 0;
    }

}
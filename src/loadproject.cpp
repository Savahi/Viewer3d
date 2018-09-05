#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "helpers.hpp"
#include "project.hpp"

namespace Spider3d {

    static int iCodePos, iNamePos, iProjVerPos, iCurTimePos;

    static char caCode[] = "Code";
    static char caName[] = "Name";
    static char caProjVer[] = "ProjVer";
    static char caCurTime[] = "CurTime";

    static int nFields = 4;
    static char *cpaFields[] = { caCode, caName, caProjVer, caCurTime };
    static int  *ipaFields[] = { &iCodePos, &iNamePos, &iProjVerPos, &iCurTimePos };

    static int iCodeIndex, iNameIndex, iProjVerIndex, iCurTimeIndex;
    static int *ipaFieldIndexes[] = { &iCodeIndex, &iNameIndex, &iProjVerIndex, &iCurTimeIndex };

    int loadProject( Project& project, const char *cpFile ) {
        FILE *fp;
        char *cpLine;
        int iStatus;
        int nScanned;
        int iReturn = -1;

        fp = fopen( cpFile, "rb" );
        if( fp != NULL ) {
            iStatus = parseFileHeader( fp, nFields, cpaFields, ipaFields );
            if( iStatus != -1 ) {
                int iParseFileLineStatus;
                cpLine = parseFileLine( fp, nFields, ipaFields, ipaFieldIndexes, &iParseFileLineStatus );
                if( cpLine != NULL ) {
                    if( iParseFileLineStatus == 0 ) {
                        project.sCode = std::string( trimString( &cpLine[iCodeIndex] ) );
                        project.sName = std::string( trimString( &cpLine[iNameIndex] ) );

                        nScanned = sscanf( &cpLine[iProjVerIndex], "%d", &project.iProjVer );
                        if( nScanned == 0 ) {
                            project.iProjVer = 0;
                        }
                        project.bProjVer = true;

                        project.sCurTime = std::string( trimString( &cpLine[iCurTimeIndex] ) );

                        iReturn = 0;
                    }
                    free(cpLine);
                }
            }
            fclose(fp);
        }
        return iReturn;
    }
}
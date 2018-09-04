#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.hpp"
#include "opresassignments.hpp"

namespace Spider3d {

    static int iOperCodePos, iResCodePos, iNumberPos, iPriorPos;

    static char caOperCode[] = "OperCode";
    static char caResCode[] = "ResCode";
    static char caNumber[] = "Number";
    static char caPrior[] = "Prior";

    static int nFields = 4;
    static char *cpaFields[] = { caOperCode, caResCode, caNumber, caPrior };
    static int  *ipaFields[] = { &iOperCodePos, &iResCodePos, &iNumberPos, &iPriorPos };

    static int iOperCodeIndex, iResCodeIndex, iNumberIndex, iPriorIndex;
    static int *ipaFieldIndexes[] = { &iOperCodeIndex, &iResCodeIndex, &iNumberIndex, &iPriorIndex };

    int loadOpResAssignments( OpResAssignments& opResAssignments, const char *cpFile ) {
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
                    OpResAssignment opResAssignment;
                    opResAssignment.sOperCode = std::string( trimString( &cpLine[iOperCodeIndex] ) );
                    opResAssignment.sResCode = std::string( trimString( &cpLine[iResCodeIndex] ) );
                    int iNumber;
                    int nScanned = sscanf( &cpLine[iNumberIndex], "%d", &opResAssignment.iNumber );
                    opResAssignment.bNumber = ( nScanned == 1 ) ? true : false;
                    int iPrior;
                    nScanned = sscanf( &cpLine[iPriorIndex], "%d", &opResAssignment.iPrior );
                    opResAssignment.bPrior = ( nScanned == 1 ) ? true : false;
                    
                    opResAssignments.add(opResAssignment);

                    free(cpLine);
                }
            }
        }
        return 0;
    }
}
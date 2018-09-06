#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "helpers.hpp"
#include "operations.hpp"

namespace Spider3d {

    static int parseDate( char *cpDatetime, struct tm& tmDatetime ); 

    static int iCodePos, iNamePos, iTypePos, iModelCodePos, iLevelPos, 
        iActualStartPos, iActualFinishPos, iAsapStartPos, iAsapFinishPos, iCompareStartPos, iCompareFinishPos, 
        iCriticalPos, iCostTotalPos, iVolSumPos, iDurSumDPos;

    static char caCode[] = "Code", caName[] = "Name", caType[] = "Type", caModelCode[] = "Model", caLevel[] = "Level", 
        caActualStart[] = "FactStart", caActualFinish[] = "FactFin", caAsapStart[] = "AsapStart", caAsapFinish[] = "AsapFin", 
        caCompareStart[] = "Start_COMP", caCompareFinish[] = "Fin_COMP", 
        iCritical[] = "f_Critical", caCostTotal[] = "CostTotal", caVolSum[] = "VolSum", caDurSumD[]= "DurSumD";

    static int nFields = 15;
    static char *cpaFields[] = { caCode, caName, caType, caModelCode, caLevel, caActualStart, caActualFinish, caAsapStart, caAsapFinish, 
        caCompareStart, caCompareFinish, iCritical, caCostTotal, caVolSum, caDurSumD };
    static int  *ipaFields[] = { &iCodePos, &iNamePos, &iTypePos, &iModelCodePos, &iLevelPos, 
        &iActualStartPos, &iActualFinishPos, &iAsapStartPos, &iAsapFinishPos, &iCompareStartPos, &iCompareFinishPos, 
        &iCriticalPos, &iCostTotalPos, &iVolSumPos, &iDurSumDPos };

    static int iCodeIndex, iNameIndex, iTypeIndex, iModelCodeIndex, iLevelIndex, 
        iActualStartIndex, iActualFinishIndex, iAsapStartIndex, iAsapFinishIndex, iCompareStartIndex, iCompareFinishIndex, 
        iCriticalIndex, iCostTotalIndex, iVolSumIndex, iDurSumDIndex;
    static int *ipaFieldIndexes[] = { &iCodeIndex, &iNameIndex, &iTypeIndex, &iModelCodeIndex, &iLevelIndex, 
        &iActualStartIndex, &iActualFinishIndex, &iAsapStartIndex, &iAsapFinishIndex, &iCompareStartIndex, &iCompareFinishIndex, 
        &iCriticalIndex, &iCostTotalIndex, &iVolSumIndex, &iDurSumDIndex };

    int loadOperations( Operations& operations, const char *cpFile ) {
        FILE *fp;
        char *cpLine;
        int iStatus;
        int nScanned;

        fp = fopen( cpFile, "rb" );
        if( fp != NULL ) {            
            iStatus = parseFileHeader( fp, nFields, cpaFields, ipaFields, false );
            if( iStatus != -1 ) {                
                while(1) {
                    int iParseFileLineStatus;
                    cpLine = parseFileLine( fp, nFields, ipaFields, ipaFieldIndexes, &iParseFileLineStatus, false );
                    if( cpLine == NULL ) {
                        break;
                    }
                    if( iParseFileLineStatus == 0 ) {
                        Operation operation;

                        operation.sLevel = (iLevelIndex != -1) ? std::string( &cpLine[iLevelIndex] ) : std::string("");
                        if( iLevelIndex != -1 ) {
                            nScanned = sscanf( &cpLine[iLevelIndex], "%d", &operation.iLevel );
                            operation.bLevel = ( nScanned == 1 ) ? true : false;
                        } else {
                            operation.iLevel = -1;
                            operation.bLevel = false;
                        }

                        operation.sCode = (iCodeIndex != -1) ? std::string( &cpLine[iCodeIndex]  ) : std::string("");
                        operation.sName = (iNameIndex != -1) ? std::string( &cpLine[iNameIndex]  ) : std::string("");
                        operation.sType = (iTypeIndex != -1) ? std::string( &cpLine[iTypeIndex]  ) : std::string("");
                        operation.sModelCode = (iModelCodeIndex != -1) ? std::string( &cpLine[iModelCodeIndex] ) : std::string("");

                        if( iActualStartIndex != -1 ) {                                                   
                            operation.sActualStart = std::string( &cpLine[iActualStartIndex]  ); 
                            iStatus = parseDate( &cpLine[iActualStartIndex], operation.tmActualStart );
                            if( iStatus != -1 ) {
                                operation.tActualStart = mktime( &(operation.tmActualStart) );
                            } 
                        }
                        if( iActualStartIndex == -1 || iStatus == -1 ) {
                            operation.tActualStart = 0;            
                            operation.sActualStart = std::string("");
                        }

                        if( iActualFinishIndex != -1 ) {
                            operation.sActualFinish = std::string( &cpLine[iActualFinishIndex] );
                            iStatus = parseDate( &cpLine[iActualFinishIndex], operation.tmActualFinish );
                            if( iStatus != -1 ) {
                                operation.tActualFinish = mktime( &(operation.tmActualFinish) );
                            } 
                        }
                        if( iActualFinishIndex == -1 || iStatus == -1 ) {
                            operation.tActualFinish = 0;            
                            operation.sActualFinish = std::string("");
                        }

                        if( iAsapStartIndex != -1 ) {
                            operation.sAsapStart = std::string( &cpLine[iAsapStartIndex] );
                            iStatus = parseDate( &cpLine[iAsapStartIndex], operation.tmAsapStart );
                            if( iStatus != -1 ) {
                                operation.tAsapStart = mktime( &(operation.tmAsapStart) );
                            } 
                        }
                        if( iAsapStartIndex == -1 || iStatus == -1 ) {
                            operation.tAsapStart = 0;            
                            operation.sAsapStart = std::string("");
                        }
                        if( iAsapFinishIndex != -1 ) {
                            operation.sAsapFinish = std::string( &cpLine[iAsapFinishIndex] );
                            iStatus = parseDate( &cpLine[iAsapFinishIndex], operation.tmAsapFinish );
                            if( iStatus != -1 ) {
                                operation.tAsapFinish = mktime( &(operation.tmAsapFinish) );
                            }
                        }
                        if( iAsapFinishIndex == -1 || iStatus == -1 ) {
                            operation.tAsapFinish = 0;            
                            operation.sAsapFinish = std::string("");
                        }
                        std::cout << "sAsapStart=" << operation.sAsapStart << std::endl;
                        int iStatus1 = -1, iStatus2 = -1;
                        if( iCompareStartIndex != -1 && iCompareFinishIndex != -1 ) {
                            operation.sCompareStart = std::string( &cpLine[iCompareStartIndex] );
                            iStatus1 = parseDate( &cpLine[iCompareStartIndex], operation.tmCompareStart );
                            if( iStatus1 != -1 ) {
                                operation.tCompareStart = mktime( &(operation.tmCompareStart) );
                            } 
                            operation.sCompareFinish = std::string( &cpLine[iCompareFinishIndex] );
                            iStatus2 = parseDate( &cpLine[iCompareFinishIndex], operation.tmCompareFinish );
                            if( iStatus2 != -1 ) {
                                operation.tCompareFinish = mktime( &(operation.tmCompareFinish) );
                            } 
                        }
                        if( iStatus1 == -1 || iStatus2 == -1 ) {
                            operation.tCompareStart = 0;            
                            operation.sCompareStart = std::string("");
                            operation.tCompareFinish = 0;            
                            operation.sCompareFinish = std::string("");                            
                        }

                        if( iCriticalIndex != -1 ) {
                            nScanned = sscanf( &cpLine[iCriticalIndex], "%d", &operation.iCritical );
                            operation.bCritical = ( nScanned == 1 ) ? true : false;
                        } else {
                            operation.iCritical = -1;
                            operation.bCritical = false;
                        }

                        if( iCostTotalIndex != -1 ) {
                            nScanned = sscanf( &cpLine[iCostTotalIndex], "%f", &operation.fCostTotal );
                            operation.bCostTotal = ( nScanned == 1 ) ? true : false;                            
                        } else {
                            operation.fCostTotal = -1.0;
                            operation.bCostTotal = false;
                        }

                        if( iVolSumIndex != -1 ) {
                            nScanned = sscanf( &cpLine[iVolSumIndex], "%f", &operation.fVolSum );
                            operation.bVolSum = ( nScanned == 1 ) ? true : false;                            
                        } else {
                            operation.fVolSum = -1.0;
                            operation.bVolSum = false;                            
                        }

                        if( iDurSumDIndex != -1 ) {
                            nScanned = sscanf( &cpLine[iDurSumDIndex], "%f", &operation.fDurSumD );
                            operation.bDurSumD = ( nScanned == 1 ) ? true : false;                            
                        } else {
                            operation.fDurSumD = -1.0;
                            operation.bDurSumD = false;                            
                        }

                        // Checking dates are valid...
                        if( (operation.tActualStart > 0 && operation.tActualFinish > 0 ) || // if "finished" ...
                            (operation.tAsapStart > 0 && operation.tAsapFinish > 0) || // ... or not "not started"
                            (operation.tActualStart > 0 && operation.tAsapStart && operation.tAsapFinish > 0) ) { // ...or "partly finished..."
                            operations.add( operation ); // ... then it's ok - adding a new operation.
                        }
                    }
                    free(cpLine);
                }                
            }
            fclose(fp);
        }
        return 0;
    }

    static int parseDate( char *cpDatetime, struct tm& tmDatetime ) {
        int iStatus;
        for( int i = 0 ; cpDatetime[i] != '\x0' ; i++ ) {
            if( cpDatetime[i] == '.' || cpDatetime[i] == '-' || cpDatetime[i] == ':' ) {
                cpDatetime[i] = ' ';
            }
        }
        iStatus = sscanf( cpDatetime, "%d %d %d %d %d", &tmDatetime.tm_mday, &tmDatetime.tm_mon, &tmDatetime.tm_year, &tmDatetime.tm_hour, &tmDatetime.tm_min );
        if( iStatus != 5 ) { 
            return -1;
        }
        tmDatetime.tm_year -= 1900;
        tmDatetime.tm_mon -= 1;
        tmDatetime.tm_sec = 0;
        tmDatetime.tm_wday = tmDatetime.tm_yday = tmDatetime.tm_isdst = 0;
        return 0;
    }

}
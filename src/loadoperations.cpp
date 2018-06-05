#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.hpp"
#include "operations.hpp"

namespace Spider3d {

    static int parseFileHeader( FILE *fp );
    static int parseFileLine( Operations& operations, FILE *fp );

    static int parseDates( char *cpActualStart, char *cpActualFinish, char *cpAsapStart, char *cpAsapFinish, Operation& operation );

    static int getValuesByColumnPos( char *cpLine, int *ipModel, int *ipFactStart, int *ipFactFin, int *ipAsapStart, int *ipAsapFin );

    static int _iModelPos, _iFactStartPos, _iFactFinPos, _iAsapStartPos, _iAsapFinPos;

    int loadOperations( Operations& operations, const char *cpFile ) {

        FILE *fp;
        int iStatus;

        fp = fopen( cpFile, "rb" );
        if( fp != NULL ) {
            iStatus = parseFileHeader( fp );
            if( iStatus != -1 ) {
                while(1) {
                    iStatus = parseFileLine( operations, fp );
                    if( iStatus == -1 ) {
                        break;
                    }
                }
            }
            fclose(fp);
        }

        return 0;
    }

    static int parseFileHeader( FILE *fp ) {
        int iReturn=0, i, iStatus, iStart;
        char *cpHeader;

        cpHeader = readLineFromFile(fp);
        if( cpHeader == NULL ) {
            return -1;
        }

        _iModelPos = getPosByColumnName( cpHeader, "model" );
        if( _iModelPos == -1 ) {
            iReturn = -1;
        }
        _iFactStartPos = getPosByColumnName( cpHeader, "factstart" );
        if( _iFactStartPos == -1 ) {
            iReturn = -1;
        }
        _iFactFinPos = getPosByColumnName( cpHeader, "factfin" );
        if( _iFactFinPos == -1 ) {
            iReturn = -1;
        }
        _iAsapStartPos = getPosByColumnName( cpHeader, "asapstart" );
        if( _iAsapStartPos == -1 ) {
            iReturn = -1;
        }
        _iAsapFinPos = getPosByColumnName( cpHeader, "asapfin" );
        if( _iAsapFinPos == -1 ) {
            iReturn = -1;
        }

        free(cpHeader);
        return iReturn;
    }

    static int parseFileLine( Operations& operations, FILE *fp ) {
        int iStatus, iModel, iFactStart, iFactFin, iAsapStart, iAsapFin;
        char *cpLine; 

        cpLine = readLineFromFile(fp);
        if( cpLine == NULL ) {
            return -1;
        }

        iStatus = getValuesByColumnPos( cpLine, &iModel, &iFactStart, &iFactFin, &iAsapStart, &iAsapFin );
        if( iStatus == 0 ) {
            Operation operation;
            if( parseDates( &cpLine[iFactStart], &cpLine[iFactFin], &cpLine[iAsapStart], &cpLine[iAsapFin], operation ) == 0 ) {
                operation.sModelCode = std::string( &cpLine[iModel] );
                operations.add( operation );
            }
        }
        free(cpLine);
        return 0;
    }


    static int getValuesByColumnPos( char *cpLine, int *ipModel, int *ipFactStart, int *ipFactFin, int *ipAsapStart, int *ipAsapFin ) {
        int i, iPos, iLineLen;
        *ipModel=-1; 
        *ipFactStart=-1;
        *ipFactFin=-1;
        *ipAsapStart=-1;
        *ipAsapFin=-1;

        iLineLen = strlen( cpLine ); 
        for( i = 0, iPos = 0 ; i < iLineLen ; i++ ) {
            if( iPos == _iModelPos && *ipModel == -1 ) {
                *ipModel = i;
            } else if( iPos == _iFactStartPos && *ipFactStart == -1 ) {
                *ipFactStart = i;
            } else if( iPos == _iFactFinPos && *ipFactFin == -1 ) { 
                *ipFactFin = i;
            } else if( iPos == _iAsapStartPos && *ipAsapStart == -1 ) {
                *ipAsapStart = i;
            } else if( iPos == _iAsapFinPos && *ipAsapFin == -1 ) { 
                *ipAsapFin = i;
            }
            if( cpLine[i] == ';' ) {
                iPos++;
                cpLine[i] = '\x0';
            } else if( cpLine[i] == '\r' || cpLine[i] == '\n') {
                cpLine[i] = '\x0';            
            }
        }
        if( *ipModel == -1 || *ipFactStart == -1 || *ipFactFin == -1 || *ipAsapStart == -1 || *ipAsapFin == -1 ) {
            return -1;
        }
        return 0;
    }


    static int parseDate( char *cpDatetime, struct tm& tmDatetime ) {
        int iStatus;
        for( int i = 0 ; cpDatetime[i] != '\x0' ; i++ ) {
            if( cpDatetime[i] == '.' || cpDatetime[i] == ':' ) {
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

    static int parseDates( char *cpActualStart, char *cpActualFinish, char *cpAsapStart, char *cpAsapFinish, Operation& operation ) {
        int iStatus;

        iStatus = parseDate( cpActualStart, operation.tmActualStart );
        if( iStatus == -1 ) {
            return -1;
        }
        operation.tActualStart = mktime( &(operation.tmActualStart) );

        iStatus = parseDate( cpActualFinish, operation.tmActualFinish );
        if( iStatus == -1 ) {
            return -1;
        }
        operation.tActualFinish = mktime( &(operation.tmActualFinish) );
        
        iStatus = parseDate( cpAsapStart, operation.tmAsapStart );
        if( iStatus == -1 ) {
            return -1;
        }
        operation.tAsapStart = mktime( &(operation.tmAsapStart) );

        iStatus = parseDate( cpAsapFinish, operation.tmAsapFinish );
        if( iStatus == -1 ) {
            return -1;
        }
        operation.tAsapFinish = mktime( &(operation.tmAsapFinish) );
        
        operation.bDatesInitialized = true;

        return 0;
    }

}
#include <stdio.h>
#include "Helpers.hpp"
#include "Operations.hpp"

namespace Spider3d {

    static int _objectPos, _factStartPos, _factFinPos, _asapStartPos, _asapFinPos;

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

        _objectPos = getPosByColumnName( cpHeader, "model" );
        if( _objectPos == -1 ) {
            iReturn = -1;
        }
        _factStartPos = getPosByColumnName( cpHeader, "factstart" );
        if( _factStartPos == -1 ) {
            iReturn = -1;
        }
        _factFinPos = getPosByColumnName( cpHeader, "factfin" );
        if( _factFinPos == -1 ) {
            iReturn = -1;
        }
        _asapStartPos = getPosByColumnName( cpHeader, "asapstart" );
        if( _asapStartPos == -1 ) {
            iReturn = -1;
        }
        _asapFinPos = getPosByColumnName( cpHeader, "asapfin" );
        if( _asapFinPos == -1 ) {
            iReturn = -1;
        }

        free(cpHeader);
        return iReturn;
    }

    static int parseFileLine( Operations& operations, FILE *fp ) {
        int iStatus, iObject, iFactStart, iFactFin, iAsapStart, iAsapFin;
        char *cpLine; 

        cpLine = readLineFromFile(fp);
        if( cpLine == NULL ) {
            return -1;
        }

        iStatus = getValuesByColumnPos( cpLine, &iObject, &iFactStart, &iFactFin, &iAsapStart, &iAsapFin );
        if( iStatus == 0 ) {
            Operation operation;
            if( parseDates( &cpLine[iFactStart], &cpLine[iFactFin], &cpLine[iAsapStart], &cpLine[iAsapFin], operation ) == 0 ) {
                operation.mObjectId = &cpLine[iObject];
                operations.add( operation );
            }
        }
        free(cpLine);
        return 0;
    }


    static int getValuesByColumnPos( char *cpLine, int *ipObect, int *ipFactStart, int *ipFactFin, int *ipAsapStart, int *ipAsapFin ) {
        int i, iPos, iLineLen;
        *ipObect=-1; 
        *ipFactStart=-1;
        *ipFactFin=-1;
        *ipAsapStart=-1;
        *ipAsapFin=-1;

        iLineLen = strlen( cpLine ); 
        for( i = 0, iPos = 0 ; i < iLineLen ; i++ ) {
            if( iPos == _objectPos && *ipObect == -1 ) {
                *ipObect = i;
            } else if( iPos == _factStartPos && *ipFactStart == -1 ) {
                *ipFactStart = i;
            } else if( iPos == _factFinPos && *ipFactFin == -1 ) { 
                *ipFactFin = i;
            } else if( iPos == _asapStartPos && *ipAsapStart == -1 ) {
                *ipAsapStart = i;
            } else if( iPos == _asapFinPos && *ipAsapFin == -1 ) { 
                *ipAsapFin = i;
            }
            if( cpLine[i] == ';' ) {
                iPos++;
                cpLine[i] = '\x0';
            } else if( cpLine[i] == '\r' || cpLine[i] == '\n') {
                cpLine[i] = '\x0';            
            }
        }
        if( *ipObect == -1 || *ipFactStart == -1 || *ipFactFin == -1 || *ipAsapStart == -1 || *ipAsapFin == -1 ) {
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

        iStatus = parseDate( cpActualStart, operation.mActualStart );
        if( iStatus == -1 ) {
            return -1;
        }
        operation.mActualStartMs = mktime( &(operation.mActualStart) );

        iStatus = parseDate( cpActualFinish, operation.mActualFinish );
        if( iStatus == -1 ) {
            return -1;
        }
        operation.mActualFinishMs = mktime( &(operation.mActualFinish) );
        
        iStatus = parseDate( cpAsapStart, operation.mAsapStart );
        if( iStatus == -1 ) {
            return -1;
        }
        operation.mAsapStartMs = mktime( &(operation.mAsapStart) );

        iStatus = parseDate( cpAsapFinish, operation.mAsapFinish );
        if( iStatus == -1 ) {
            return -1;
        }
        operation.mAsapFinishMs = mktime( &(operation.mAsapFinish) );
        
        operation.mDatesInitialized = true;

        return 0;
    }

}
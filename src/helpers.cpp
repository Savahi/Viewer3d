
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <ctype.h>
#include "helpers.hpp"

namespace Spider3d {

    char *readLineFromFile( FILE *fp ) {
        int nAllocated, iAllocatedPtr;
        char cRead;
        char *cpAllocated;

        if( feof(fp) ) {
            return NULL;
        }

        cpAllocated = (char*)malloc( READ_BUFFER_SIZE );
        if( cpAllocated == NULL ) {
            return NULL;
        }
        nAllocated = READ_BUFFER_SIZE;
        iAllocatedPtr = 0;

        while( ( cRead = fgetc( fp ) ) != EOF ) {
            if( iAllocatedPtr >= nAllocated-1 ) {
                if( nAllocated == 0 ) {
                    cpAllocated = (char*)malloc( READ_BUFFER_SIZE );
                } else {
                    cpAllocated = (char*)realloc( cpAllocated, nAllocated + READ_BUFFER_SIZE );
                }
                if( cpAllocated == NULL ) {
                    return NULL;
                }
                nAllocated += READ_BUFFER_SIZE;
            }
            cpAllocated[iAllocatedPtr] = cRead;
            iAllocatedPtr++;
            if( cRead == '\n' ) {
                break;
            }
        }
        cpAllocated[iAllocatedPtr] = '\x0';
        return cpAllocated;
    }


    int findTagContent( char *cpText, const char *cpTagName, int iStartAt, int iStopAt, int *ipStart, int *ipEnd ) {
        char caOpenTag[MAX_TAG_NAME+3];
        char caCloseTag[MAX_TAG_NAME+4];
        int iFound, iOpenTagStart, iOpenTagEnd, iCloseTagStart, iCloseTagEnd;

        if( strlen(cpTagName) > MAX_TAG_NAME ) {
            return 0;
        }

        sprintf( caOpenTag, "<%s>", cpTagName );
        sprintf( caCloseTag, "</%s>", cpTagName );

        iFound = findSubstring( cpText, caOpenTag, iStartAt, iStopAt, &iOpenTagStart, &iOpenTagEnd );
        if( iFound == 0 ) {
            return 0;
        }
        iFound = findSubstring( cpText, caCloseTag, iOpenTagEnd, iStopAt, &iCloseTagStart, &iCloseTagEnd );
        if( iFound == 0 ) {
            return 0;
        }

        if( ipStart != NULL ) {
            *ipStart = iOpenTagEnd+1;
        }
        if( ipEnd != NULL ) {
            *ipEnd = iCloseTagStart-1;
        }
        return 1;
    }

    int findSubstring( char *cpText, const char *cpSubstring, int iStartAt, int iStopAt, int *ipStart, int *ipEnd ) {
        int iFound = 0;
        int i, iTextLen, iSubstringLen, iSubstringIndex;

        iTextLen = strlen( cpText );
        if( iStopAt == -1 ) { 
            iStopAt = iTextLen-1;
        }
        iSubstringLen = strlen( cpSubstring );
        iSubstringIndex = 0;
        for( i = iStartAt ; i <= iStopAt ; i++ ) {
            if( tolower(cpSubstring[iSubstringIndex]) == tolower(cpText[i]) ) {
                iSubstringIndex++;
                if( iSubstringIndex == iSubstringLen ) {
                    if( ipStart != NULL ) {
                        *ipStart = i-iSubstringLen+1;
                    }
                    if( ipEnd != NULL ) {
                        *ipEnd = i;
                    }
                    iFound = 1;
                    break;
                }
            } else {
                if( iSubstringIndex > 0 ) {
                    iSubstringIndex = 0;
                }
            }
        }
        return iFound;
    }

    int getPosByColumnName( char *cpBuffer, const char *cpColumn ) {
        int iPosition=-1, i, iStatus, iStart;

        iStatus = findSubstring( cpBuffer, cpColumn, 0, -1, &iStart, NULL );
        if( iStatus == 1 ) {
            iPosition = 0;
            for( i = 0 ; i < iStart ; i++ ) {
                if( cpBuffer[i] == ';' ) {
                    iPosition++;
                }
            }
        }
        return iPosition;
    }

    std::string& ltrim(std::string& str, const std::string& chars )
    {
        str.erase(0, str.find_first_not_of(chars));
        return str;
    }
     
    std::string& rtrim(std::string& str, const std::string& chars )
    {
        str.erase(str.find_last_not_of(chars) + 1);
        return str;
    }
     
    std::string& trim(std::string& str, const std::string& chars )
    {
        return ltrim( rtrim(str, chars), chars );
    }


    char *trimString( char *cp ) {
        for( int i = 0 ; cp[i] == ' ' && cp[i] != '\x0' ; ) {
            for( int j = i ; cp[j] != '\x0' ; j++ ) {
                cp[j] = cp[j+1];
            }
        }
        for( int i = strlen(cp)-1 ; (cp[i] == ' ' || cp[i] == '\r' || cp[i] == '\n') && i >= 0 ; i-- ) {
            cp[i] = cp[i+1];
        }
        return cp;
    }


    int timetToStr( time_t timetDT, char *cpBuffer, int iBufferSize, bool bAscTime, bool bHHMM, char cDMYDelim ) {
        int iReturn = -1;

        strcpy( cpBuffer, "" );

        struct tm *tmDT = localtime( &timetDT );

        if( bAscTime ) {
            char *cp = asctime( tmDT );
            if( strlen(cp) < iBufferSize ) {
                strcpy( cpBuffer, cp );
                iReturn = 0;
            } 
        } else if( (tmDT->tm_year > 0 && tmDT->tm_year < 1000) && 
                    (tmDT->tm_mon >= 0 ) && (tmDT->tm_mon <= 11) && 
                    (tmDT->tm_mday >= 0) && (tmDT->tm_mday <= 31) &&
                    (tmDT->tm_hour >= 0) && (tmDT->tm_hour <= 24) && 
                    (tmDT->tm_min >= 0) && (tmDT->tm_min <= 60) && iBufferSize > 28 ) 
        {
            if( (tmDT->tm_hour > 0 || tmDT->tm_min > 0) && bHHMM ) {
                sprintf( cpBuffer, "%02d%c%02d%c%4d %02d:%02d", 
                    tmDT->tm_mday, cDMYDelim, (tmDT->tm_mon+1), cDMYDelim, (tmDT->tm_year+1900), tmDT->tm_hour, tmDT->tm_min );
            } else {
                sprintf( cpBuffer, "%02d%c%02d%c%4d", tmDT->tm_mday, cDMYDelim, (tmDT->tm_mon+1), cDMYDelim, (tmDT->tm_year+1900) );            
            }
            iReturn = 0;
        }
        return iReturn;
    }


    int parseFileHeader( FILE *fp, int nFields, char **cpaFields, int **ipaFields ) {
        int iReturn=0;
        char *cpHeader;

        cpHeader = readLineFromFile(fp);
        if( cpHeader == NULL ) {
            return -1;
        }

        for( int i = 0 ; i < nFields ; i++ ) {
            int iPos = getPosByColumnName( cpHeader, cpaFields[i] );
            if( iPos == -1 ) {
                iReturn = -1;
                break;
            }
            *ipaFields[i] = iPos;
        }

        free(cpHeader);
        return iReturn;
    }

    static int parseFileLineIntoFields( char *cpLine, int nFields, int **ipaFields, int **ipaFieldIndexes ); 

    char *parseFileLine( FILE *fp, int nFields, int **ipaFields, int **ipaFieldIndexes, int *ipStatus ) {
        char *cpLine; 

        cpLine = readLineFromFile(fp);
        if( cpLine == NULL ) {
            return NULL;
        }

        *ipStatus = parseFileLineIntoFields( cpLine, nFields, ipaFields, ipaFieldIndexes );
        return cpLine;
    }


    static int parseFileLineIntoFields( char *cpLine, int nFields, int **ipaFields, int **ipaFieldIndexes ) 
    {
        int iPos, iLineLen;

        for( int i = 0 ; i < nFields ; i++ ) {
            *ipaFieldIndexes[i] = -1;
        }

        iLineLen = strlen( cpLine ); 
        for( int i = 0, iPos = 0 ; i < iLineLen ; i++ ) {
            for( int iF = 0 ; iF < nFields ; iF++ ) {
                if( iPos == *ipaFields[iF]  && *ipaFieldIndexes[iF] == -1 ) {
                    *ipaFieldIndexes[iF] = i;
                    break;
                }
            }
            if( cpLine[i] == ';' ) {
                iPos++;
                cpLine[i] = '\x0';
            } else if( cpLine[i] == '\r' || cpLine[i] == '\n') {
                cpLine[i] = '\x0';            
            }
        }

        for( int i = 0 ; i < nFields ; i++ ) {
            if( *ipaFieldIndexes[i] == -1 ) {
                return -1;
            }
        }
        return 0;
    }

}
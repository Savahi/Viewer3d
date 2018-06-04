
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "Helpers.hpp"

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
    
}
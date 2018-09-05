#pragma once
#include <stdio.h>
#include <string>
#include <ctime>

namespace Spider3d {

	#define MAX_TAG_NAME 40

	#define READ_BUFFER_SIZE 1024

	char *readLineFromFile( FILE *fp );

	int findTagContent( char *cpText, const char *cpTagName, int iStartAt, int iStopAt, int *ipStart, int *ipEnd );

	int findSubstring( char *cpText, const char *cpSubstring, 
        int iStartAt, int iStopAt, int *ipStart, int *ipEnd, bool bWholeWordSearch=true );

    int getPosByColumnName( char *cpBuffer, const char *cpColumn );	

    std::string& ltrim(std::string& str, const std::string& chars = "\t\r\n ");
    std::string& rtrim(std::string& str, const std::string& chars = "\t\r\n ");
    std::string& trim(std::string& str, const std::string& chars = "\t\r\n ");

    char *trimString( char *cp );

    int timetToStr( time_t timetDT, char *cpBuffer, int iBufferSize=0, bool bAscTime=false, bool bHHMM=true, char cDMYDelim='.' );

    int parseFileHeader( FILE *fp, int nFields, char **cpaFields, int **ipaFields, bool allFieldsRequired=true );
    char *parseFileLine( FILE *fp, int nFields, int **ipaFields, int **ipaFieldIndexes, int *ipStatus, bool allFieldsRequired=true );
}
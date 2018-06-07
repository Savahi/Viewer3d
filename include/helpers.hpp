#pragma once
#include <stdio.h>
#include <string>
#include <ctime>

namespace Spider3d {

	#define MAX_TAG_NAME 40

	#define READ_BUFFER_SIZE 512

	char *readLineFromFile( FILE *fp );

	int findTagContent( char *cpText, const char *cpTagName, int iStartAt, int iStopAt, int *ipStart, int *ipEnd );

	int findSubstring( char *cpText, const char *cpSubstring, int iStartAt, int iStopAt, int *ipStart, int *ipEnd );

    int getPosByColumnName( char *cpBuffer, const char *cpColumn );	

    std::string& ltrim(std::string& str, const std::string& chars = "\t\r\n ");
    std::string& rtrim(std::string& str, const std::string& chars = "\t\r\n ");
    std::string& trim(std::string& str, const std::string& chars = "\t\r\n ");

    char *trimString( char *cp );

    int timetToStr( time_t timetDT, char *cpBuffer, int iBufferSize=0, bool bAscTime=false );

    int parseFileHeader( FILE *fp, int, char **, int ** );
    char *parseFileLine( FILE *fp, int, int **, int **, int * );
}
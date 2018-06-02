#pragma once
#include "Model3d.hpp" 
#include "Operations.hpp"

namespace Spider3d {

	#define MAX_TAG_NAME 40

	#define READ_BUFFER_SIZE 512

	char *readLineFromFile( FILE *fp );

	int findTagContent( char *cpText, const char *cpTagName, int iStartAt, int iStopAt, int *ipStart, int *ipEnd );

	int findSubstring( char *cpText, const char *cpSubstring, int iStartAt, int iStopAt, int *ipStart, int *ipEnd );

	int loadModel( Model3d& model, const char *cpFile );

	int loadOperations( Operations& operations, const char *cpFile );

}
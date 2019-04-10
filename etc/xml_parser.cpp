#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <string>
#include <string.h>

int xmlLoad( char *fileName, std::string& fileContents );

char *xmlLoad( char *fileName );
char *xmlReadTagValue( char *tag, char *contents );
char *xmlGetNextTag( char *_contents, int &_offset );
int xmlFindStartingBracket( char *contents );

int xmlSubstrInStr( char *substr, char *str );


int main( int argc, char **argv ) {

	if( argc < 2 ) {
		std::cout << "Use " << argv[0] << " <xml-file-name>" << std::endl;
	} else {
		char *xml = xmlLoad( argv[1] );
		if( xml != NULL ) 
		{
			char *code = xmlReadTagValue( (char*)"Code", xml );
			if( code != NULL ) {
				std::cout << "code=" << code << std::endl;
				delete [] code;
			}
			char *name = xmlReadTagValue( (char*)"Name", xml );
			if( name != NULL ) {
				std::cout << "code=" << name << std::endl;
				delete [] name;
			}
			char *version = xmlReadTagValue( (char*)"Ver", xml );
			char *start = xmlReadTagValue( (char*)"Start", xml );
			char *finish = xmlReadTagValue( (char*)"Finish", xml );
			char *calendars = xmlReadTagValue( (char*)"Calendars", xml );
			if( calendars != NULL ) {
				int offset = 0;

				for( int i = 0 ; ; i++ ) {
					char *tag = xmlGetNextTag( calendars, offset );
					if( tag == NULL ) {
						break;
					}
					std::cout << "tag=" << tag << " (offset=" << offset << ")" << std::endl;

					char *calendar = xmlReadTagValue( tag, calendars );
					std::cout << "calendar=" << calendar << std::endl;

					delete [] calendar;
					delete [] tag;
				}
			}

			delete [] xml;
		} 
		else 
		{
			std::cout << -1 << std::endl;
		}
	}
}

int xmlFindStartingBracket( char *contents ) {
	int returnValue = -1;
	int startingBracket = 0, overallOffset = 0;
	int len = strlen(contents);

	while(true) {
		startingBracket = xmlSubstrInStr( (char*)"<", &contents[startingBracket] );
		if( startingBracket == -1 ) {
			break;
		}
		if( startingBracket >= len-2 ) {
			break;
		}
		if( contents[startingBracket+1] != '/' ) {
			returnValue = overallOffset + startingBracket;
			break;
		}
		startingBracket += 1;
		overallOffset += startingBracket;
	}
	return returnValue;
}


char *xmlGetNextTag( char *_contents, int &_offset ) {
	char *tag=NULL;

	if( _offset < strlen(_contents) ) {
		char *contents = &_contents[_offset];
		int startingBracket = xmlFindStartingBracket( contents );
		if( startingBracket >= 0 ) {
			std::cout << "startingBracket=" << startingBracket << std::endl;
			int endingBracket = xmlSubstrInStr( (char*)">", &contents[startingBracket] );
			std::cout << "endingBracket=" << endingBracket << std::endl;
			if( endingBracket > 1 ) {
				tag = new char [ endingBracket ];
				if( tag != 0x0 ) {
					strncpy( tag, &contents[startingBracket+1], endingBracket-1 );
					tag[endingBracket-1] = '\x0';

					_offset += startingBracket + endingBracket + 1;
				}
			}
		}
	}
	return tag;
}


char *xmlNextTagValue( char *_contents, int &_offset ) {
	char *value=NULL;

	if( _offset < strlen(_contents) ) {
		char *contents = &_contents[_offset];
		int startingBracket = xmlFindStartingBracket( contents );
		if( startingBracket >= 0 ) {
			int endingBracket = xmlSubstrInStr( (char*)">", &contents[startingBracket] );
			if( endingBracket > 1 ) {
				char *tag = new char [ endingBracket ];
				if( tag != 0x0 ) {
					strncpy( tag, &contents[startingBracket], endingBracket-1 );
					tag[endingBracket-1] = '\x0';

					value = xmlReadTagValue( tag, &contents[_offset] );
					_offset += (startingBracket + endingBracket);

					delete [] tag;
				}
			} else if (endingBracket == 1 ) {
				_offset += startingBracket + 1;
			} else {
				_offset = -1;				
			}
		} else {
			_offset = -1;
		}
	} else {
		_offset = -1;
	}
	return value;
}



char *xmlReadTagValue( char *tag, char *contents ) {
	char *buffer=NULL;

	int tagLen = strlen(tag);
	char *openingTag = new char [tagLen+3];
	if( openingTag != 0x0 ) {
		openingTag[0] = '<';
		openingTag[1] = '\x0';
		strcat( openingTag, tag);
		openingTag[tagLen+1] = '>';	
		openingTag[tagLen+2] = '\x0';	

		int openingTagStart = xmlSubstrInStr( openingTag, contents ); 
		if( openingTagStart >= 0 ) {
			char *closingTag = new char [tagLen+4];
			if( closingTag != 0x0 ) {
				closingTag[0] = '<';
				closingTag[1] = '/';
				closingTag[2] = '\x0';
				strcat( closingTag, tag );
				closingTag[tagLen+2] = '>';	
				closingTag[tagLen+3] = '\x0';

				int tagContentStart = openingTagStart+tagLen+2;
				int closingTagStart = xmlSubstrInStr( closingTag, &contents[tagContentStart] ); 
				if( closingTagStart >= 0 ) {
					int bufferLen = closingTagStart;
					buffer = new char [bufferLen+1];
					strncpy( buffer, &contents[tagContentStart], bufferLen);
					buffer[bufferLen] = '\x0';	
				}			
				delete [] closingTag;
			}
		}			
		delete [] openingTag;
	}
	return buffer;
}


char * xmlLoad( char *fileName ) {
	char *returnValue = NULL;

	std::ifstream in( fileName, std::ios::binary );
	if( in.is_open() ) {
	    
	    in.seekg(0, std::ios::end);
	    size_t len = in.tellg();
	    in.seekg(0);

	    char *buffer = new char [len+1];
	    if( buffer != 0x0 ) {
		    in.read( buffer, len );
		    buffer[len] = '\x0';
		    in.close();	    	
		    returnValue = buffer;
	    }
	}
	return returnValue;
}


int xmlSubstrInStr( char *substr, char *str ) {
	int returnValue = -1;
	int istr, isubstr;

	int strLen = strlen(str);
	int substrLen = strlen( substr );
	if( substrLen <= strLen ) {
		for( int i = 0 ; i < strLen - substrLen + 1 ; i++ ) {
			bool substringFound = true;
			for( istr=i, isubstr=0 ; isubstr < substrLen ; istr++, isubstr++ ) {
				if( str[istr] != substr[isubstr] ) {
					substringFound=false;
					break;
				}
			}
			if( substringFound ) {
				returnValue = i;
				break;
			}
		}
	}
	return returnValue;
}


int xmlLoad( char *fileName, std::string& fileContents ) {
	int returnValue = -1;

	std::ifstream in( fileName, std::ifstream::binary );
	if ( in.is_open() ) {
	    
	    in.seekg(0, std::ios::end);
	    size_t len = in.tellg();
	    in.seekg(0);

	    fileContents.reserve(len);
	    while( !in.eof() ) {
      		fileContents += in.get();
    	}	
	    in.close();

	    returnValue = len;
	}
	return returnValue;
}

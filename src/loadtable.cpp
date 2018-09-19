
#include "stdafx.h"

namespace Spider3d {

    int loadTable( Table& table, const char *cpFile ) {
        int iStatus;
        int nScanned;

        std::ifstream infile( cpFile );
        if( !infile.is_open() ) {
            return -1;
        }
        int numParsed = parseAllFileHeader( infile, table.fieldsNames, table.fieldsPositions );
        if( numParsed != -1 ) {
            while(1) {
                std::vector<std::string> fieldsParsed;    
                int numParsed = parseFileLine( infile, fieldsParsed );
                if( numParsed == -1 ) {
                    break;
                }
                TableLine tl;
                for( int i = 0 ; i < fieldsParsed.size() ; i++ ) {
                    tl.fields.push_back( fieldsParsed[ i ] );                
                }
                table.add( tl );
            }
        }
        infile.close();
        return table.size();
    }
}
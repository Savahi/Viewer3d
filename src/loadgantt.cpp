
#include "stdafx.h"

namespace Spider3d {

    int loadGantt( Gantt& gantt, const char *cpFile ) {
        int iStatus;
        int nScanned;

        std::ifstream infile( cpFile );
        if( !infile.is_open() ) {
            return -1;
        }
        int numParsed = parseAllFileHeader( infile, gantt.fieldsNames, gantt.fieldsPositions );
        if( numParsed != -1 ) {
            int numTitlesParsed = parseAllFileHeaderTitles( infile, gantt.fieldsNames, gantt.fieldsTitles );
            if( numTitlesParsed == numParsed ) {
                int numFlagsParsed = parseAllFileHeaderFlags( infile, gantt.fieldsNames, gantt.fieldsFlags );
                if( numFlagsParsed != numParsed ) {
                    while(1) {
                        std::vector<std::string> fieldsParsed;    
                        int numParsed = parseFileLine( infile, fieldsParsed );
                        if( numParsed == -1 ) {
                            break;
                        }
                        GanttOperation ga;
                        for( int i = 0 ; i < fieldsParsed.size() ; i++ ) {
                            ga.fields.push_back( fieldsParsed[ i ] );                
                        }
                        gantt.add( ga );
                    }
                }
            }
        }
        infile.close();
        return gantt.number();
    }
}
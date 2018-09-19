#include "stdafx.h"

namespace Spider3d {

    static int fieldsToReadNum = 15;
    static const char *fieldsToRead[] = { "Code", "Name", "Type", "Model", "Level", 
        "Start", "Fin", "FactStart", "FactFin", "AsapStart", "AsapFin", 
        "Start_COMP", "Fin_COMP", "f_Critical", "CostTotal", "VolSum", "DurSumD", "Notes" };

    int loadOperations( Operations& operations, const char *cpFile ) {
        int iStatus;
        int nScanned;

        std::vector<std::string> fieldsNames;
        for( int i= 0 ; i < fieldsToReadNum ; i++ ) {
            fieldsNames.push_back( fieldsToRead[i] );
        }
        std::map<std::string,int> fieldsPositions;

        std::ifstream infile( cpFile );
        if( !infile.is_open() ) {
            return -1;
        }

        int numHeaderParsed = parseFileHeader( infile, fieldsNames, fieldsPositions );
        if( numHeaderParsed != -1 ) {
            while(1) {
                std::map<std::string,std::string> fieldsParsed;    
                int numParsed = parseFileLine( infile, fieldsPositions, fieldsParsed );
                if( numParsed == -1 ) {
                    break;
                }
                Operation operation;

                operation.sLevel = fieldsParsed["Level"];
                nScanned = sscanf( operation.sLevel.c_str(), "%d", &operation.iLevel );
                operation.bLevel = ( nScanned == 1 ) ? true : false;

                operation.sCode = fieldsParsed["Code"];
                operation.sName = fieldsParsed["Name"];
                operation.sType = fieldsParsed["Type"];

                operation.sModelCode = fieldsParsed["ModelCode"];

                operation.sStart = fieldsParsed["Start"]; 
                iStatus = parseDatetime( operation.sStart, operation.tmStart );
                if( iStatus != -1 ) {
                    operation.tStart = mktime( &(operation.tmStart) );
                } else {
                    operation.tStart = 0;            
                    operation.sStart = std::string("");
                }
                operation.sFinish = fieldsParsed["Fin"]; 
                iStatus = parseDatetime( operation.sFinish, operation.tmFinish );
                if( iStatus != -1 ) {
                    operation.tFinish = mktime( &(operation.tmFinish) );
                } else {
                    operation.tFinish = 0;            
                    operation.sFinish = std::string("");
                }

                operation.sActualStart = fieldsParsed["FactStart"]; 
                iStatus = parseDatetime( operation.sActualStart, operation.tmActualStart );
                if( iStatus != -1 ) {
                    operation.tActualStart = mktime( &(operation.tmActualStart) );
                } else {
                    operation.tActualStart = 0;            
                    operation.sActualStart = std::string("");
                }
                operation.sActualFinish = fieldsParsed["FactFin"]; 
                iStatus = parseDatetime( operation.sActualFinish, operation.tmActualFinish );
                if( iStatus != -1 ) {
                    operation.tActualFinish = mktime( &(operation.tmActualFinish) );
                } else {
                    operation.tActualFinish = 0;            
                    operation.sActualFinish = std::string("");
                }

                operation.sAsapStart = fieldsParsed["AsapStart"]; 
                iStatus = parseDatetime( operation.sAsapStart, operation.tmAsapStart );
                if( iStatus != -1 ) {
                    operation.tAsapStart = mktime( &(operation.tmAsapStart) );
                } else {
                    operation.tAsapStart = 0;            
                    operation.sAsapStart = std::string("");
                }
                operation.sAsapFinish = fieldsParsed["AsapFin"]; 
                iStatus = parseDatetime( operation.sAsapFinish, operation.tmAsapFinish );
                if( iStatus != -1 ) {
                    operation.tAsapFinish = mktime( &(operation.tmAsapFinish) );
                } else {
                    operation.tAsapFinish = 0;            
                    operation.sAsapFinish = std::string("");
                }

                int iStatus1 = -1, iStatus2 = -1;
                operation.sCompareStart = fieldsParsed["Start_COMP"];
                iStatus1 = parseDatetime( operation.sCompareStart, operation.tmCompareStart );
                if( iStatus1 != -1 ) {
                    operation.tCompareStart = mktime( &(operation.tmCompareStart) );
                } 
                operation.sCompareFinish = fieldsParsed["Fin_COMP"];
                iStatus2 = parseDatetime( operation.sCompareFinish, operation.tmCompareFinish );
                if( iStatus2 != -1 ) {
                    operation.tCompareFinish = mktime( &(operation.tmCompareFinish) );
                } 
                if( iStatus1 == -1 || iStatus2 == -1 ) {
                    operation.tCompareStart = 0;            
                    operation.sCompareStart = std::string("");
                    operation.tCompareFinish = 0;            
                    operation.sCompareFinish = std::string("");                            
                }

                nScanned = sscanf( fieldsParsed["f_Critical"].c_str(), "%d", &operation.iCritical );
                operation.bCritical = ( nScanned == 1 ) ? true : false;

                nScanned = sscanf( fieldsParsed["CostTotal"].c_str(), "%f", &operation.fCostTotal );
                operation.bCostTotal = ( nScanned == 1 ) ? true : false;                            

                nScanned = sscanf( fieldsParsed["VolSum"].c_str(), "%f", &operation.fVolSum );
                operation.bVolSum = ( nScanned == 1 ) ? true : false;                            

                nScanned = sscanf( fieldsParsed["DurSumD"].c_str(), "%f", &operation.fDurSumD );
                operation.bDurSumD = ( nScanned == 1 ) ? true : false;                            

                // Checking dates are valid...
                if( (operation.tActualStart > 0 && operation.tActualFinish > 0 ) || // if "finished" ...
                    (operation.tAsapStart > 0 && operation.tAsapFinish > 0) || // ... or not "not started"
                    (operation.tActualStart > 0 && operation.tAsapStart && operation.tAsapFinish > 0) ) { // ...or "partly finished..."
                    operations.add( operation ); // ... then it's ok - adding a new operation.
                }

                operation.sNotes = fieldsParsed["Notes"];                
            }
        }
        infile.close();
        return 0;
    }
}
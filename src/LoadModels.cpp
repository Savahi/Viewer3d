#include <stdio.h>
#include "Helpers.hpp"
#include "Models.hpp"

namespace Spider3d {
    static int _codePos, _namePos, _descriptionPos, _notesPos;

    int loadModels( Models& models, const char *cpFile ) {

        FILE *fp;
        int status;

        fp = fopen( cpFile, "rb" );
        if( fp != NULL ) {
            status = parseFileHeader( fp );
            if( status != -1 ) {
                while(1) {
                    status = parseFileLine( operations, fp );
                    if( status == -1 ) {
                        break;
                    }
                }
            }
            fclose(fp);
        }

        return 0;
    }


    static int parseFileHeader( FILE *fp ) {
        int ret_val=0;
        char *cpHeader;

        cpHeader = readLineFromFile(fp);
        if( cpHeader == NULL ) {
            return -1;
        }

        _codePos = getPosByColumnName( cpHeader, "code" );
        if( _codePos == -1 ) {
            ret_val = -1;
        }
        _namePos = getPosByColumnName( cpHeader, "name" );
        if( _namePos == -1 ) {
            ret_val = -1;
        }
        _descriptionPos = getPosByColumnName( cpHeader, "description" );
        if( _descriptionPos == -1 ) {
            ret_val = -1;
        }
        _notesPos = getPosByColumnName( cpHeader, "notes" );
        if( _notesPos == -1 ) {
            ret_val = -1;
        }

        free(cpHeader);
        return ret_val;
    }


    static int parseFileLine( Models& models, FILE *fp ) {
        int status, codeIndex, nameIndex, descriptionIndex, notesIndex;
        char *cpLine; 

        cpLine = readLineFromFile(fp);
        if( cpLine == NULL ) {
            return -1;
        }

        status = getValuesByColumnPos( cpLine, &codeIndex, &nameIndex, &descriptionIndex, &notesIndex );
        if( status == 0 ) {
            Model model;
            parseModel( Model& model, &cpLine[descriptionIndex] );
            model.setCode( &cpLine[codeIndex] );
            model.setName( &cpLine[nameIndex] );
            models.add(model);

            if( parseDates(  &cpLine[codeIndex], &cpLine[nameIndex], &cpLine[descriptionIndex], operation ) == 0 ) {
                operation.mObjectId = &cpLine[iObject];
                operations.add( operation );
            }
        }
        free(cpLine);
        return 0;
    }


    static int parseModel( Model& model, char *cpText ) {
        int iStatus, lenText, facetStart, facetEnd, pointStart, pointEnd, boxStart, boxEnd;
        int iIndex, nBuffered;
        char caBuffer[READ_BUFFER_SIZE];
        float fX, fY, fZ;
        float xNear, yNear, zNear, xFar, yFar, zFar;

        lenText = strlen( cpText );

        facetEnd = 0;
        while(1) {
            iStatus = findTagContent( cpText, "facet", facetEnd, lenText, &facetStart, &facetEnd );
            if( iStatus == 0 ) {
                break;
            }
            Facet facet;

            pointEnd = facetStart;
            while(1) {
                iStatus = findTagContent( cpText, "point", pointEnd, facetEnd, &pointStart, &pointEnd );
                if( iStatus == 0 ) {
                    break;
                }

                for( iIndex = pointStart, nBuffered = 0 ; iIndex <= pointEnd && nBuffered < READ_BUFFER_SIZE ; ) {
                    if( cpText[iIndex] == ',' ) {
                        caBuffer[ nBuffered ] = ' ';
                    } else {
                        caBuffer[ nBuffered ] = cpText[iIndex];
                    }
                    iIndex++;
                    nBuffered++;
                }
                caBuffer[ nBuffered ] = '\x0';
                iStatus = sscanf( caBuffer, " %f %f %f", &fX, &fY, &fZ );
                if( iStatus == 3 ) {
                    Vertex vertex( fX, fY, fZ );
                    facet.add(vertex);
                }
            }
            model.add(facet);
        }

        boxEnd = 0;
        while(1) {
            iStatus = findTagContent( cpText, "box", boxEnd, lenText, &boxStart, &boxEnd );
            if( iStatus == 0 ) {
                break;
            }

            for( iIndex = boxStart, nBuffered = 0 ; iIndex <= boxEnd && nBuffered < READ_BUFFER_SIZE ; ) {
                if( cpText[iIndex] == ',' ) {
                    caBuffer[ nBuffered ] = ' ';
                } else {
                    caBuffer[ nBuffered ] = cpText[iIndex];
                }
                iIndex++;
                nBuffered++;
            }
            caBuffer[ nBuffered ] = '\x0'; 
            iStatus = sscanf( caBuffer, " %f %f %f %f %f %f", &xNear, &yNear, &zNear, &xFar, &yFar, &zFar );
            if( iStatus == 6 ) {
                Facet facet;
                Vertex vertex;
                vertex.setXYZ( xNear, yNear, zNear ); facet.add(vertex);
                vertex.setXYZ( xFar, yNear, zNear ); facet.add(vertex);
                vertex.setXYZ( xFar, yFar, zNear ); facet.add(vertex);
                vertex.setXYZ( xNear, yFar, zNear ); facet.add(vertex);
                model.add(facet);
                vertex.setXYZ( xNear, yNear, zFar ); facet.add(vertex);
                vertex.setXYZ( xFar, yNear, zFar ); facet.add(vertex);
                vertex.setXYZ( xFar, yFar, zFar ); facet.add(vertex);
                vertex.setXYZ( xNear, yFar, zFar ); facet.add(vertex);
                model.add(facet);
                vertex.setXYZ( xNear, yNear, zNear ); facet.add(vertex);
                vertex.setXYZ( xFar, yNear, zNear ); facet.add(vertex);
                vertex.setXYZ( xFar, yNear, zFar ); facet.add(vertex);
                vertex.setXYZ( xNear, yNear, zFar ); facet.add(vertex);
                model.add(facet);
                vertex.setXYZ( xNear, yFar, zNear ); facet.add(vertex);
                vertex.setXYZ( xFar, yFar, zNear ); facet.add(vertex);
                vertex.setXYZ( xFar, yFar, zFar ); facet.add(vertex);
                vertex.setXYZ( xNear, yFar, zFar ); facet.add(vertex);
                model.add(facet);
                vertex.setXYZ( xNear, yNear, zNear ); facet.add(vertex);
                vertex.setXYZ( xNear, yFar, zNear ); facet.add(vertex);
                vertex.setXYZ( xNear, yFar, zFar ); facet.add(vertex);
                vertex.setXYZ( xNear, yNear, zFar ); facet.add(vertex);
                model.add(facet);
                vertex.setXYZ( xFar, yNear, zNear ); facet.add(vertex);
                vertex.setXYZ( xFar, yFar, zNear ); facet.add(vertex);
                vertex.setXYZ( xFar, yFar, zFar ); facet.add(vertex);
                vertex.setXYZ( xFar, yNear, zFar ); facet.add(vertex);
                model.add(facet);
            }
        }
        return 0;
    }

}
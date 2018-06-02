#include <stdio.h>
#include "Helpers.hpp"
#include "Model3d.hpp"

namespace Spider3d {

    int loadModel( Model3d& model, const char *cpFile ) {
        FILE *fp;
        char *cpLine;

        fp = fopen( cpFile, "rb" );
        if( fp != NULL ) {
            while(1) {
                cpLine = readLineFromFile(fp);
                if( cpLine == NULL ) {
                    break;
                }
                parseFileLine( model, fp );
                free( cpLine ); 
            }
            fclose(fp);
        }
        return 0;
    }


    int parseFileLine( Model3d& model, char *cpText ) {

        int iStatus, iObjectStart, iObjectEnd, iFacetStart, iFacetEnd, iPointStart, iPointEnd, iBoxStart, iBoxEnd;
        int iIndex, nBuffered;
        char caBuffer[READ_BUFFER_SIZE];
        float fX, fY, fZ;
        float xNear, yNear, zNear, xFar, yFar, zFar;

        iObjectEnd = 0;
        while(1) {
            iStatus = findTagContent( cpText, "object", iObjectEnd, -1, &iObjectStart, &iObjectEnd );
            if( iStatus == 0 ) {
                break;
            }
            Object object;

            iFacetEnd = iObjectStart;
            while(1) {
                iStatus = findTagContent( cpText, "facet", iFacetEnd, iObjectEnd, &iFacetStart, &iFacetEnd );
                if( iStatus == 0 ) {
                    break;
                }
                Facet3d facet;

                iPointEnd = iFacetStart;
                while(1) {
                    iStatus = findTagContent( cpText, "point", iPointEnd, iFacetEnd, &iPointStart, &iPointEnd );
                    if( iStatus == 0 ) {
                        break;
                    }

                    for( iIndex = iPointStart, nBuffered = 0 ; iIndex <= iPointEnd && nBuffered < READ_BUFFER_SIZE ; ) {
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
                        Vertex3d vertex( fX, fY, fZ );
                        facet.add(vertex);
                    }
                }
                object.add(facet);
            }

            iBoxEnd = iObjectStart;
            while(1) {
                iStatus = findTagContent( cpText, "box", iBoxEnd, iObjectEnd, &iBoxStart, &iBoxEnd );
                if( iStatus == 0 ) {
                    break;
                }

                for( iIndex = iBoxStart, nBuffered = 0 ; iIndex <= iBoxEnd && nBuffered < READ_BUFFER_SIZE ; ) {
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
                    Facet3d facet;
                    Vertex3d vertex;
                    vertex.setXYZ( xNear, yNear, zNear ); facet.add(vertex);
                    vertex.setXYZ( xFar, yNear, zNear ); facet.add(vertex);
                    vertex.setXYZ( xFar, yFar, zNear ); facet.add(vertex);
                    vertex.setXYZ( xNear, yFar, zNear ); facet.add(vertex);
                    object.add(facet);
                    vertex.setXYZ( xNear, yNear, zFar ); facet.add(vertex);
                    vertex.setXYZ( xFar, yNear, zFar ); facet.add(vertex);
                    vertex.setXYZ( xFar, yFar, zFar ); facet.add(vertex);
                    vertex.setXYZ( xNear, yFar, zFar ); facet.add(vertex);
                    object.add(facet);
                    vertex.setXYZ( xNear, yNear, zNear ); facet.add(vertex);
                    vertex.setXYZ( xFar, yNear, zNear ); facet.add(vertex);
                    vertex.setXYZ( xFar, yNear, zFar ); facet.add(vertex);
                    vertex.setXYZ( xNear, yNear, zFar ); facet.add(vertex);
                    object.add(facet);
                    vertex.setXYZ( xNear, yFar, zNear ); facet.add(vertex);
                    vertex.setXYZ( xFar, yFar, zNear ); facet.add(vertex);
                    vertex.setXYZ( xFar, yFar, zFar ); facet.add(vertex);
                    vertex.setXYZ( xNear, yFar, zFar ); facet.add(vertex);
                    object.add(facet);
                    vertex.setXYZ( xNear, yNear, zNear ); facet.add(vertex);
                    vertex.setXYZ( xNear, yFar, zNear ); facet.add(vertex);
                    vertex.setXYZ( xNear, yFar, zFar ); facet.add(vertex);
                    vertex.setXYZ( xNear, yNear, zFar ); facet.add(vertex);
                    object.add(facet);
                    vertex.setXYZ( xFar, yNear, zNear ); facet.add(vertex);
                    vertex.setXYZ( xFar, yFar, zNear ); facet.add(vertex);
                    vertex.setXYZ( xFar, yFar, zFar ); facet.add(vertex);
                    vertex.setXYZ( xFar, yNear, zFar ); facet.add(vertex);
                    object.add(facet);
                }
            }
            model.add(object);
        }
        return 0;
    }

}
#include <GL/glut.h>
#include <string.h>
#include <ctime>

#include "operations.hpp"
#include "optypes.hpp"
#include "models.hpp"
#include "display.hpp"

namespace Spider3d {

	static double _fLeft, _fRight, _fBottom, _fTop, _fW, _fH;

	static double _fMinX, _fMaxX, _fMinY, _fMaxY, _fMinZ, _fMaxZ;

	static void calcMinMax( Model *m );


	void displaySelectedModelInfo( void ) {
		if( _modelSelected == NULL ) {
			return;
		}
		Model *m = _modelSelected;

		calcMinMax( m );

		char caOp[200], caOpType[40], caDTText[80], caDT[41];
		int iOpPair;

		_fLeft = _fModelAreaRight;
		_fRight = _fWindowRight;
		_fBottom = _fWindowBottom;
		_fTop = _fWindowTop;
		_fW = _fRight - _fLeft;
		_fH = _fTop - _fBottom;

		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D( _fWindowLeft, _fWindowRight, _fWindowBottom, _fWindowTop );
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glLoadIdentity();

		iOpPair = 0;
		std::map<time_t,Operation*>::reverse_iterator opPair = m->operations.rbegin();
	    for( ; opPair != m->operations.rend() ; ++opPair, ++iOpPair ) {
	    	
	    	Operation *op = opPair->second;

			if( _tDisplayTime < op->tActualStart ) {
				timetToStr( op->tActualStart, caDT, 40, false );
				sprintf( caDTText, "To start on: %s", caDT );
			} else if ( _tDisplayTime > op->tActualFinish ) {
				timetToStr( op->tActualFinish, caDT, 40, false );
				sprintf( caDTText, "Finished on: %s", caDT );
			} else {
				timetToStr( op->tActualStart, caDT, 40, false );
				int pct = int( ( (_tDisplayTime - op->tActualStart)*100 ) / (op->tActualFinish - op->tActualStart) );  
				sprintf( caDTText, "Started on %s is under way now with %d%% done.", caDT, pct );
			}

			std::string opTypeName;
			if( op->opType != NULL ) {
				strncpy( caOpType, opTypeName.c_str(), 40 );
			} else {
				strcpy( caOpType, "" );
			}

			//sprintf( caOp, "%s%s: %s", op->sName.c_str(), caOpType, caDTText );

			double fUpperMargin = _fH*0.25;
			double fBetweenLines = _fH*0.01;

			glColor3f( 1.0f, 1.0f, 1.0f );
			const char *cpOpName = op->sName.c_str();
			//glRasterPos2f( _fLeft, _fTop - fUpperMargin - (double)(iOpPair*4)*fBetweenLines );
			glRasterPos2f( _fMaxX, _fMaxY - (double)(iOpPair*4)*fBetweenLines );
			for( int i = 0 ; cpOpName[i] != '\x0' ; ++i ) {
			    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, cpOpName[i] );
			}
			// glRasterPos2f( _fLeft, _fTop - fUpperMargin - (double)(iOpPair*4+1)*fBetweenLines );
			glRasterPos2f( _fMaxX, _fMaxY - (double)(iOpPair*4+1)*fBetweenLines );
			for( int i = 0 ; caOpType[i] != '\x0' ; ++i ) {
			    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, caOpType[i] );
			}
			//glRasterPos2f( _fLeft, _fTop - fUpperMargin - (double)(iOpPair*4+2)*fBetweenLines );
			glRasterPos2f( _fMaxX, _fMaxY - (double)(iOpPair*4+2)*fBetweenLines );
			for( int i = 0 ; caDTText[i] != '\x0' ; ++i ) {
			    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, caDTText[i] );
			}
		}

		glPopMatrix();
		glMatrixMode( GL_PROJECTION );
		glPopMatrix();
		glMatrixMode( GL_MODELVIEW );
		//glutSwapBuffers();
	}

	static void calcMinMax( Model *m ) {		
		bool bFirst = true;	    
	    for( std::vector<Facet>::iterator fa = m->mFacets.begin() ; fa != m->mFacets.end() ; ++fa ) {
		    for( std::vector<Vertex>::iterator ve = fa->mVertices.begin() ; ve != fa->mVertices.end() ; ++ve ) {
				double x = (_faDisplayMatrix[0] * ve->mX) + (_faDisplayMatrix[4] * ve->mY) + (_faDisplayMatrix[8] * ve->mZ);
			    double y = (_faDisplayMatrix[1] * ve->mX) + (_faDisplayMatrix[5] * ve->mY) + (_faDisplayMatrix[9] * ve->mZ);
			    double z = (_faDisplayMatrix[2] * ve->mX) + (_faDisplayMatrix[6] * ve->mY) + (_faDisplayMatrix[10] * ve->mZ);
			    if( bFirst ) {
			    	_fMinX = _fMaxX = x; _fMinY = _fMaxY = y; _fMinZ = _fMaxZ = z;
			    	bFirst = false;
			    } else {
			    	if( x < _fMinX ) { _fMinX = x; }
			    	if( x > _fMaxX ) { _fMaxX = x; }
			    	if( y < _fMinY ) { _fMinY = y; }
			    	if( y > _fMaxY ) { _fMaxY = y; }
			    	if( z < _fMinZ ) { _fMinZ = z; }
			    	if( z > _fMaxZ ) { _fMaxZ = z; }
			    }
			}
		}
	}
}
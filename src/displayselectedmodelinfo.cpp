#include <string.h>
#include <ctime>
#include <GL/glut.h>
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

		char caOpDates[80], caOpDate1[41], caOpDate2[41];
		int iOpPair;

		_fLeft = _fModelAreaRight;
		_fRight = _fWindowRight;
		_fBottom = _fWindowBottom;
		_fTop = _fWindowTop;
		_fW = _fRight - _fLeft;
		_fH = _fTop - _fBottom;


		double fLineHeight = 12.0 * _fWindowHeight / (double)_iWindowHeight;
		double fBetweenLines = fLineHeight*1.25;

/*
		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D( _fWindowLeft, _fWindowRight, _fWindowBottom, _fWindowTop );
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glLoadIdentity();
*/
		iOpPair = 0;
		std::map<time_t,Operation*>::reverse_iterator opPair = m->operations.rbegin();
	    for( ; opPair != m->operations.rend() ; ++opPair, ++iOpPair ) {
	    	
	    	Operation *op = opPair->second;

			if( _tDisplayTime < op->tActualStart ) {
				timetToStr( op->tActualStart, caOpDate1, 40, false, false );
				timetToStr( op->tActualFinish, caOpDate2, 40, false, false );
				sprintf( caOpDates, "[0%%]: %s - %s", caOpDate1, caOpDate1 );
			} else if ( _tDisplayTime > op->tActualFinish ) {
				timetToStr( op->tActualStart, caOpDate1, 40, false, false );
				timetToStr( op->tActualFinish, caOpDate2, 40, false, false );
				sprintf( caOpDates, "[100%%]: %s - %s", caOpDate1, caOpDate2 );
			} else {
				timetToStr( op->tActualStart, caOpDate1, 40, false, false );
				timetToStr( op->tActualFinish, caOpDate2, 40, false, false );
				int iPct = int( ( (_tDisplayTime - op->tActualStart)*100 ) / (op->tActualFinish - op->tActualStart) );  
				sprintf( caOpDates, "[%d%%] %s - %s", iPct, caOpDate1, caOpDate2 );
			}

			//glColor3f( 1.0f, 1.0f, 1.0f );
			glColor3f( op->opType->fR, op->opType->fG, op->opType->fB );

			const char *cpOpName = op->sName.c_str();
			//glRasterPos2f( _fLeft, _fTop - fUpperMargin - (double)(iOpPair*4)*fBetweenLines );
			glRasterPos3f( _fMaxX, _fMaxY - (double)(iOpPair*4)*fBetweenLines, _fMaxZ );
			for( int i = 0 ; cpOpName[i] != '\x0' ; ++i ) {
			    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, cpOpName[i] );
			}
			// glRasterPos2f( _fLeft, _fTop - fUpperMargin - (double)(iOpPair*4+1)*fBetweenLines );
			const char *cpOpType = op->opType->sName.c_str();
			glRasterPos3f( _fMaxX, _fMaxY - (double)(iOpPair*4+1)*fBetweenLines, _fMaxZ );
			for( int i = 0 ; cpOpType[i] != '\x0' ; ++i ) {
			    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, cpOpType[i] );
			}
			//glRasterPos2f( _fLeft, _fTop - fUpperMargin - (double)(iOpPair*4+2)*fBetweenLines );
			glRasterPos3f( _fMaxX, _fMaxY - (double)(iOpPair*4+2)*fBetweenLines, _fMaxZ );
			for( int i = 0 ; caOpDates[i] != '\x0' ; ++i ) {
			    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, caOpDates[i] );
			}
		}
/*
		glPopMatrix();
		glMatrixMode( GL_PROJECTION );
		glPopMatrix();
		glMatrixMode( GL_MODELVIEW );
		//glutSwapBuffers();
*/
	}

	static void calcMinMax( Model *m ) {		
		bool bFirst = true;	    
	    for( std::vector<Facet>::iterator fa = m->mFacets.begin() ; fa != m->mFacets.end() ; ++fa ) {
		    for( std::vector<Vertex>::iterator ve = fa->mVertices.begin() ; ve != fa->mVertices.end() ; ++ve ) {
				double x = (_faDisplayMVMatrix[0] * ve->mX) + (_faDisplayMVMatrix[4] * ve->mY) + (_faDisplayMVMatrix[8] * ve->mZ);
			    double y = (_faDisplayMVMatrix[1] * ve->mX) + (_faDisplayMVMatrix[5] * ve->mY) + (_faDisplayMVMatrix[9] * ve->mZ);
			    double z = (_faDisplayMVMatrix[2] * ve->mX) + (_faDisplayMVMatrix[6] * ve->mY) + (_faDisplayMVMatrix[10] * ve->mZ);
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
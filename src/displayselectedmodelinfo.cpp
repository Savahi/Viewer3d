#include <GL/glut.h>
#include <string.h>
#include <ctime>

#include "operations.hpp"
#include "optypes.hpp"
#include "models.hpp"
#include "display.hpp"

namespace Spider3d {

	static double _fLeft, _fRight, _fBottom, _fTop, _fW, _fH;


	void displaySelectedModelInfo( Model *m, time_t _tDisplayTime ) {
		if( m == NULL ) {
			return;
		}

		char caOp[200], caOpType[40], caDTText[80], caDT[41];
		int iOpPair;

		_fLeft = _fModelAreaLeft;
		_fRight = _fModelAreaRight;
		_fBottom = _fModelAreaTop;
		_fTop = _fWindowTop;
		_fW = _fRight - _fLeft;
		_fH = _fTop - _fBottom;


		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D( 0, _iWindowWidth, 0, _iWindowHeight );
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

			sprintf( caOp, "%s%s: %s", op->sName.c_str(), caOpType, caDTText );

			glColor3f( 1.0f, 1.0f, 1.0f );
			glRasterPos2i( 0, _iWindowHeight - 20 - iOpPair*12 );
			for( int i = 0 ; caOp[i] != '\x0' ; ++i ) {
			    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, caOp[i] );
			}
		}

		glPopMatrix();
		glMatrixMode( GL_PROJECTION );
		glPopMatrix();
		glMatrixMode( GL_MODELVIEW );
		//glutSwapBuffers();
	}
}
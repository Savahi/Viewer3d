
#define GL_GLEX_tPROTOTYPES
#include <GL/glut.h>
#include <string.h>
#include <ctime>

#include "operations.hpp"
#include "optypes.hpp"
#include "models.hpp"
#include "display.hpp"

namespace Spider3d {

	static void drawText( time_t time, const char *cpText, int yPct=50, int align=0 );
	static void drawLine( double fX1, double fY1, double fX2, double fY2, double fR=1.0, double fG=1.0, double fB=1.0, double fO=1.0 );

	static double timeToX( time_t time );

	static time_t _tScaleMin, _tScaleMax;

	static double _fScaleAreaLeft, _fScaleAreaRight, _fScaleAreaBottom, _fScaleAreaTop, _fScaleAreaW, _fScaleAreaH;
	static double _fScaleLeft, _fScaleRight, _fScaleW, _fScaleY, _fScaleExtra;

	void displayTimeScale( void ) 
	{
		char caBuffer[200];
		int iOpPair;

		_fScaleAreaLeft = _fWindowLeft;
		_fScaleAreaRight = _fModelAreaRight;
		_fScaleAreaBottom = _fModelAreaTop;
		_fScaleAreaTop = _fWindowTop;
		_fScaleAreaW = _fScaleAreaRight - _fScaleAreaLeft;
		_fScaleAreaH = _fScaleAreaTop - _fScaleAreaBottom;

		_fScaleLeft = _fScaleAreaLeft + _fScaleAreaW*0.1;
		_fScaleRight = _fScaleAreaLeft + _fScaleAreaW*0.9;
		_fScaleW = _fScaleRight - _fScaleLeft;
		_fScaleExtra = _fScaleW*0.05;

		_fScaleY = _fScaleAreaBottom + _fScaleAreaH/2.0;

		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D( _fWindowLeft, _fWindowRight, _fWindowBottom, _fWindowTop );
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glLoadIdentity();

		_tScaleMin = _tDisplayTimeMin;
		_tScaleMax = _tDisplayTimeMax;
		
		// Drawing the scale
		drawLine( _fScaleLeft - _fScaleExtra, _fScaleY, _fScaleRight + _fScaleExtra, _fScaleY );
		double fScaleMark = _fScaleAreaH/20.0;
		drawLine( _fScaleLeft, _fScaleY-fScaleMark, _fScaleLeft, _fScaleY+fScaleMark ); // _tDisplayTimeMin
		drawLine( _fScaleRight, _fScaleY-fScaleMark, _fScaleRight, _fScaleY+fScaleMark ); // _tDisplayTimeMax
		double xDisplayTime = timeToX( _tDisplayTime ); // _tDisplayTime
		drawLine( xDisplayTime, _fScaleY - fScaleMark*2.0, xDisplayTime, _fScaleY+fScaleMark*2.0, 1.0, 0.0, 0.0, 0.5 ); // _tDisplayTime

		timetToStr( _tScaleMin, caBuffer, 40, false, false );
		drawText( _tDisplayTimeMin, caBuffer, 60 );
		timetToStr( _tScaleMax, caBuffer, 40, false, false );
		drawText( _tDisplayTimeMax, caBuffer, 60 );

		timetToStr( _tDisplayTime, caBuffer, 60, false, false );
		drawText( _tDisplayTime, caBuffer, 5 );
		//drawText( _tDisplayTimeActual, "" );

		glPopMatrix();
		glMatrixMode( GL_PROJECTION );
		glPopMatrix();
		glMatrixMode( GL_MODELVIEW );
	}

	static void drawText( time_t time, const char *cpText, int yPct, int align ) {
		double x = timeToX( time );
		glColor3f( 1.0f, 1.0f, 1.0f );
		double y = _fScaleAreaBottom + _fScaleAreaH * (double)yPct / 100.0;
		glRasterPos2f( x, y );

		for( int i = 0 ; cpText[i] != '\x0' ; ++i ) {
		    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, cpText[i] );
		}
	}

	static void drawLine( double fX1, double fY1, double fX2, double fY2, double fR, double fG, double fB, double fO  ) {
		glColor4f( fR, fG, fB, fO );
		glBegin(GL_LINES);
		glVertex2f( fX1,fY1 );
		glVertex2f( fX2,fY2 );
		glEnd();
	}

	static double timeToX( time_t time ) {
		double x = _fScaleLeft + ( (double)(time - _tScaleMin) * _fScaleW) / (double)(_tScaleMax - _tScaleMin);
		if( x < _fScaleLeft ) {
			x = _fScaleLeft - _fScaleExtra;
		} else if( x > _fScaleRight ) {
			x = _fScaleRight + _fScaleExtra;
		}
		return x;
	}	


	bool catchMouseInTimeScale( int button, int state, int x, int y ) {

		if( !(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) ) {
			return false;
		}

		double fY = _fWindowBottom + ( 1.0 - (double)y/(double)_iWindowHeight ) * _fWindowHeight;
		if( fY < _fScaleAreaBottom ) {
			return false;
		}
		double fX = _fWindowLeft + ( (double)x / (double)_iWindowWidth ) * _fWindowWidth;
		if( fX > _fScaleAreaRight ) {
			return false;
		}

		time_t timeToSet = _tScaleMin + (fX - _fScaleLeft) * (_tScaleMax - _tScaleMin) / _fScaleW; 
		if( timeToSet < _tDisplayTimeMin ) {
			if( _tDisplayTimeActual < _tDisplayTimeMin ) {
				timeToSet = _tDisplayTimeActual;
			} else {
				timeToSet = _tDisplayTimeMin;				
			}
		} else if( timeToSet > _tDisplayTimeMax ) {
			if( _tDisplayTimeActual > _tDisplayTimeMax ) {
				timeToSet = _tDisplayTimeActual;
			} else {
				timeToSet = _tDisplayTimeMax;
			}
		}
		_tDisplayTime = timeToSet;
	}

} // The end of the namespace
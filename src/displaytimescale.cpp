
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

	double timeToX( time_t time );

	static time_t _tScaleMin, _tScaleMax;

	static double _fLeft, _fRight, _fBottom, _fTop, _fW, _fH;
	static double _fScaleLeft, _fScaleRight, _fScaleW, _fScaleY, _fScaleExtra;

	void displayTimeScale( time_t tDisplayTime, time_t tStartTime, time_t tFinishTime, time_t tTimeNow ) 
	{
		char caBuffer[200];
		int iOpPair;

		_fLeft = _fModelAreaLeft;
		_fRight = _fModelAreaRight;
		_fBottom = _fModelAreaTop;
		_fTop = _fWindowTop;
		_fW = _fRight - _fLeft;
		_fH = _fTop - _fBottom;

		_fScaleLeft = _fLeft + _fW*0.1;
		_fScaleRight = _fLeft + _fW*0.9;
		_fScaleW = _fScaleRight - _fScaleLeft;
		_fScaleExtra = _fScaleW*0.05;

		_fScaleY = _fBottom + _fH/2.0;

		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D( _fModelAreaLeft, _fWindowRight, _fModelAreaBottom, _fWindowTop );
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glLoadIdentity();

		_tScaleMin = tStartTime;
		_tScaleMax = tFinishTime;
		
		// Drawing the scale
		drawLine( _fScaleLeft - _fScaleExtra, _fScaleY, _fScaleRight + _fScaleExtra, _fScaleY );
		double fScaleMark = _fH/20.0;
		drawLine( _fScaleLeft, _fScaleY-fScaleMark, _fScaleLeft, _fScaleY+fScaleMark ); // tStartTime
		drawLine( _fScaleRight, _fScaleY-fScaleMark, _fScaleRight, _fScaleY+fScaleMark ); // tFinishTime
		double xDisplayTime = timeToX( tDisplayTime ); // tDisplayTime
		drawLine( xDisplayTime, _fScaleY - fScaleMark*2.0, xDisplayTime, _fScaleY+fScaleMark*2.0, 1.0, 0.0, 0.0, 0.5 ); // tDisplayTime

		timetToStr( _tScaleMin, caBuffer, 40, false, false );
		drawText( tStartTime, caBuffer, 60 );
		timetToStr( _tScaleMax, caBuffer, 40, false, false );
		drawText( tFinishTime, caBuffer, 60 );

		timetToStr( tDisplayTime, caBuffer, 60, false, false );
		drawText( tDisplayTime, caBuffer, 5 );
		//drawText( tTimeNow, "" );

		glPopMatrix();
		glMatrixMode( GL_PROJECTION );
		glPopMatrix();
		glMatrixMode( GL_MODELVIEW );
	}

	static void drawText( time_t time, const char *cpText, int yPct, int align ) {
		double x = timeToX( time );
		glColor3f( 1.0f, 1.0f, 1.0f );
		double y = _fBottom + _fH * (double)yPct / 100.0;
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

	double timeToX( time_t time ) {
		double x = _fScaleLeft + ( (double)(time - _tScaleMin) * _fScaleW) / (double)(_tScaleMax - _tScaleMin);
		if( x < _fScaleLeft ) {
			x = _fScaleLeft - _fScaleExtra;
		} else if( x > _fScaleRight ) {
			x = _fScaleRight + _fScaleExtra;
		}
		return x;
	}	

}
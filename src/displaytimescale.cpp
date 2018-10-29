#include "stdafx.h"
#include <GL/glut.h>

namespace Spider3d {

	static void drawText( time_t time, const char *cpText, int yPct=50, double fR=0.7, double fG=0.7, double fB=0.7 );
	static void drawLine( double fX1, double fY1, double fX2, double fY2, double fR=1.0, double fG=1.0, double fB=1.0, double fO=1.0 );

	static double timeToX( time_t time );

	static time_t _tScaleMin, _tScaleMax;

	static double _fScaleAreaLeft, _fScaleAreaRight, _fScaleAreaBottom, _fScaleAreaTop, _fScaleAreaW, _fScaleAreaH;
	static double _fScaleLeft, _fScaleRight, _fScaleW, _fScaleY, _fScaleExtra;

	static bool _bScaleActive = false;

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

		//glMatrixMode( GL_PROJECTION );
		//glPushMatrix();
		//glLoadIdentity();
		//gluOrtho2D( _fWindowLeft, _fWindowRight, _fWindowBottom, _fWindowTop );
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glLoadIdentity();

		_tScaleMin = _tDisplayTimeMin;
		_tScaleMax = _tDisplayTimeMax;
		
		// Drawing the scale
		double fR, fG, fB;
		if( !_bScaleActive ) { fR = 0.5; fG=0.5; fB = 0.5; }
		else { fR = 0.9; fG=0.9; fB = 0.9; }
		drawLine( _fScaleLeft - _fScaleExtra, _fScaleY, _fScaleRight + _fScaleExtra, _fScaleY, fR, fB, fG );
		double fScaleMark = _fScaleAreaH/20.0;
		drawLine( _fScaleLeft, _fScaleY-fScaleMark, _fScaleLeft, _fScaleY+fScaleMark, fR, fB, fG ); // _tDisplayTimeMin
		drawLine( _fScaleRight, _fScaleY-fScaleMark, _fScaleRight, _fScaleY+fScaleMark, fR, fB, fG ); // _tDisplayTimeMax
		double xDisplayTime = timeToX( _tDisplayTime ); // _tDisplayTime
		drawLine( xDisplayTime, _fScaleY - fScaleMark*2.0, xDisplayTime, _fScaleY+fScaleMark*2.0, 1.0, 0.0, 0.0, 0.75 ); // _tDisplayTime

		timetToStr( _tScaleMin, caBuffer, 40, false, false );
		drawText( _tDisplayTimeMin, caBuffer, 60, fR, fB, fG );
		timetToStr( _tScaleMax, caBuffer, 40, false, false );
		drawText( _tDisplayTimeMax, caBuffer, 60, fR, fB, fG );

		timetToStr( _tDisplayTime, caBuffer, 60, false, false );
		drawText( _tDisplayTime, caBuffer, 5, fR, fB, fG );
		//drawText( _tDisplayTimeActual, "" );

		glPopMatrix();
		//glMatrixMode( GL_PROJECTION );
		//glPopMatrix();
	}

	static void drawText( time_t time, const char *cpText, int yPct, double fR, double fG, double fB ) {
		double x = timeToX( time );
		glColor3f( fR, fG, fB );
		double y = _fScaleAreaBottom + _fScaleAreaH * (double)yPct / 100.0;
		glRasterPos3f( x, y, 0 );

		for( int i = 0 ; cpText[i] != '\x0' ; ++i ) {
		    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, cpText[i] );
		}
	}

	static void drawLine( double fX1, double fY1, double fX2, double fY2, double fR, double fG, double fB, double fO  ) {
		glColor4f( fR, fG, fB, fO );
		glBegin(GL_LINES);
		glVertex3f( fX1, fY1, 0 );
		glVertex3f( fX2, fY2, 0  );
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


	bool catchMouseInTimeScale( int button, int state, int x, int y, double fX, double fY ) {

		if( fY < _fScaleAreaBottom || fX > _fScaleAreaRight ) {
			if( _bScaleActive ) {
				_bScaleActive = false;
				return true; // Redraw.
			}
			return false; // Do not redraw.
		}

		if( state == MOUSE_MOVES_NOT_PRESSED ) { // Passive motion over the scale
			if( _bScaleActive == false ) { // The scale is not active yet.
				_bScaleActive = true;
				return true;
			}
			return false;
		}
		if( button == 3 or button == 4 ) {
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

/*
			if( _tDisplayTime < _tDisplayTimeMin ) {
				_tDisplayTime = _tDisplayTimeMin;				
			} else { 
				if( _tDisplayTime == _tDisplayTimeMax ) {
					if( _tDisplayTime < _tDisplayTimeActual ) { 
						_tDisplayTime = _tDisplayTimeActual; 
					}
				} else if( _tDisplayTime < _tDisplayTimeMax ) {
					printf( "BEFORE: time=%ld, timeMax=%ld, timeActual=%ld\n", _tDisplayTime, _tDisplayTimeMax, _tDisplayTimeActual );					
					_tDisplayTime += (_tDisplayTimeMax - _tDisplayTimeMin)/_iDateScaleRate;	
					printf( "AFTER: time=%ld, timeMax=%ld, timeActual=%ld\n", _tDisplayTime, _tDisplayTimeMax, _tDisplayTimeActual );					
					if( _tDisplayTime > _tDisplayTimeMax ) { 
						_tDisplayTime = _tDisplayTimeMax; 
						printf( "AND AFTER: time=%ld, timeMax=%ld, timeActual=%ld\n", _tDisplayTime, _tDisplayTimeMax, _tDisplayTimeActual );						
					}
				}
			}
			if ( _tDisplayTime > _tDisplayTimeMax ) { 
				_tDisplayTime = _tDisplayTimeMax;
			} else {
				if( _tDisplayTime == _tDisplayTimeMin ) {
					if( _tDisplayTime > _tDisplayTimeActual ) { 
						_tDisplayTime = _tDisplayTimeActual; 
					}
				} else if( _tDisplayTime > _tDisplayTimeMin ) {
					_tDisplayTime -= (_tDisplayTimeMax - _tDisplayTimeMin)/_iDateScaleRate;
					if( _tDisplayTime < _tDisplayTimeMin ) { 
						_tDisplayTime = _tDisplayTimeMin; 
					} 
				} 
			}
*/
} // The end of the namespace
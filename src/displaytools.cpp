#include "stdafx.h"
#include <GL/glut.h>

namespace Spider3d {

	static double _fLeft, _fRight, _fBottom, _fTop, _fW, _fH;

	static double _fMinX, _fMaxX, _fMinY, _fMaxY, _fMinZ, _fMaxZ;

	static double _fButtonW, _fButtonH;
	static double _fGridLeft, _fGridTop, _fGridW, _fGridH;

	static int _nButtons=4;
	static double _faButtonXs[10], _faButtonYs[10];
	static char _caButtonCodes[10] = { 'u', 'd', 'l', 'r' };
	static int _iActiveButton = -1;

	static void assignButtonCoords( char cButton, double& left, double& bottom );
	static void drawButton( int iButton, bool bActive=false );

	static void drawRect( double fX1, double fY1, double fX2, double fY2, double fR=0.5, double fG=0.5, double fB=0.5 );
	static void drawTriangle( double fX1, double fY1, double fX2, double fY2, double fX3, double fY3, 
		double fR=0.25, double fG=0.25, double fB=0.25 );

	static void drawText( const char *cpText, double x, double y );


	void displayTools( void ) {
		_fLeft = _fModelAreaRight;
		_fRight = _fWindowRight;
		_fBottom = _fWindowBottom;
		_fTop = _fWindowTop;
		_fW = _fRight - _fLeft;
		_fH = _fTop - _fBottom;


		_fButtonH = (12.0 / (double)_iWindowHeight) * _fWindowHeight;
		_fButtonW = (12.0 / (double)_iWindowWidth) * _fWindowWidth;
		_fGridW = _fButtonW*1.5;
		_fGridH = _fButtonH*1.5;
		_fGridLeft = _fW * 0.4;
		_fGridTop = _fH * 0.2;

		for( int i = 0 ; i < _nButtons ; i++ ) {
			assignButtonCoords( _caButtonCodes[i], _faButtonXs[i], _faButtonYs[i] );
			drawButton(i, (_iActiveButton == i) );
			if( _caButtonCodes[i] == 'd' || _caButtonCodes[i] == 'r' ) {
				char caBuf[20];
				if( _caButtonCodes[i] == 'd' ) { 
					sprintf( caBuf, "%+d grad.", _iModelsRotateX );
				} else {
					sprintf( caBuf, "%+d grad", _iModelsRotateY );
				}
				drawText( caBuf,  _faButtonXs[i] + _fGridW, _faButtonYs[i] );
			}
		}
	}

	static void assignButtonCoords( char cButton, double& fLeft, double& fBottom ) {
		int iCol, iRow;
		if( cButton == 'u' ) { iCol = 0; iRow = 0; } 
		else if( cButton == 'd' ) { iCol = 1; iRow = 0; }
		else if( cButton == 'l' ) { iCol = 0; iRow = 1; }
		else if( cButton == 'r' ) { iCol = 1; iRow = 1; }

		fLeft = _fLeft + _fGridLeft + _fGridW*(double)iCol + (_fGridW - _fButtonW)/2.0;
		fBottom = _fTop - _fGridTop - _fGridH*(double)(iRow+1) + (_fGridH - _fButtonH)/2.0;
	}


	static void drawButton( int iButton, bool bActive ) {
		double x1 = _faButtonXs[iButton];
		double y1 = _faButtonYs[iButton];
		double x2 = _faButtonXs[iButton] + _fButtonW;
		double y2 = _faButtonYs[iButton] + _fButtonH;
		if( !bActive ) {
			drawRect( x1,y1,x2,y2 );
		} else {
			drawRect( x1,y1,x2,y2, 0.9, 0.9, 0.9 );			
		}
		double dx = (x2-x1)*0.2;
		double dy = (y2-y1)*0.2;
		x1 += dx;
		x2 -= dx;
		y1 += dy;
		y2 -= dy;
		if( _caButtonCodes[iButton] == 'u' ) { drawTriangle( x1, y2, x2, y2, x1+(x2-x1)/2.0, y1 ); } 
		else if( _caButtonCodes[iButton] == 'd' ) { drawTriangle( x1, y1, x2, y1, x1+(x2-x1)/2.0, y2 ); }
		else if( _caButtonCodes[iButton] == 'l' ) { drawTriangle( x1, y1+(y2-y1)/2.0, x2, y2, x2, y1 ); }
		else if( _caButtonCodes[iButton] == 'r' ) { drawTriangle( x2, y1+(y2-y1)/2.0, x1, y2, x1, y1 ); }
	}


	bool catchMouseInTools( int button, int state, int x, int y, double fX, double fY ) {

		if( button == GLUT_LEFT_BUTTON ) {
			if( state == GLUT_UP ) {
				if( _iActiveButton >= 0 ) {
					_iActiveButton=-1;
					return true;
				}
			}

			if( state == GLUT_DOWN ) {
				for( int i = 0 ; i < _nButtons ; i++ ) {
					if( (fX < _faButtonXs[i]) || (fX > _faButtonXs[i]+_fButtonW) || (fY < _faButtonYs[i]) || (fY > _faButtonYs[i]+_fButtonH) ) {
						continue;
					}
					if( _caButtonCodes[i] == 'u' ) { _iModelsRotateX += 1; } 
					else if( _caButtonCodes[i] == 'd' ) { _iModelsRotateX -= 1; }
					else if( _caButtonCodes[i] == 'l' ) { _iModelsRotateY -= 1; }
					else if( _caButtonCodes[i] == 'r' ) { _iModelsRotateY += 1; }
					_iActiveButton = i;
					return true;
				}
			}
		}
		return false;
	}

	static void drawRect( double fX1, double fY1, double fX2, double fY2, double fR, double fG, double fB ) {
		glColor3f( fR, fG, fB );
		glBegin(GL_QUADS);
		glVertex3f( fX1,fY1,0 );
		glVertex3f( fX1,fY2,0 );
		glVertex3f( fX2,fY2,0 );
		glVertex3f( fX2,fY1,0 );
		glEnd();
	}

	static void drawTriangle( double fX1, double fY1, double fX2, double fY2, double fX3, double fY3, 
		double fR, double fG, double fB ) 
	{
		glColor3f( fR, fG, fB );
		glBegin( GL_TRIANGLES );
		glVertex3f( fX1, fY1,0.01 );
		glVertex3f( fX2, fY2,0.01 );
		glVertex3f( fX3, fY3,0.01 );
		glEnd();
	}

	static void drawText( const char *cpText, double x, double y ) {
		glColor3f( 1.0f, 1.0f, 1.0f );
		glRasterPos3f( x, y, 0 );
		for( int i = 0 ; cpText[i] != '\x0' ; ++i ) {
		    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, cpText[i] );
		}
	}

}
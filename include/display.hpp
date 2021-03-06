#pragma once
#include "stdafx.h"
#include <GL/glut.h>

namespace Spider3d {

	#define DISPLAY_AREA_RIGHT_PANE 0.75
	#define DISPLAY_AREA_TOP_PANE 0.2

	#define MOUSE_MOVES_PRESSED -100
	#define MOUSE_MOVES_NOT_PRESSED -101

	extern GLdouble _faDisplayMVMatrix[];

	extern GLsizei _iWindowWidth, _iWindowHeight;

	extern double _fWindowLeft, _fWindowRight, _fWindowBottom, _fWindowTop, _fWindowWidth, _fWindowHeight;
	extern double _fModelAreaLeft, _fModelAreaRight, _fModelAreaBottom, _fModelAreaTop;

	extern double _fModelsMinX, _fModelsMaxX, _fModelsMinY, _fModelsMaxY, _fModelsMinZ, _fModelsMaxZ;
	extern double _fModelsW, _fModelsL, _fModelsH;

	extern int _iModelsRotateX, _iModelsRotateY;

	extern Model *_modelSelected;

	extern time_t _tDisplayTimeActual;
	extern time_t _tDisplayTime;
	extern time_t _tDisplayTimeMin, _tDisplayTimeMax;;

	void displayFacet( Facet& facet, Model& model, int iOrder, double progress=0, 
		float fR=0.8, float fB=0.8, float fG=0.8, bool selected=false, bool touched=false );

	void displaySelectedModelInfo( void );

	void displayTimeScale( void );
	bool catchMouseInTimeScale( int button, int state, int x, int y, double fX, double fY );

	void displayTools( void );
	bool catchMouseInTools( int button, int state, int x, int y, double fX, double fY );

	void displayAxis( void );

	extern bool _bDisplayAxisActive;
}
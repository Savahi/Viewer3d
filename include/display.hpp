#pragma once

namespace Spider3d {

	#define DISPLAY_AREA_RIGHT_PANE 0.75
	#define DISPLAY_AREA_TOP_PANE 0.2

	extern GLsizei _iWindowWidth, _iWindowHeight;

	extern double _fWindowLeft, _fWindowRight, _fWindowBottom, _fWindowTop, _fWindowWidth, _fWindowHeight;
	extern double _fModelAreaLeft, _fModelAreaRight, _fModelAreaBottom, _fModelAreaTop;

	extern time_t _tDisplayTimeActual;
	extern time_t _tDisplayTime;
	extern time_t _tDisplayTimeMin, _tDisplayTimeMax;;

	void displayFacet( Facet& facet, Model& model, int iOrder, double progress=0, 
		float fR=0.8, float fB=0.8, float fG=0.8, bool selected=false );

	void displaySelectedModelInfo( Model *m, time_t _tDisplayTime );

	void displayTimeScale( void );
	bool catchMouseInTimeScale( int button, int state, int x, int y );
}
#pragma once

namespace Spider3d {

	#define DISPLAY_AREA_RIGHT_PANE 0.75
	#define DISPLAY_AREA_TOP_PANE 0.2

	extern GLsizei _iWindowWidth, _iWindowHeight;

	extern double _fWindowRight, _fWindowTop;
	extern double _fModelAreaLeft, _fModelAreaRight, _fModelAreaBottom, _fModelAreaTop;

	void displayFacet( Facet& facet, Model& model, int iOrder, double progress=0, 
		float fR=0.8, float fB=0.8, float fG=0.8, bool selected=false );

	void displaySelectedModelInfo( Model *m, time_t _tDisplayTime );

	void displayTimeScale( time_t _tDisplayTime, time_t _tStartTime, time_t _tFinishTime, time_t tTimeNow );

}
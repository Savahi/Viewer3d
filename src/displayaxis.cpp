#include "display.hpp"
	
namespace Spider3d {

	void displayAxis( void ) {
		float cx = _fModelsMinX - _fModelsW*0.25;
		float cy = _fModelsMinY - _fModelsH*0.05;
		float cz = _fModelsMinZ - _fModelsL*0.25;
		float xAxisEnd = _fModelsMaxX*1.5;
		float yAxisEnd = _fModelsMaxY*1.5;
		float zAxisEnd = _fModelsMaxZ*1.5;

		double fR, fG, fB;
		double fGridR, fGridG, fGridB;
		if( _bDisplayAxisActive ) {
			fR=0.75; fG=0.75; fB=0.75;
			fGridR=0.25; fGridG=0.25; fGridB=0.25;
		} else {
			fR=0.25; fG=0.25; fB=0.25;
			fGridR=0.05; fGridG=0.05; fGridB=0.05;
		}

		glBegin(GL_LINES);
		glColor3f( fR, fG, fB );
		glVertex3f( cx, cy, cz );
		glVertex3f( cx, yAxisEnd, cz );

		glVertex3f( cx, cy, cz );
		glVertex3f( xAxisEnd, cy, cz );

		glVertex3f( cx, cy, cz );
		glVertex3f( cx, cy, xAxisEnd );

		glColor3f( fGridR, fGridG, fGridB );
		for( int i = 0 ; i < 6 ; i++ ) {
			glVertex3f( cx, cy, cz + _fModelsL * (double)i*0.25 );
			glVertex3f( xAxisEnd, cy, cz + _fModelsL * (double)i*0.25 );
		}		

		for( int i = 0 ; i < 6 ; i++ ) {
			glVertex3f( cx + _fModelsW * (double)i*0.25, cy, cz );
			glVertex3f( cx + _fModelsW * (double)i*0.25, cy, zAxisEnd );
		}		
		glEnd();
	}
}
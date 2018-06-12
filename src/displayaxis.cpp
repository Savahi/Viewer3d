#include "display.hpp"
	
namespace Spider3d {

	void displayAxis( void ) {
		float cx = _fModelsMinX - _fModelsW*0.25;
		float cy = _fModelsMinY - _fModelsH*0.05;
		float cz = _fModelsMinZ - _fModelsL*0.25;
		float xAxisEnd = _fModelsMaxX*1.5;
		float yAxisEnd = _fModelsMaxY*1.5;
		float zAxisEnd = _fModelsMaxZ*1.5;
		glBegin(GL_LINES);
		glColor3f( 0.25, 0.25, 0.25 );
		glVertex3f( cx, cy, cz );
		glVertex3f( cx, yAxisEnd, cz );

		glVertex3f( cx, cy, cz );
		glVertex3f( xAxisEnd, cy, cz );

		glVertex3f( cx, cy, cz );
		glVertex3f( cx, cy, xAxisEnd );

		glColor3f( 0.05, 0.05, 0.05 );
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
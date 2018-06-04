#include <vector>
#include <iostream>
#include <stdio.h> 
#include <math.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>

#include "Models.hpp"

namespace Spider3d {

	static void displayInitializer( void );

	static void displayFunc();
	static void displayAxis( void );
	static void displayFacet( Facet& facet, float progress=100, bool selected=false );

	static void displayKeys( int key, int x, int y );
	static void displayMouse ( int button, int state, int x, int y );
	static void displayReshape( GLsizei width, GLsizei height );

	static GLsizei _windowWidth, _windowHeight;

	static float f_ModelW, f_ModelL, f_ModelH, f_ModelMinX, f_ModelMaxX, f_ModelMinY, f_ModelMaxY, f_ModelMinZ, f_ModelMaxZ;
	static int i_ModelRotateX, i_ModelRotateY;
	static Model *o_Model;
	static time_t t_TimeNow;
	static time_t t_ModelTime;
	static time_t t_ModelStart, t_ModelFinish; 
	static Operation *_operationSelected=NULL;

	static int i_DateScaleRate = 50;

	static void displayInitializer( void ) {

	  	i_ModelRotateX = 15;
	  	i_ModelRotateY = 15;
	  	t_TimeNow = t_ModelTime = time(0);
	  	t_ModelStart = t_ModelFinish = -1.0;
		
		bool bFirst = true;	
		for( std::vector<Operation>::iterator  op = o_Model->mOperations.begin() ; op != o_Model->mOperations.end() ; ++op ) {
			if( t_ModelStart < 0.0 ) { 
				t_ModelStart = op->tActualStart;
			} else if( op->tActualStart < t_ModelStart ) {
				t_ModelStart = op->tActualStart;
			} 
			if( t_ModelFinish < 0.0 ) {
				t_ModelFinish = op->tActualFinish;			
			} else if ( op->tActualFinish > t_ModelFinish ) {
				t_ModelFinish = op->tActualFinish;
			}
		    for( std::vector<Object>::iterator ob = (*op).mObjects.begin() ; ob != (*op).mObjects.end() ; ++ob ) {
			    for( std::vector<Facet>::iterator fa = (*ob).mFacets.begin() ; fa != (*ob).mFacets.end() ; ++fa ) {	
				    for( std::vector<Vertex>::iterator ve = (*fa).mVertices.begin() ; ve != (*fa).mVertices.end() ; ++ve ) {
				    	float x = (*ve).mX;
				    	float y = (*ve).mY;
				    	float z = (*ve).mZ;

				    	if( bFirst ) {
				    		f_ModelMinX = f_ModelMaxX = x;
				    		f_ModelMinY = f_ModelMaxY = y;
				    		f_ModelMinZ = f_ModelMaxZ = z;
				    		bFirst = false;
				    	} else {
				    		if( x < f_ModelMinX ) {
				    			f_ModelMinX = x;
				    		}
				    		if( x > f_ModelMaxX ) {
				    			f_ModelMaxX = x;
				    		}
				    		if( y < f_ModelMinY ) {
				    			f_ModelMinY = y;
				    		}
				    		if( y > f_ModelMaxY ) {
				    			f_ModelMaxY = y;
				    		}
				    		if( z < f_ModelMinZ ) {
				    			f_ModelMinZ = z;
				    		}
				    		if( z > f_ModelMaxZ ) {
				    			f_ModelMaxZ = z;
				    		}
				    	}
				    }
				}
			}
		}
		f_ModelW = f_ModelMaxX - f_ModelMinX;
		f_ModelH = f_ModelMaxY - f_ModelMinY;
		f_ModelL = f_ModelMaxZ - f_ModelMinZ;
	}

	// void Model::display( Model& model, int argc, char* argv[] ) {

	void Model::display( int argc, char* argv[] ) {
	  	o_Model = this;

		displayInitializer();

	  	glutInit( &argc, argv ); // Initialize GLUT and process user parameters

		glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH ); // Request double buffered true color window with Z-buffer

		glutInitWindowSize(640, 480);   // Set the window's initial width & height
		glutInitWindowPosition(50, 50); // Position the window's initial top-left corner  
		glutCreateWindow("How the Building has been being Built...");

		glEnable( GL_DEPTH_TEST ); // Enable Z-buffer depth test
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		// Callback functions
		glutDisplayFunc(displayFunc);
		glutSpecialFunc(displayKeys);
		glutMouseFunc(displayMouse);
		glutReshapeFunc(displayReshape);       // Register callback handler for window re-size event

		//  Pass control to GLUT for events
		glutMainLoop();
	}


	static void displayReshape( GLsizei width, GLsizei height ) {

		if( width > height ) {
			width = height;
		} else {
			height = width;
		}

		// Set the viewport to cover the new window
		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
		glLoadIdentity();             // Reset

		float fMarginX = f_ModelW*1.0;
		float fMarginY = f_ModelH*1.0;
		float fMarginZ = f_ModelL*1.0;
		glOrtho( f_ModelMinX-fMarginX, f_ModelMaxX+fMarginX, f_ModelMinY-fMarginY, f_ModelMaxY+fMarginY, f_ModelMinZ-fMarginZ, f_ModelMaxZ+fMarginZ );
		//std::cout << "f_ModelMinX=" << f_ModelMinX <<", f_ModelMaxX=" << f_ModelMaxX;
		//std::cout << ", f_ModelMinY=" << f_ModelMinY << ", f_ModelMaxY=" << f_ModelMaxY;
		//std::cout << ", f_ModelMinZ=" << f_ModelMinZ << ", f_ModelMaxZ=" << f_ModelMaxZ <<"\n";
		_windowWidth = width;
		_windowHeight = height;
	}


	static void displayFunc( void ) { 
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );   //  Clear screen and Z-buffer
		
		glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
	 
	 	glLoadIdentity(); // Reset transformations
	  
		glPushMatrix();

		glTranslatef( (f_ModelMinX + f_ModelW/2.0), 0.0, (f_ModelMinZ + f_ModelL/2.0) );  
		glRotatef( i_ModelRotateY, 0.0, 1.0, 0.0 );
		glTranslatef( -(f_ModelMinX + f_ModelW/2.0), 0.0, -(f_ModelMinZ + f_ModelL/2.0) );  

		glTranslatef( 0.0, (f_ModelMinY + f_ModelH/2.0), (f_ModelMinZ + f_ModelL/2.0) );
		glRotatef( i_ModelRotateX, 1.0, 0.0, 0.0 );
		glTranslatef( 0.0, -(f_ModelMinY + f_ModelH/2.0), -(f_ModelMinZ + f_ModelL/2.0) );

		displayAxis();
		
		/*
		glTranslatef( (f_ModelMinX + f_ModelW/2.0), (f_ModelMinY + f_ModelH/2.0), (f_ModelMinZ + f_ModelL/2.0) );  
		glRotatef( i_ModelRotateY, 0.0, 1.0, 0.0 );
		glRotatef( i_ModelRotateX, 1.0, 0.0, 0.0 );	
		glTranslatef( -(f_ModelMinX + f_ModelW/2.0), -(f_ModelMinY + f_ModelH/2.0), -(f_ModelMinZ + f_ModelL/2.0) );  
		*/

	    for( std::vector<Operation>::iterator op = (*o_Model).mOperations.begin() ; op != (*o_Model).mOperations.end() ; ++op ) {    	
	    	time_t tStart = op->tActualStart;
	    	time_t tFinish = op->tActualFinish;
	    	double dAfterStart = difftime( t_ModelTime, tStart  );
	    	double dAfterFinish = difftime( t_ModelTime, tFinish ); 
	    	float progress;
	    	if( !(dAfterStart > 0.0) ) {
	    		progress = 0;
	    	} else if( !(dAfterFinish < 0.0) ) {
	    		progress = 100;
	    	} else {
	    		progress = dAfterStart / (dAfterStart-dAfterFinish);	
	    	}
	    	bool selected = (_operationSelected == &(*op)) ? true : false;
	        for( std::vector<Object>::iterator ob = (*op).mObjects.begin() ; ob != (*op).mObjects.end() ; ++ob ) {
		        for( std::vector<Facet>::iterator fa = (*ob).mFacets.begin() ; fa != (*ob).mFacets.end() ; ++fa ) {
		        	displayFacet( *fa, progress, selected );
				}
			}
	    }	

		if( _operationSelected ) {
			Operation *op = _operationSelected;
			char text[100];

			if( t_ModelTime < op->tActualStart ) {
				sprintf( text, "This operation is to start at: %ld", op->tActualStart );
			} else if ( t_ModelTime > op->tActualFinish ) {
				sprintf( text, "This operation haas been finished at: %ld", op->tActualFinish );
			} else {
				int pct = int( ( (t_ModelTime - op->tActualStart)*100 ) / (op->tActualFinish - op->tActualStart) );  
				sprintf( text, "This operation is under way now, %d%% done.", pct );
			}
			glColor3f( 1.0f, 1.0f, 1.0f );

			glMatrixMode( GL_PROJECTION );
			glPushMatrix();
			glLoadIdentity();
			gluOrtho2D( 0, _windowWidth, 0, _windowHeight );
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
			glLoadIdentity();
			
			glRasterPos2i( 10, 10 );
			for( int i = 0 ; text[i] != '\x0' ; ++i ) {
			    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, text[i] );
			}

			glPopMatrix();
			glMatrixMode( GL_PROJECTION );
			glPopMatrix();
			glMatrixMode( GL_MODELVIEW );
			
			glutSwapBuffers();
		}

		glPopMatrix();
		//glFlush();
		glutSwapBuffers(); 
	}


	static void displayFacet( Facet& facet, float progress, bool selected ) {
		// Facet
		glBegin(GL_POLYGON);
		glColor4f( 1.0f, 1.0f, 1.0f, progress );
	    for( std::vector<Vertex>::iterator ve = facet.mVertices.begin() ; ve != facet.mVertices.end() ; ++ve ) {
			glVertex3f( ve->mX, ve->mY, ve->mZ );
	    }
		glEnd();

		// Ribs only
		glBegin(GL_LINES);
		glEnable( GL_LINE_SMOOTH );
		glLineWidth( 4.0 );
		if( !selected ) {
			glColor3f( 1.0, 1.0, 1.0 );     
		} else {
			glColor3f( 0.0, 0.4, 0.8 );     		
		}
		bool firstVertex = false;
		bool prevVertex = false;
		double prevX, prevY, prevZ;
		double firstX, firstY, firstZ;
	    for( std::vector<Vertex>::iterator ve = facet.mVertices.begin() ; ve != facet.mVertices.end() ; ++ve ) {
			if( !firstVertex ) {
				firstX = ve->mX; firstY = ve->mY; firstZ = ve->mZ;
				firstVertex = true;
			} else {
				glVertex3f( ve->mX, ve->mY, ve->mZ );
				glVertex3f( prevX, prevY, prevZ );
			}
			prevX = ve->mX; prevY = ve->mY; prevZ = ve->mZ;
			prevVertex = true;
		}
		if( firstVertex && prevVertex ) {
			glVertex3f( prevX, prevY, prevZ );
			glVertex3f( firstX, firstY, firstZ );
		}
		glEnd();	
	}


	static void displayAxis( void ) {
		float cx = f_ModelMinX - f_ModelW*0.25;
		float cy = f_ModelMinY - f_ModelH*0.01;
		float cz = f_ModelMinZ - f_ModelL*0.25;
		glBegin(GL_LINES);
		glColor3f( 1.0, 0.0, 1.0 );
		glVertex3f( cx, cy, cz );
		glVertex3f( cx, f_ModelMaxY, cz );

		glVertex3f( cx, cy, cz );
		glVertex3f( f_ModelMaxX, cy, cz );

		glVertex3f( cx, cy, cz );
		glVertex3f( cx, cy, f_ModelMaxZ );
		glEnd();
	}


	static void displayKeys( int key, int x, int y ) {
	 
		switch(key) {
			case GLUT_KEY_RIGHT:
				i_ModelRotateY += 5;
				break;
			case GLUT_KEY_LEFT:
				i_ModelRotateY -= 5;
				break;
			case GLUT_KEY_UP:
				i_ModelRotateX += 5;
				break;
			case GLUT_KEY_DOWN:
				i_ModelRotateX -= 5;
				break;
			case GLUT_KEY_PAGE_UP:
				if( t_ModelTime < t_ModelFinish ) {
					t_ModelTime += (t_ModelFinish - t_ModelStart)/i_DateScaleRate;
				} else if( t_ModelTime < t_TimeNow ) {
					t_ModelTime = t_TimeNow;
				}
				break;
			case GLUT_KEY_PAGE_DOWN:
				if( t_ModelTime > t_ModelFinish ) {
					t_ModelTime = t_ModelFinish;
				} else if( t_ModelTime > t_ModelStart ) {
					t_ModelTime -= (t_ModelFinish - t_ModelStart)/i_DateScaleRate;
				}
				break;
		} 
	  //  Request display update
	  glutPostRedisplay();
	}

	static void displayMouse ( int button, int state, int x, int y ) {
		if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) { 
			double objx, objy, objz;
			double modelview[16], projection[16];
			int viewport[4];
			float z;


			// get the modelview matrix		
			glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
			// get the projection matrix
			glGetDoublev( GL_PROJECTION_MATRIX, projection );
			// get the viewport		
			glGetIntegerv( GL_VIEWPORT, viewport );
			// Read the window z coordinate (the z value on that point in unit cube)		
			glReadPixels( x, viewport[3]-y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z );
			// Unproject the window coordinates to find the world coordinates.
			gluUnProject( x, viewport[3]-y, z, modelview, projection, viewport, &objx, &objy, &objz ); 

			std::cout << objx << " , " << objy << " , " << objz << "\n";

			Operation *operationFound=NULL; 
			for( std::vector<Operation>::iterator  op = o_Model->mOperations.begin() ; op != o_Model->mOperations.end() ; ++op ) {
			    double minX, maxX, minY, maxY, minZ, maxZ;
			    bool first=true;
			    for( std::vector<Object>::iterator ob = (*op).mObjects.begin() ; ob != (*op).mObjects.end() ; ++ob ) {
				    for( std::vector<Facet>::iterator fa = (*ob).mFacets.begin() ; fa != (*ob).mFacets.end() ; ++fa ) {	
					    for( std::vector<Vertex>::iterator ve = (*fa).mVertices.begin() ; ve != (*fa).mVertices.end() ; ++ve ) {
					    	if( first ) {
					    		minX = maxX = ve->mX;
					    		minY = maxY = ve->mY;
					    		minZ = maxZ = ve->mZ;
					    		first = false;
					    	} else {
					    		if( ve->mX < minX) { minX = ve->mX; }
					    		if( ve->mX > maxX ) { maxX = ve->mX; }
					    		if( ve->mY < minY ) { minY = ve->mY; }
					    		if( ve->mY > maxY ) { maxY = ve->mY; }
					    		if( ve->mZ < minZ ) { minZ = ve->mZ; }
					    		if( ve->mZ > maxZ ) { maxZ = ve->mZ; }
					    	}
					    }
					}
				}
				if( !first ) {
					if( !(objx < minX) && !(objx > maxX) && !(objy < minY) && !(objy > maxY) && !(objz < minZ) && !(objz > maxZ) ) {
						_operationSelected = &(*op);
						glutPostRedisplay();
						break;
					}
				}
			}
		}	
	}

}
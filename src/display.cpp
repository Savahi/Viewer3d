#include <vector>
#include <iostream>
#include <stdio.h> 
#include <math.h>

#define GL_GLEX_tPROTOTYPES
#include <GL/glut.h>

#include "models.hpp"
#include "operations.hpp"
#include "viewer3d.hpp"

namespace Spider3d {

	static void displayInitializer( void );

	static void displayFunc( void );
	static void displayModelsWithZeroProgress( void );	
	static void displayAxis( void );
	static void displayFacet( Facet& facet, float progress=100, bool selected=false );
	static void displaySelectedModelInfo( void );
	static void displayKeys( int key, int x, int y );
	static void displayMouse ( int button, int state, int x, int y );
	static void displayReshape( GLsizei width, GLsizei height );

	static Models *_models;
	static Operations *_operations;
	static Model *_modelSelected=NULL;

	static GLsizei _windowWidth, _windowHeight;

	static float _fModelsW, _fModelsL, _fModelsH, _fModelsMinX, _fModelsMaxX, _fModelsMinY, _fModelsMaxY, _fModelsMinZ, _fModelsMaxZ;
	static int _iModelsRotateX, _iModelsRotateY;
	static time_t _tTimeNow;
	static time_t _tOperationsTime;
	static time_t _tOperationsStart, _tOperationsFinish; 

	static int _iDateScaleRate = 50;

	static void displayInitializer( void ) {

	  	_iModelsRotateX = 15;
	  	_iModelsRotateY = 15;
	  	_tTimeNow = _tOperationsTime = time(0);
	  	_tOperationsStart = _tOperationsFinish = -1.0;

		for( std::vector<Operation>::iterator op = _operations->mOperations.begin() ; op != _operations->mOperations.end() ; ++op ) {
			if( _tOperationsStart < 0.0 ) { 
				_tOperationsStart = op->tActualStart;
			} else if( op->tActualStart < _tOperationsStart ) {
				_tOperationsStart = op->tActualStart;
			} 
			if( _tOperationsFinish < 0.0 ) {
				_tOperationsFinish = op->tActualFinish;			
			} else if ( op->tActualFinish > _tOperationsFinish ) {
				_tOperationsFinish = op->tActualFinish;
			}
		}
		
		bool bFirst = true;	
		for( std::vector<Model>::iterator model = _models->mModels.begin() ; model != _models->mModels.end() ; ++model ) {
		    for( std::vector<Facet>::iterator fa = model->mFacets.begin() ; fa != model->mFacets.end() ; ++fa ) {	
			    for( std::vector<Vertex>::iterator ve = fa->mVertices.begin() ; ve != fa->mVertices.end() ; ++ve ) {
			    	float x = (*ve).mX;
			    	float y = (*ve).mY;
			    	float z = (*ve).mZ;

			    	if( bFirst ) {
			    		_fModelsMinX = _fModelsMaxX = x;
			    		_fModelsMinY = _fModelsMaxY = y;
			    		_fModelsMinZ = _fModelsMaxZ = z;
			    		bFirst = false;
			    	} else {
			    		if( x < _fModelsMinX ) {
			    			_fModelsMinX = x;
			    		}
			    		if( x > _fModelsMaxX ) {
			    			_fModelsMaxX = x;
			    		}
			    		if( y < _fModelsMinY ) {
			    			_fModelsMinY = y;
			    		}
			    		if( y > _fModelsMaxY ) {
			    			_fModelsMaxY = y;
			    		}
			    		if( z < _fModelsMinZ ) {
			    			_fModelsMinZ = z;
			    		}
			    		if( z > _fModelsMaxZ ) {
			    			_fModelsMaxZ = z;
			    		}
			    	}
			    }
			}
		}
		_fModelsW = _fModelsMaxX - _fModelsMinX;
		_fModelsH = _fModelsMaxY - _fModelsMinY;
		_fModelsL = _fModelsMaxZ - _fModelsMinZ;
	}


	void display( Models& models, Operations& operations, int argc, char* argv[] ) {
	  	_models = &models;
	  	_operations = &operations;

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

		float fMarginX = _fModelsW*1.0;
		float fMarginY = _fModelsH*1.0;
		float fMarginZ = _fModelsL*1.0;
		glOrtho( _fModelsMinX-fMarginX, _fModelsMaxX+fMarginX, _fModelsMinY-fMarginY, _fModelsMaxY+fMarginY, _fModelsMinZ-fMarginZ, _fModelsMaxZ+fMarginZ );
		//std::cout << "_fModelsMinX=" << _fModelsMinX <<", _fModelsMaxX=" << _fModelsMaxX;
		//std::cout << ", _fModelsMinY=" << _fModelsMinY << ", _fModelsMaxY=" << _fModelsMaxY;
		//std::cout << ", _fModelsMinZ=" << _fModelsMinZ << ", _fModelsMaxZ=" << _fModelsMaxZ <<"\n";
		_windowWidth = width;
		_windowHeight = height;
	}


	static void displayFunc( void ) { 
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );   //  Clear screen and Z-buffer
		
		glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
	 
	 	glLoadIdentity(); // Reset transformations
	  
		glPushMatrix();

		glTranslatef( (_fModelsMinX + _fModelsW/2.0), 0.0, (_fModelsMinZ + _fModelsL/2.0) );  
		glRotatef( _iModelsRotateY, 0.0, 1.0, 0.0 );
		glTranslatef( -(_fModelsMinX + _fModelsW/2.0), 0.0, -(_fModelsMinZ + _fModelsL/2.0) );  

		glTranslatef( 0.0, (_fModelsMinY + _fModelsH/2.0), (_fModelsMinZ + _fModelsL/2.0) );
		glRotatef( _iModelsRotateX, 1.0, 0.0, 0.0 );
		glTranslatef( 0.0, -(_fModelsMinY + _fModelsH/2.0), -(_fModelsMinZ + _fModelsL/2.0) );

		displayAxis();
		
		/*
		glTranslatef( (_fModelsMinX + _fModelsW/2.0), (_fModelsMinY + _fModelsH/2.0), (_fModelsMinZ + _fModelsL/2.0) );  
		glRotatef( _iModelsRotateY, 0.0, 1.0, 0.0 );
		glRotatef( _iModelsRotateX, 1.0, 0.0, 0.0 );	
		glTranslatef( -(_fModelsMinX + _fModelsW/2.0), -(_fModelsMinY + _fModelsH/2.0), -(_fModelsMinZ + _fModelsL/2.0) );  
		*/

		/*
	    for( std::vector<Model>::iterator model = (*_models).mModels.begin() ; model != (*_models).mModels.end() ; ++model ) {    	
	    	time_t tStart = model->tActualStart;
	    	time_t tFinish = model->tActualFinish;
	    	double dAfterStart = difftime( _tOperationsTime, tStart  );
	    	double dAfterFinish = difftime( _tOperationsTime, tFinish ); 
	    	float progress;
	    	if( !(dAfterStart > 0.0) ) {
	    		progress = 0;
	    	} else if( !(dAfterFinish < 0.0) ) {
	    		progress = 100;
	    	} else {
	    		progress = dAfterStart / (dAfterStart-dAfterFinish);	
	    	}
	    	bool selected = (_modelSelected == &(*model)) ? true : false;
	        for( std::vector<Facet>::iterator fa = (*model).mFacets.begin() ; fa != (*model).mFacets.end() ; ++fa ) {
	        	displayFacet( *fa, progress, selected );
			}
	    }	
		*/

	    displayModelsWithZeroProgress();
		
		//displaySelectedModelInfo();

		glPopMatrix();
		//glFlush();
		glutSwapBuffers(); 
	}

	static void displayModelsWithZeroProgress() {

	    for( std::vector<Model>::iterator model = (*_models).mModels.begin() ; model != (*_models).mModels.end() ; ++model ) {    	
	    	float progress;
	    	bool selected = (_modelSelected == &(*model)) ? true : false;
	        for( std::vector<Facet>::iterator fa = (*model).mFacets.begin() ; fa != (*model).mFacets.end() ; ++fa ) {
	        	displayFacet( *fa, progress, selected );
			}
	    }	

	}

	static void displaySelectedModelInfo() {
		/*
		if( _modelSelected ) {
			Model *model = _modelSelected;
			char text[100];

			if( _tOperationsTime < model->tActualStart ) {
				sprintf( text, "This operation is to start at: %ld", model->tActualStart );
			} else if ( _tOperationsTime > model->tActualFinish ) {
				sprintf( text, "This operation haas been finished at: %ld", model->tActualFinish );
			} else {
				int pct = int( ( (_tOperationsTime - model->tActualStart)*100 ) / (model->tActualFinish - model->tActualStart) );  
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
		*/
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
		float cx = _fModelsMinX - _fModelsW*0.25;
		float cy = _fModelsMinY - _fModelsH*0.01;
		float cz = _fModelsMinZ - _fModelsL*0.25;
		glBegin(GL_LINES);
		glColor3f( 1.0, 0.0, 1.0 );
		glVertex3f( cx, cy, cz );
		glVertex3f( cx, _fModelsMaxY, cz );

		glVertex3f( cx, cy, cz );
		glVertex3f( _fModelsMaxX, cy, cz );

		glVertex3f( cx, cy, cz );
		glVertex3f( cx, cy, _fModelsMaxZ );
		glEnd();
	}


	static void displayKeys( int key, int x, int y ) {
	 
		switch(key) {
			case GLUT_KEY_RIGHT:
				_iModelsRotateY += 5;
				break;
			case GLUT_KEY_LEFT:
				_iModelsRotateY -= 5;
				break;
			case GLUT_KEY_UP:
				_iModelsRotateX += 5;
				break;
			case GLUT_KEY_DOWN:
				_iModelsRotateX -= 5;
				break;
			case GLUT_KEY_PAGE_UP:
				if( _tOperationsTime < _tOperationsFinish ) {
					_tOperationsTime += (_tOperationsFinish - _tOperationsStart)/_iDateScaleRate;
				} else if( _tOperationsTime < _tTimeNow ) {
					_tOperationsTime = _tTimeNow;
				}
				break;
			case GLUT_KEY_PAGE_DOWN:
				if( _tOperationsTime > _tOperationsFinish ) {
					_tOperationsTime = _tOperationsFinish;
				} else if( _tOperationsTime > _tOperationsStart ) {
					_tOperationsTime -= (_tOperationsFinish - _tOperationsStart)/_iDateScaleRate;
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

			Model *modelToSelect = NULL;
			for( std::vector<Model>::iterator model = _models->mModels.begin() ; model != _models->mModels.end() ; ++model ) {
			    double minX, maxX, minY, maxY, minZ, maxZ;
			    bool coordsInitialized = false;
			    for( std::vector<Facet>::iterator fa = (*model).mFacets.begin() ; fa != (*model).mFacets.end() ; ++fa ) {	
				    for( std::vector<Vertex>::iterator ve = (*fa).mVertices.begin() ; ve != (*fa).mVertices.end() ; ++ve ) {
				    	if( !coordsInitialized ) {
				    		minX = maxX = ve->mX;
				    		minY = maxY = ve->mY;
				    		minZ = maxZ = ve->mZ;
				    		coordsInitialized = true;
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

				if( modelToSelect == NULL ) {
					if( coordsInitialized ) {
						if( !(objx < minX) && !(objx > maxX) && !(objy < minY) && !(objy > maxY) && !(objz < minZ) && !(objz > maxZ) ) {
							if( _modelSelected != &(*model) ) {
								modelToSelect = &(*model);
							}
						}
					}
				}
			}

			if( modelToSelect ) {
				_modelSelected = modelToSelect;
				glutPostRedisplay();
			}
		}	
	}

}
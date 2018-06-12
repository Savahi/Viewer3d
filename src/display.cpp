#include <vector>
#include <iostream>
#include <stdio.h> 
#include <string.h>
#include <math.h>

#define GL_GLEX_tPROTOTYPES
#include <GL/glut.h>

#include "models.hpp"
#include "optypes.hpp"
#include "operations.hpp"
#include "display.hpp"
#include "viewer3d.hpp"

namespace Spider3d {

	static void displayInitializer( void );

	static void displayFunc( void );
	static void displayModelsWithZeroProgress( void );	
	static void displayModelsWithOperations( void );

	static void displayModel( Model& model, int iOrder, double dProgress=0.0, float fR=0.8, float fG=0.8, float fB=0.8f );	

	static void displayAxis( void );

	static void catchKeys( int key, int x, int y );
	static void catchMouse ( int button, int state, int x, int y );
	static void catchReshape( GLsizei width, GLsizei height );

	static Models *_models;
	static Operations *_operations;
	static OpTypes *_opTypes;
	static Model *_modelSelected=NULL;

	GLsizei _iWindowWidth, _iWindowHeight;
	double _fWindowLeft, _fWindowRight, _fWindowBottom, _fWindowTop, _fWindowWidth, _fWindowHeight;
	double _fModelAreaLeft, _fModelAreaRight, _fModelAreaBottom, _fModelAreaTop;

	static double _fModelsMinX=0, _fModelsMaxX=0, _fModelsMinY=0, _fModelsMaxY=0, _fModelsMinZ=0, _fModelsMaxZ=0;
	static double _fModelsW=0, _fModelsL=0, _fModelsH=0;
	static int _iModelsRotateX, _iModelsRotateY;
	time_t _tDisplayTimeActual;
	time_t _tDisplayTime;
	time_t _tDisplayTimeMin, _tDisplayTimeMax; 

	static int _iDateScaleRate = 50;

	static void displayInitializer( void ) {

	  	_iModelsRotateX = 15;
	  	_iModelsRotateY = 15;
	  	_tDisplayTimeActual = _tDisplayTime = time(0);
	  	_tDisplayTimeMin = _tDisplayTimeMax = -1.0;

		for( std::vector<Operation>::iterator op = _operations->mOperations.begin() ; op != _operations->mOperations.end() ; ++op ) {
			if( _tDisplayTimeMin < 0.0 ) { 
				_tDisplayTimeMin = op->tActualStart;
			} else if( op->tActualStart < _tDisplayTimeMin ) {
				_tDisplayTimeMin = op->tActualStart;
			} 
			if( _tDisplayTimeMax < 0.0 ) {
				_tDisplayTimeMax = op->tActualFinish;			
			} else if ( op->tActualFinish > _tDisplayTimeMax ) {
				_tDisplayTimeMax = op->tActualFinish;
			}
		}

		bool bFirst = true;	
		for( std::vector<Model>::iterator model = _models->mModels.begin() ; model != _models->mModels.end() ; ++model ) {
			if( !model->bMinMax ) {
				continue;
			}
	    	if( bFirst ) {
	    		_fModelsMinX = model->fMinX;
	    		_fModelsMaxX = model->fMaxX;
	    		_fModelsMinY = model->fMinY;
	    		_fModelsMaxY = model->fMaxY;
	    		_fModelsMinZ = model->fMinZ;
	    		_fModelsMaxZ = model->fMaxZ;
	    		bFirst = false;
	    	} else {
	    		if( model->fMinX < _fModelsMinX ) {
	    			_fModelsMinX = model->fMinX;
	    		}
	    		if( model->fMaxX > _fModelsMaxX ) {
	    			_fModelsMaxX = model->fMaxX;
	    		}
	    		if( model->fMinY < _fModelsMinY ) {
	    			_fModelsMinY = model->fMinY;
	    		}
	    		if( model->fMaxY > _fModelsMaxY ) {
	    			_fModelsMaxY = model->fMaxY;
	    		}
	    		if( model->fMinZ < _fModelsMinZ ) {
	    			_fModelsMinZ = model->fMinZ;
	    		}
	    		if( model->fMaxZ > _fModelsMaxZ ) {
	    			_fModelsMaxZ = model->fMaxZ;
	    		}
	    	}
		}
		_fModelsW = _fModelsMaxX - _fModelsMinX;
		_fModelsH = _fModelsMaxY - _fModelsMinY;
		_fModelsL = _fModelsMaxZ - _fModelsMinZ;

		// Creating operations stack sorted ascending...
		for( std::vector<Model>::iterator model = _models->mModels.begin() ; model != _models->mModels.end() ; ++model ) {
			for( std::vector<Operation>::iterator op = _operations->mOperations.begin() ; op != _operations->mOperations.end() ; ++op ) {
				if( model->sCode.compare( op->sModelCode) != 0 ) {
					continue;
				}
    			model->operations.insert( std::pair<time_t,Operation*>( op->tActualStart, &(*op) ) );

    			// Searching for the corresponding opType and assining a pointer to.
				for( std::vector<OpType>::iterator opType = _opTypes->mOpTypes.begin() ; opType != _opTypes->mOpTypes.end() ; ++opType ) {
					if( op->sType.compare( opType->sCode ) == 0 ) {
						(*op).opType = &(*opType);
						break;
					}
				}
    		}
    	}
	}


	void display( Models& models, Operations& operations, OpTypes& opTypes, int argc, char* argv[] ) {
	  	_models = &models;
	  	_operations = &operations;
	  	_opTypes = &opTypes;

		displayInitializer();

	  	glutInit( &argc, argv ); // Initialize GLUT and process user parameters

		glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH ); // Request double buffered true color window with Z-buffer

		glutInitWindowSize(800, 600);   // Set the window's initial width & height
		glutInitWindowPosition(50, 50); // Position the window's initial top-left corner  
		glutCreateWindow("How the Building has been being Built...");

		glEnable( GL_DEPTH_TEST ); // Enable Z-buffer depth test

		// Callback functions
		glutDisplayFunc(displayFunc);
		glutSpecialFunc(catchKeys);
		glutMouseFunc(catchMouse);
		glutReshapeFunc(catchReshape);       // Register callback handler for window re-size event

		//  Pass control to GLUT for events
		glutMainLoop();
	}


	static void catchReshape( GLsizei width, GLsizei height ) {
		_iWindowWidth = width;
		_iWindowHeight = height;
		// Set the viewport to cover the whole window
		//glViewport( 0, 0, _iWindowWidth * DISPLAY_MODEL_W, _iWindowHeight * DISPLAY_MODEL_H );
		glViewport( 0, 0, _iWindowWidth, _iWindowHeight );
	}


	static void displayFunc( void ) { 

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );   //  Clear screen and Z-buffer

		displaySelectedModelInfo( _modelSelected, _tDisplayTime );

		displayTimeScale();

		glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
		glLoadIdentity();             // Reset

		_fModelAreaLeft = _fModelsMinX - _fModelsW*0.75;
		_fModelAreaRight = _fModelsMaxX + _fModelsW*0.75;
		_fModelAreaBottom = _fModelsMinY - _fModelsH*0.5;
		_fModelAreaTop = _fModelsMaxY + _fModelsH*0.5;
		_fWindowLeft = _fModelAreaLeft;
		_fWindowRight = _fModelAreaRight + _fModelsW * DISPLAY_AREA_RIGHT_PANE;
		_fWindowWidth = _fWindowRight - +_fWindowLeft;
		_fWindowBottom = _fModelAreaBottom;
		_fWindowTop = _fModelAreaTop + _fModelsH * DISPLAY_AREA_TOP_PANE;
		_fWindowHeight = _fWindowTop - _fWindowBottom;
		double fMarginZ = _fModelsL*0.75;
		glOrtho( _fModelAreaLeft, _fWindowRight, _fModelAreaBottom, _fWindowTop, _fModelsMinZ - fMarginZ, _fModelsMaxZ + fMarginZ );
		
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

		displayModelsWithOperations();
	    displayModelsWithZeroProgress();

		glPopMatrix();
		glutSwapBuffers(); 
	}

	static void displayModelsWithZeroProgress() {
	    for( std::vector<Model>::iterator model = _models->mModels.begin() ; model != _models->mModels.end() ; ++model ) {    	
	    	displayModel( (*model), 0, 0.00 );
	    }	
	}

	static void displayModelsWithOperations( void ) {
		for( std::vector<Model>::iterator model = _models->mModels.begin() ; model != _models->mModels.end() ; ++model ) {    	

			int iOrder = 0;
		    std::map<time_t,Operation*>::iterator opPair=model->operations.begin();
		    for( ; opPair != model->operations.end() ; ++opPair, ++iOrder ) {
		    	Operation *op = opPair->second;
		    	time_t tStart = op->tActualStart;
	    		time_t tFinish = op->tActualFinish;
		    	double dAfterStart = difftime( _tDisplayTime, tStart  );
		    	double dAfterFinish = difftime( _tDisplayTime, tFinish ); 
		    	double dProgress;
		    	if( !(dAfterStart > 0.0) ) {
		    		continue;
		    	} else if( !(dAfterFinish < 0.0) ) {
		    		dProgress = 1.0;
		    	} else {
		    		dProgress = dAfterStart / (dAfterStart-dAfterFinish);	
		    	}
		    	//printf( "%s: %g/%d\n", (op->sName).c_str(), dProgress, iOrder );
		    	if( dProgress > 0.0 ) {
		    		float fR=0.8, fG=0.8, fB=0.8;
		    		if( op->opType != NULL ) {
		    			fR = op->opType->fR; fG = op->opType->fG; fB = op->opType->fB;
		    		}
		    		displayModel( (*model), model->operations.size()-iOrder-1, dProgress, fR, fG, fB );
		    	}
        	}
        }
    }

	static void displayModel( Model& model, int iOrder, double dProgress, float fR, float fG, float fB ) {
    	bool selected = (_modelSelected == &model) ? true : false;
        for( std::vector<Facet>::iterator fa = model.mFacets.begin() ; fa != model.mFacets.end() ; ++fa ) {
        	displayFacet( *fa, model, iOrder, dProgress, fR, fG, fB, selected );
		}
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


	static void catchKeys( int key, int x, int y ) {
	 
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
				if( _tDisplayTime < _tDisplayTimeMax ) {
					_tDisplayTime += (_tDisplayTimeMax - _tDisplayTimeMin)/_iDateScaleRate;
				} else if( _tDisplayTime < _tDisplayTimeActual ) {
					_tDisplayTime = _tDisplayTimeActual;
				}
				break;
			case GLUT_KEY_PAGE_DOWN:
				if( _tDisplayTime > _tDisplayTimeMax ) {
					_tDisplayTime = _tDisplayTimeMax;
				} else if( _tDisplayTime > _tDisplayTimeMin ) {
					_tDisplayTime -= (_tDisplayTimeMax - _tDisplayTimeMin)/_iDateScaleRate;
				}
				break;
		} 
	  //  Request display update
	  glutPostRedisplay();
	}

	static void catchMouse ( int button, int state, int x, int y ) {

		if( catchMouseInTimeScale( button, state, x, y ) ) {
			glutPostRedisplay();
			return;
		}
		
		if( !(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) ) { 
			return;
		}

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
		printf( "x=%d, y=%d, l=%d, b=%d, w=%d, h=%d\n", x, y, viewport[0], viewport[1], viewport[2], viewport[3] );
		// Read the window z coordinate (the z value on that point in unit cube)		
		//int vpY = viewport[3] - y + (_iWindowHeight * (1.0 - DISPLAY_MODEL_H));
		glReadPixels( x, viewport[3] - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z );
		printf("COLOR: %f\n", z );
		// Unproject the window coordinates to find the world coordinates.
		gluUnProject( x, viewport[3] - y, z, modelview, projection, viewport, &objx, &objy, &objz ); 

		// std::cout << objx << " , " << objy << " , " << objz << "\n";

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
			if( _modelSelected != modelToSelect ) {
				_modelSelected = modelToSelect;
				glutPostRedisplay();
			}
		} else {
			if( _modelSelected != NULL ) {
				_modelSelected = NULL;
				glutPostRedisplay();
			}
		}
	}

} // The end of the namespace
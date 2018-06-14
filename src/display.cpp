#include <vector>
#include <iostream>
#include <stdio.h> 
#include <string.h>
#include <math.h>
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

	static void catchKeys( int key, int x, int y );
	static void catchMouse ( int button, int state, int x, int y );
	static bool catchMouseInModelArea( int button, int state, int x, int y, double fX, double fY );
	static void catchMouseMotion( int x, int y );
	static void catchMousePassiveMotion( int x, int y );

	static void catchReshape( GLsizei width, GLsizei height );

	static double screenXToWorld( int x );
	static double screenYToWorld( int y );

	static void incrementRotationAngle( int& angle, int inc );

	static Models *_models;
	static Operations *_operations;
	static OpTypes *_opTypes;
	Model *_modelSelected = NULL;
	Model *_modelTouched = NULL;

	GLdouble _faDisplayMVMatrix[16];
	GLdouble _faDisplayPrjMatrix[16];
	GLint _iaDisplayViewport[4];

	GLsizei _iWindowWidth, _iWindowHeight;
	double _fWindowLeft, _fWindowRight, _fWindowBottom, _fWindowTop, _fWindowWidth, _fWindowHeight;
	double _fModelAreaLeft, _fModelAreaRight, _fModelAreaBottom, _fModelAreaTop;

	double _fModelsMinX=0, _fModelsMaxX=0, _fModelsMinY=0, _fModelsMaxY=0, _fModelsMinZ=0, _fModelsMaxZ=0;
	double _fModelsW=0, _fModelsL=0, _fModelsH=0;
	int _iModelsRotateX, _iModelsRotateY;
	time_t _tDisplayTimeActual;
	time_t _tDisplayTime;
	time_t _tDisplayTimeMin, _tDisplayTimeMax; 

	static int _iDateScaleRate = 50;

	bool _bDisplayAxisActive = false;

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
		glutCreateWindow("Viewer3d");

		glEnable( GL_DEPTH_TEST ); // Enable Z-buffer depth test

		glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
		glLoadIdentity();             // Reset

		// Callback functions
		glutReshapeFunc(catchReshape);       // Register callback handler for window re-size event
		glutDisplayFunc(displayFunc);
		glutSpecialFunc(catchKeys);
		glutMouseFunc(catchMouse);
		glutMotionFunc(catchMouseMotion);
		glutPassiveMotionFunc(catchMousePassiveMotion);

		//  Pass control to GLUT for events
		glutMainLoop();
	}


	static void catchReshape( GLsizei width, GLsizei height ) {
		_iWindowWidth = width;
		_iWindowHeight = height;

		// Seting the viewport to cover the whole window
		glViewport( 0, 0, _iWindowWidth, _iWindowHeight );
		glGetIntegerv( GL_VIEWPORT, _iaDisplayViewport );

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
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();		
		glOrtho( _fModelAreaLeft, _fWindowRight, _fModelAreaBottom, _fWindowTop, _fModelsMinZ-_fModelsL*0.75, _fModelsMaxZ+_fModelsL*0.75 );
		glGetDoublev( GL_PROJECTION_MATRIX, _faDisplayPrjMatrix );
		/*
		printf( "\nPRJ:[ %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g ]", 
			_faDisplayPrjMatrix[0], _faDisplayPrjMatrix[1], _faDisplayPrjMatrix[2], _faDisplayPrjMatrix[3], 
			_faDisplayPrjMatrix[4], _faDisplayPrjMatrix[5], _faDisplayPrjMatrix[6], _faDisplayPrjMatrix[7], 
			_faDisplayPrjMatrix[8], _faDisplayPrjMatrix[9], _faDisplayPrjMatrix[10], _faDisplayPrjMatrix[11], 
			_faDisplayPrjMatrix[12], _faDisplayPrjMatrix[13], _faDisplayPrjMatrix[14], _faDisplayPrjMatrix[15] );
		*/		
	}


	static void displayFunc( void ) { 

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );   //  Clear screen and Z-buffer
		
		glMatrixMode(GL_MODELVIEW); // To operate on model-view matrix	 
	 	glLoadIdentity(); // Reset transformations
		glPushMatrix();
	  
		glTranslatef( (_fModelsMinX + _fModelsW/2.0), 0.0, (_fModelsMinZ + _fModelsL/2.0) );  
		glRotatef( _iModelsRotateY, 0.0, 1.0, 0.0 );
		glTranslatef( -(_fModelsMinX + _fModelsW/2.0), 0.0, -(_fModelsMinZ + _fModelsL/2.0) );  

		glTranslatef( 0.0, (_fModelsMinY + _fModelsH/2.0), (_fModelsMinZ + _fModelsL/2.0) );
		glRotatef( _iModelsRotateX, 1.0, 0.0, 0.0 );
		glTranslatef( 0.0, -(_fModelsMinY + _fModelsH/2.0), -(_fModelsMinZ + _fModelsL/2.0) );

		glGetDoublev( GL_MODELVIEW_MATRIX, _faDisplayMVMatrix );
		/*
		printf( "\nMV: [ %g, %g, %g, %g, %g, %g, %g, %g,  %g, %g, %g, %g, %g, %g, %g, %g  ]", _faDisplayMVMatrix[0], _faDisplayMVMatrix[1],
		 _faDisplayMVMatrix[2], _faDisplayMVMatrix[3], _faDisplayMVMatrix[4], _faDisplayMVMatrix[5], _faDisplayMVMatrix[6], 
		 _faDisplayMVMatrix[7], _faDisplayMVMatrix[8], _faDisplayMVMatrix[9], _faDisplayMVMatrix[10], _faDisplayMVMatrix[11], 
		 _faDisplayMVMatrix[12], _faDisplayMVMatrix[13], _faDisplayMVMatrix[14], _faDisplayMVMatrix[15] );
		*/
		displayModelsWithOperations();

	    displayModelsWithZeroProgress();

		displayAxis();

		glPopMatrix();

		displayTimeScale();

		displaySelectedModelInfo();

		displayTools();

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
    	bool touched = (_modelTouched == &model) ? true : false;
        for( std::vector<Facet>::iterator fa = model.mFacets.begin() ; fa != model.mFacets.end() ; ++fa ) {
        	displayFacet( *fa, model, iOrder, dProgress, fR, fG, fB, selected, touched );
		}
	}

	static void catchKeys( int key, int x, int y ) {
	 
		switch(key) {
			case GLUT_KEY_RIGHT:
				incrementRotationAngle( _iModelsRotateY, 5 );
				break;
			case GLUT_KEY_LEFT:
				incrementRotationAngle( _iModelsRotateY, -5 );
				break;
			case GLUT_KEY_UP:
				incrementRotationAngle( _iModelsRotateX, 5 );
				break;
			case GLUT_KEY_DOWN:
				incrementRotationAngle( _iModelsRotateX, -5 );
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

	static void catchMouseMotion( int x, int y ) {
		bool bRedisplay = false;
		bRedisplay = catchMouseInTimeScale( -1, MOUSE_MOVES_PRESSED, x, y, screenXToWorld(x), screenYToWorld(y) );
		bRedisplay = bRedisplay | catchMouseInModelArea( -1, MOUSE_MOVES_PRESSED, x, y, screenXToWorld(x), screenYToWorld(y) ); 		
		if( bRedisplay ) {
			glutPostRedisplay();
		}		
	}

	static void catchMousePassiveMotion( int x, int y ) {
		bool bRedisplay = false;
		bRedisplay = catchMouseInTimeScale( -1, MOUSE_MOVES_NOT_PRESSED, x, y, screenXToWorld(x), screenYToWorld(y) );
		bRedisplay = bRedisplay | catchMouseInModelArea( -1, MOUSE_MOVES_NOT_PRESSED, x, y, screenXToWorld(x), screenYToWorld(y) );
		if( bRedisplay ) {
			glutPostRedisplay();
		}
	}

	static void catchMouse ( int button, int state, int x, int y ) {
		bool bRedisplay = false;

		bRedisplay = catchMouseInTimeScale( button, state, x, y, screenXToWorld(x), screenYToWorld(y) );
		bRedisplay = bRedisplay | catchMouseInTools( button, state, x, y, screenXToWorld(x), screenYToWorld(y) );
		bRedisplay = bRedisplay | catchMouseInModelArea( button, state, x, y, screenXToWorld(x), screenYToWorld(y) );
		if( bRedisplay ) {
			glutPostRedisplay();
		}
	}

	static int _xPrev, _yPrev;

	static bool catchMouseInModelArea( int button, int state, int x, int y, double fX, double fY ) {

		if( fY > _fModelAreaTop || fX > _fModelAreaRight ) {
			bool bReturn = false;
			if( state == MOUSE_MOVES_PRESSED || state == MOUSE_MOVES_NOT_PRESSED ) { // Active or passive motion outside the bounds of the scale.
				if( _bDisplayAxisActive ) {
					_bDisplayAxisActive = false;
					bReturn = true; // Redraw.
				}
				if( _modelTouched ) {
					_modelTouched = NULL;
					bReturn = true; // Redraw.
				}
			}
			return bReturn; 
		}

		GLdouble objx, objy, objz;
		GLfloat z;

		glReadPixels( x, _iaDisplayViewport[3] - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z );
		//printf("Z-depth: %f\n", z );
		// Unproject the window coordinates to find the world coordinates.
		gluUnProject( x, _iaDisplayViewport[3] - y, z, _faDisplayMVMatrix, _faDisplayPrjMatrix, _iaDisplayViewport, &objx, &objy, &objz ); 
		//printf( "Obj: %g,  %g,  %g\n", objx, objy, objz );        

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
			if( !coordsInitialized ) {
				continue;
			}
			
			if( modelToSelect == NULL ) {
				double dX=maxX-minX; 
				double dY=maxY-minY;
				double dZ=maxZ-minZ;
				if( !(objx < minX - dX*0.01) && !(objx > maxX + dX*0.01) && 
					!(objy < minY - dY*0.01) && !(objy > maxY + dY*0.01) && 
					!(objz < minZ - dZ*0.01) && !(objz > maxZ + dZ*0.01) ) {
					//if( _modelSelected != &(*model) ) {
					//	modelToSelect = &(*model);
					//	break;
					//}
					modelToSelect = &(*model);
					break;
				}
			}
		}

		bool bReturn = false;
		if( modelToSelect ) { // The mouse cursor hovers above one of the models 
			if( _bDisplayAxisActive ) {
				_bDisplayAxisActive = false;
				bReturn = true;
			}
			if( _modelSelected != modelToSelect && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
				_modelSelected = modelToSelect;
				bReturn = true;
			} else if( _modelSelected == modelToSelect && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
				_modelSelected = NULL;				
				bReturn = true;
			} else if( state == MOUSE_MOVES_NOT_PRESSED ) {
				_modelTouched = modelToSelect;
				bReturn = true;
			}
		} else { // The mouse cursor hovers above the axis
			if( state == MOUSE_MOVES_NOT_PRESSED ) {
				_modelTouched = NULL;
				bReturn = true;		
			}	
			if( state == MOUSE_MOVES_PRESSED && _bDisplayAxisActive ) {
				if( x != _xPrev || y != _yPrev ) {
					// _iModelsRotateY += ( x - _xPrev );
					// _iModelsRotateX += ( _yPrev - y );
					incrementRotationAngle( _iModelsRotateY, x -_xPrev );
					incrementRotationAngle( _iModelsRotateX, y - _yPrev );
					bReturn = true;					
				}
			}
			if( !_bDisplayAxisActive ) {
				_bDisplayAxisActive = true;
				bReturn = true;
			}

			_xPrev = x;
			_yPrev = y;
		}
		return bReturn;
	}

	static double screenXToWorld( int x ) {
		return _fWindowLeft + ( (double)x / (double)_iWindowWidth ) * _fWindowWidth;
	}
	static double screenYToWorld( int y ) {
		return _fWindowBottom + ( 1.0 - (double)y/(double)_iWindowHeight ) * _fWindowHeight;
	}

	static void incrementRotationAngle( int& angle, int inc ) {
		angle += inc;
		if( angle > 180 ) {
			angle = angle - 360;
		} else if( angle < -180 ) {
			angle = 360 + angle;
		}
	}

} // The end of the namespace
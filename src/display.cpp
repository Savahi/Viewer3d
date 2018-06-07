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
#include "viewer3d.hpp"

namespace Spider3d {

	static void displayInitializer( void );

	static void displayFunc( void );
	static void displayModelsWithZeroProgress( void );	
	static void displayOperations( void );

	static void displaySelectedModelInfo( void );
	static int getOpTypeDetails( std::string& code, std::string& name, float *fpR, float *fpG, float *fpB );	

	static void displayModel( Model& model, double dProgress=0.0 );	
	static void displayFacet( Facet& facet, double progress=0, bool selected=false );
	static void displayRib( double fX1, double fY1, double fZ1, double fX2, double fY2, double fZ2 );	

	static void displayAxis( void );

	static void displayKeys( int key, int x, int y );
	static void displayMouse ( int button, int state, int x, int y );
	static void displayReshape( GLsizei width, GLsizei height );

	static Models *_models;
	static Operations *_operations;
	static OpTypes *_opTypes;
	static Model *_modelSelected=NULL;

	static GLsizei _iWindowWidth, _iWindowHeight;

	static float _fModelsW, _fModelsL, _fModelsH, _fModelsMinX, _fModelsMaxX, _fModelsMinY, _fModelsMaxY, _fModelsMinZ, _fModelsMaxZ;
	static int _iModelsRotateX, _iModelsRotateY;
	static time_t _tTimeNow;
	static time_t _tDisplayTime;
	static time_t _tOperationsStart, _tOperationsFinish; 

	static int _iDateScaleRate = 50;

	static void displayInitializer( void ) {

	  	_iModelsRotateX = 15;
	  	_iModelsRotateY = 15;
	  	_tTimeNow = _tDisplayTime = time(0);
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

		// Creating operations stack sorted ascending...
		for( std::vector<Model>::iterator model = _models->mModels.begin() ; model != _models->mModels.end() ; ++model ) {
			for( std::vector<Operation>::iterator op = _operations->mOperations.begin() ; op != _operations->mOperations.end() ; ++op ) {
				if( model->sCode.compare( op->sModelCode) != 0 ) {
					continue;
				}
				// std::cout << "c=" << model->sCode << " c2=" << op->sModelCode << "\n";				
    			model->operations.insert( std::pair<time_t,Operation*>( op->tActualStart, &(*op) ) );
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
		glOrtho( _fModelsMinX-fMarginX, _fModelsMaxX+fMarginX, 
			_fModelsMinY-fMarginY, _fModelsMaxY+fMarginY, _fModelsMinZ-fMarginZ, _fModelsMaxZ+fMarginZ );
		_iWindowWidth = width;
		_iWindowHeight = height;
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

		displayOperations();
	    displayModelsWithZeroProgress();
		displaySelectedModelInfo();

		glPopMatrix();
		//glFlush();
		glutSwapBuffers(); 
	}

	static void displayModelsWithZeroProgress() {
	    for( std::vector<Model>::iterator model = (*_models).mModels.begin() ; model != (*_models).mModels.end() ; ++model ) {    	
	    	displayModel( (*model), 0.0 );
	    }	
	}

	static void displayOperations( void ) {
		for( std::vector<Model>::iterator model = (*_models).mModels.begin() ; model != (*_models).mModels.end() ; ++model ) {    	
		    for( std::map<time_t,Operation*>::reverse_iterator op=model->operations.rbegin() ; op != model->operations.rend() ; ++op ) {

		    	time_t tStart = (op->second)->tActualStart;
	    		time_t tFinish = (op->second)->tActualFinish;
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

		    	if( dProgress > 0.0 ) {
		    		displayModel( (*model), dProgress );
		    	}
		    	break;
        	}
        }
    }

	static void displaySelectedModelInfo() {
		if( _modelSelected ) {
			Model *m = _modelSelected;
			char caOp[200], caOpType[40], caDTText[80], caDT[41];
			int iOpPair;

			glMatrixMode( GL_PROJECTION );
			glPushMatrix();
			glLoadIdentity();
			gluOrtho2D( 0, _iWindowWidth, 0, _iWindowHeight );
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
			glLoadIdentity();

			iOpPair = 0;
			std::map<time_t,Operation*>::reverse_iterator opPair = m->operations.rbegin();
		    for( ; opPair != m->operations.rend() ; ++opPair, ++iOpPair ) {
		    	
		    	Operation *op = opPair->second;

				if( _tDisplayTime < op->tActualStart ) {
					timetToStr( op->tActualStart, caDT, 40, false );
					sprintf( caDTText, "To start on: %s", caDT );
				} else if ( _tDisplayTime > op->tActualFinish ) {
					timetToStr( op->tActualFinish, caDT, 40, false );
					sprintf( caDTText, "Finished on: %s", caDT );
				} else {
					timetToStr( op->tActualStart, caDT, 40, false );
					int pct = int( ( (_tDisplayTime - op->tActualStart)*100 ) / (op->tActualFinish - op->tActualStart) );  
					sprintf( caDTText, "Started on %s is under way now with %d%% done.", caDT, pct );
				}

				std::string opTypeName;
				float opTypeR, opTypeG, opTypeB;
				int iStatus = getOpTypeDetails( op->sType, opTypeName, &opTypeR, &opTypeG, &opTypeB );
				if( iStatus == 0 && opTypeName.size() < 40 ) {
					sprintf( caOpType, " (%s)", opTypeName.c_str() );
				} else {
					strcpy( caOpType, "" );
				}

				sprintf( caOp, "%s%s: %s", op->sName.c_str(), caOpType, caDTText );

				glColor3f( 1.0f, 1.0f, 1.0f );
				glRasterPos2i( _iWindowWidth * 0.6, _iWindowHeight - 20 - iOpPair*12 );
				for( int i = 0 ; caOp[i] != '\x0' ; ++i ) {
				    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, caOp[i] );
				}
			}

			glPopMatrix();
			glMatrixMode( GL_PROJECTION );
			glPopMatrix();
			glMatrixMode( GL_MODELVIEW );
			
			glutSwapBuffers();
		}
	}

	static int getOpTypeDetails( std::string& sCode, std::string& sName, float *fpR, float *fpG, float *fpB ) {

		for( std::vector<OpType>::iterator opType = _opTypes->mOpTypes.begin() ; opType != _opTypes->mOpTypes.end() ; ++opType ) {
			std::cout << opType->sCode << "vs" << sCode << "\n";
			if( sCode.compare( opType->sCode ) == 0 ) {
				sName.assign( opType->sName );
				if( fpR != NULL ) {
					*fpR = opType->fR;
				}
				if( fpG != NULL ) {
					*fpG = opType->fG;
				}
				if( fpB != NULL ) {
					*fpB = opType->fB;
				}
				return 0;
			}
		}
		return -1;
	}

	static void displayModel( Model& model, double dProgress ) {
    	bool selected = (_modelSelected == &model) ? true : false;
        for( std::vector<Facet>::iterator fa = model.mFacets.begin() ; fa != model.mFacets.end() ; ++fa ) {
        	displayFacet( *fa, dProgress, selected );
		}
	}

	static void displayFacet( Facet& facet, double dProgress, bool selected ) {
		// Facet
		if( dProgress < 0.05 ) {
			dProgress = 0.05;
		}
		glBegin(GL_POLYGON);
		glColor4f( 0.8f, 0.8f, 0.8f, (float)dProgress );
	    for( std::vector<Vertex>::iterator ve = facet.mVertices.begin() ; ve != facet.mVertices.end() ; ++ve ) {
			glVertex3f( ve->mX, ve->mY, ve->mZ );
	    }
		glEnd();

		// Ribs only
		glBegin(GL_LINES);
		glEnable( GL_LINE_SMOOTH );
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
				displayRib( ve->mX, ve->mY, ve->mZ, prevX, prevY, prevZ );
			}
			prevX = ve->mX; prevY = ve->mY; prevZ = ve->mZ;
			prevVertex = true;
		}
		if( firstVertex && prevVertex ) {
			displayRib( prevX, prevY, prevZ, firstX, firstY, firstZ );
		}
		glEnd();	
	}

	static void displayRib( double fX1, double fY1, double fZ1, double fX2, double fY2, double fZ2 ) {
		glVertex3f( fX1,fY1,fZ1 );
		glVertex3f( fX2,fY2,fZ2 );
		/*
		glBegin(GL_POLYGON);
		float lineWidth = 0.01;
		float x1 = fX1 - lineWidth;
		float x2 = fX2 + lineWidth;
		float y1 = fY1 - lineWidth;
		float y2 = fY2 + lineWidth;
		float z1 = fZ1 - lineWidth;
		float z2 = fZ2 + lineWidth;
		glVertex3f( x1,y1,z1 );
		glVertex3f( x1,y1,z1 );
		glEnd();
		*/
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
				if( _tDisplayTime < _tOperationsFinish ) {
					_tDisplayTime += (_tOperationsFinish - _tOperationsStart)/_iDateScaleRate;
				} else if( _tDisplayTime < _tTimeNow ) {
					_tDisplayTime = _tTimeNow;
				}
				break;
			case GLUT_KEY_PAGE_DOWN:
				if( _tDisplayTime > _tOperationsFinish ) {
					_tDisplayTime = _tOperationsFinish;
				} else if( _tDisplayTime > _tOperationsStart ) {
					_tDisplayTime -= (_tOperationsFinish - _tOperationsStart)/_iDateScaleRate;
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
	}

}
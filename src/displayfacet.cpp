
#include <math.h>
#include <GL/glut.h>

#include "models.hpp"
#include "display.hpp"

namespace Spider3d {
	

	static void displayRib( double fX1, double fY1, double fZ1, double fX2, double fY2, double fZ2 );	

	static void calcNorm( double v1X, double v1Y, double v1Z, double v2X, double v2Y, double v2Z, double& kX, double& kY, double& kZ );
	static bool getNorm( Facet& facet, double& kX, double& kY, double&kZ );

	void displayFacet( Facet& facet, Model& model, int iOrder, double dProgress, 
		float fR, float fG, float fB, bool selected, bool touched ) {
		// Facet
		int nVertices = facet.mVertices.size();
		if( nVertices < 3 ) {
			return;
		}

		if( !(dProgress < 1.0) || dProgress > 0.0 ) { 

			glEnable( GL_BLEND );
			glBlendEquation( GL_FUNC_ADD );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glColor4f( fR, fG, fB, 0.5+(dProgress*0.5) );
			glBegin(GL_POLYGON);

			if( iOrder == 0 && false ) {
			    for( std::vector<Vertex>::iterator ve = facet.mVertices.begin() ; ve != facet.mVertices.end() ; ++ve ) {
					glVertex3f( ve->mX, ve->mY, ve->mZ );
			    }
			} else {
				// Caculating the equation of the facet's plane
				double kX, kY, kZ;
				if( !getNorm( facet, kX, kY, kZ ) ) {
					return;
				}

				std::vector<Vertex> adjustedVertices( facet.mVertices );

				// Forcing the facet in...
				double forceInFactor = 0.1;
				if( forceInFactor > 0.0 ) { 				
					forceInFactor = forceInFactor * (double)iOrder;
					double forceInX=0, forceInY=0, forceInZ=0;
					if( kX > 0.0 ) { forceInX = -forceInFactor; }
					else if( kX < 0.0 ) { forceInX = forceInFactor; }
					if( kY > 0.0 ) { forceInY = -forceInFactor; }
					else if( kY < 0.0 ) { forceInY = forceInFactor; }
					if( kZ > 0.0 ) { forceInZ = -forceInFactor; }
					else if( kZ < 0.0 ) { forceInZ = forceInFactor; }

					for( int i = 0 ; i < nVertices ; i++ ) {
						adjustedVertices[i].mX += forceInX;
						adjustedVertices[i].mY += forceInY;
						adjustedVertices[i].mZ += forceInZ;
					}
				}

				// Shrinking the facet 
				double shrinkFactor = 0.0;
				for( int i = 0 ; i < nVertices && shrinkFactor > 0.0 && iOrder > 0 ; i++ ) {
					double prevX, prevY, prevZ, nextX, nextY, nextZ;
					if( i > 0 ) {
						prevX = facet.mVertices[i-1].mX; prevY = facet.mVertices[i-1].mY; prevZ = facet.mVertices[i-1].mZ;
					} else {
						prevX = facet.mVertices[nVertices-1].mX; prevY = facet.mVertices[nVertices-1].mY; prevZ = facet.mVertices[nVertices-1].mZ;
					}
					if( i < nVertices-1 ) {
						nextX = facet.mVertices[i+1].mX; prevY = facet.mVertices[i+1].mY; nextZ = facet.mVertices[i+1].mZ;
					} else {
						nextX = facet.mVertices[0].mX; prevY = facet.mVertices[0].mY; nextZ = facet.mVertices[0].mZ;
					}
					double addX = (nextX + prevX)/2.0 - facet.mVertices[i].mX;
					double addY = (nextY + prevY)/2.0 - facet.mVertices[i].mY;
					double addZ = (nextZ + prevZ)/2.0 - facet.mVertices[i].mZ;
					adjustedVertices[i].mX += addX * (double)iOrder;
					adjustedVertices[i].mY += addY * (double)iOrder;
					adjustedVertices[i].mZ += addZ * (double)iOrder;
				}

				// Adjusting coordinates to display the facet partly constructed
				bool displayPartlyConstructed = true;
				if( displayPartlyConstructed ) {  
					double fModelH = model.fMaxY - model.fMinY;
					if( fModelH > 0.0 ) {
						double fYConstructedUpToNow = model.fMinY + fModelH * dProgress;
						double fFacetMinProgress = ( facet.fMinY - model.fMinY ) / fModelH;  
						double fFacetMaxProgress = ( facet.fMaxY - model.fMinY ) / fModelH;  
						if( facet.fMinY > fYConstructedUpToNow ) {
							return;							
						}
						if( facet.fMaxY > fYConstructedUpToNow ) {
							for( int i = 0 ; i < nVertices ; i++ ) {
								if( adjustedVertices[i].mY > fYConstructedUpToNow ) {
									adjustedVertices[i].mY = fYConstructedUpToNow;
								}
							}
						}
					}
				}


			    for( std::vector<Vertex>::iterator ve = adjustedVertices.begin() ; ve != adjustedVertices.end() ; ++ve ) {
					glVertex3f( ve->mX, ve->mY, ve->mZ );
			    }
			}   
			glEnd();
			glDisable( GL_BLEND );
		} else if( dProgress > 0.0 ) {

			glEnable( GL_BLEND );
			glBlendEquation( GL_FUNC_ADD );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

			double dOpacity = 0.2 + dProgress*0.8;
			glBegin(GL_POLYGON);
			glColor4f( fR, fG, fB, dOpacity );
		    for( std::vector<Vertex>::iterator ve = facet.mVertices.begin() ; ve != facet.mVertices.end() ; ++ve ) {
				glVertex3f( ve->mX, ve->mY, ve->mZ );
		    }
			glEnd();
			glDisable( GL_BLEND );
		}

		if( dProgress > 0.0 ) {
			return;
		}

		// Ribs only
		if( selected ) {
			glColor3f( 1.0, 1.0, 1.0 );
		} else if( touched ) {
			//glLineStipple( 1, 0x8888 );
			//glEnable( GL_LINE_STIPPLE );
			glColor3f( 0.75, 0.75, 0.75 );
		} else {
			glColor3f( 0.5, 0.5, 0.5 );     
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

		//if( touched ) {
		//	glDisable( GL_LINE_STIPPLE );
		//}
	
	}

	static void displayRib( double fX1, double fY1, double fZ1, double fX2, double fY2, double fZ2 ) {
		glBegin(GL_LINES);
		glEnable( GL_LINE_SMOOTH );
		glVertex3f( fX1,fY1,fZ1 );
		glVertex3f( fX2,fY2,fZ2 );
		glEnd();
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

	static bool getNorm( Facet& facet, double& kX, double& kY, double&kZ ) {
		double v1X = facet.mVertices[1].mX -facet.mVertices[0].mX;
		double v1Y = facet.mVertices[1].mY -facet.mVertices[0].mY;
		double v1Z = facet.mVertices[1].mZ -facet.mVertices[0].mZ;

		double v2X, v2Y, v2Z;
		for( int i = 2 ; i < facet.mVertices.size() ; i++ ) {
			double v2X = facet.mVertices[i].mX - facet.mVertices[i-1].mX;
			double v2Y = facet.mVertices[i].mY - facet.mVertices[i-1].mY;
			double v2Z = facet.mVertices[i].mZ - facet.mVertices[i-1].mZ;

			bool brX=false, brY=false, brZ=false;
			double rX, rY, rZ;
			if( fabs( v2X ) > 1e-10 ) {
				rX = v1X / v2X;
				brX = true;
			} 
			if( fabs( v2Y ) > 1e-10 ) {
				rY = v1Y / v2Y;
				brY = true;
			} 
			if( fabs( v2Z ) > 1e-10 ) {
				rZ = v1Z / v2Z;
				brZ = true;
			} 

			bool calc = false;				
			if( brX && brY && brZ ) {
				if( fabs( brX-brY) < 1e-10 && fabs(brX-brZ) < 1e-10 ) {
					continue; 
				} else {
					calc = true;
				}
			} else {
				if( !brX && (!(fabs(brY) < 1e-10) || !(fabs(brZ) < 1e-10) ) ) {
					calc = true;
				} else if ( !brY && (!(fabs(brX) < 1e-10) || !(fabs(brZ) < 1e-10) ) ) {
					calc = true;
				} else if ( !brZ && (!(fabs(brX) < 1e-10) || !(fabs(brY) < 1e-10) ) ) {
					calc = true;
				} 
			}

			if( calc ) {
				calcNorm( v1X, v1Y, v1Z, v2X, v2Y, v2Z, kX, kY, kZ );
				return true;
			}
		}
		return false;
	}

	static void calcNorm( double v1X, double v1Y, double v1Z, double v2X, double v2Y, double v2Z, double& kX, double& kY, double& kZ ) {

		kX = v1Y*v2Z - v1Z*v2Y;
		kY = v1X*v2Z - v1Z*v2X;
		kZ = v1X*v2Y - v1Y*v2X;

	}

/*
			// Getting the min and the max Ys
			double minY, maxY;
			for( int i = 0 ; i < nVertices ; i++ ) {
				if( i == 0 ) {
					minY = maxY = facet.mVertices[i];
				} else {
					if( facet.mVertices[i] < minY ) {
						minY = facet.mVertices[i];
					} else if ( facet.mVertices[i] > maxY ) {
						maxY = facet.mVertices[i];
					}
				}
			}

*/
}



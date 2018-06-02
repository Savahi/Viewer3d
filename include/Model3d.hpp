#pragma once 


#include <vector>
#include <ctime>
#include <stdlib.h>
/*
#include <iostream>
#include <string>
*/
namespace Spider3d {

	class Vertex3d {
		public:
			float mX, mY, mZ;
			Vertex3d( void ) {
				; // std::cout << "Constructor for Vertex3d\n"; 
			}
			Vertex3d( float x, float y, float z ) : mX(x), mY(y), mZ(z) {
				; // std::cout << "Constructor for Vertex3d\n"; 
			}
			~Vertex3d(){
				; // std::cout << "Destructor for Vertex3d\n"; 
			}
			int setXYZ( float x, float y, float z ) {
				this->mX = x;
				this->mY = y;
				this->mZ = z;
				return 0;
			}
	};

	class Facet3d {
		public:
			std::vector<Vertex3d> mVertices;

			int add( Vertex3d& vertex ) {
				this->mVertices.push_back( verte );
				return 0;
			}

			int clear( void ) {
				this->mVertices.clear();
			}

			Facet3d() {
				; // std::cout << "Constructor for Facet3d\n";
			}
			~Facet3d() {
				; // std::cout << "Destructor for Facet3d\n";
			}
	};

	class Object3d {
		public:
			std::vector<Facet3d> mFacets; 

			int add( Facet3d& facet ) {
				this->mFacets.push_back( facet );
				return 0;
			}

			Object3d() {
				; // std::cout << "Constructor for Object\n";
			}

			~Object3d() {
				; // std::cout << "Destructor for Object\n";
			}
	};

	class Model3d {
		public:
			std::vector<Object3d> mObjects; 

			int add( Object3d& object ) {
				this->mObjects.push_back( object );
				return 0;
			}

			bool mAxis;
			float mGrid;
			float mXMargin, mYMargin, mZMargin;

			Model3d() {
				; // std::cout << "Constructor for Model3d\n";
			}

			~Model3d() {
				; // std::cout << "Destructor for Model3d\n";
			}

			unsigned int objectsNumber( void ) {
				return this->mObjects.size(); 
			}

			void display( int argc, char* argv[] );
			
			static void display( Model3d& model, int argc, char* argv[] );
	};

}
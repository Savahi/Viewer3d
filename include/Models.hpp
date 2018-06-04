#pragma once 


#include <vector>
#include <ctime>
#include <stdlib.h>
/*
#include <iostream>
#include <string>
*/
namespace Spider3d {

	class Vertex {
		public:
			float mX, mY, mZ;
			Vertex( void ) {
				; // std::cout << "Constructor for Vertex\n"; 
			}
			Vertex( float x, float y, float z ) : mX(x), mY(y), mZ(z) {
				; // std::cout << "Constructor for Vertex\n"; 
			}
			~Vertex(){
				; // std::cout << "Destructor for Vertex\n"; 
			}
			int setXYZ( float x, float y, float z ) {
				this->mX = x;
				this->mY = y;
				this->mZ = z;
				return 0;
			}
	};

	class Facet {
		public:
			std::vector<Vertex> mVertices;

			int add( Vertex& vertex ) {
				this->mVertices.push_back( verte );
				return 0;
			}

			int clear( void ) {
				this->mVertices.clear();
			}

			Facet() {
				; // std::cout << "Constructor for Facet\n";
			}
			~Facet() {
				; // std::cout << "Destructor for Facet\n";
			}
	};

	class Model {
		public:
			std::vector<Facet> mFacets; 
			std::string mCode;
			std::string mName;

			int add( Facet& facet ) {
				this->mFacets.push_back( facet );
				return 0;
			}

			void setCode( std::string code ) {
				this->mCode = code;
			}
			void setCode( char *cpCode ) {
				this->mCode = std::string( cpCode );
			}
			
			void setName( std::string name ) {
				this->mName = name;
			}
			void setName( char *cpName ) {
				this->mName = std::string( cpName );
			}

			Model() {
				; // std::cout << "Constructor for Model\n";
			}

			Model( std::string code ) : mCode(code) {
				; // std::cout << "Constructor for Model\n";
			}

			~Model() {
				; // std::cout << "Destructor for Model\n";
			}
	};

	class Models {
		public:
			std::vector<Object> mModels; 

			bool mAxis;
			float mGrid;
			float mXMargin, mYMargin, mZMargin;

			int add( Model& model ) {
				this->mModels.push_back( model );
				return 0;
			}


			Models() {
				; // std::cout << "Constructor for Model\n";
			}

			~Models() {
				; // std::cout << "Destructor for Model\n";
			}

			unsigned int totalNumber( void ) {
				return this->mModels.size(); 
			}

			void display( int argc, char* argv[] );
			
			static void display( Model& model, int argc, char* argv[] );
	};

}




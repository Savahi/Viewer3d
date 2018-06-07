#pragma once 

#include <string>
#include <vector>
#include <ctime>
#include <map>
#include "helpers.hpp"
#include "operations.hpp"

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
				this->mVertices.push_back( vertex );
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
			std::string sCode;
			std::string sName;
			std::string sNotes;
			bool bSelected;

			std::map<time_t,Operation*> operations;

			int add( Facet& facet ) {
				this->mFacets.push_back( facet );
				return 0;
			}

			void setCode( std::string& code ) { this->sCode = code; }
			void setCode( char *cpCode ) { this->sCode = std::string( cpCode ); }

			void setName( std::string& name ) { this->sName = name; }
			void setName( char *cpName ) { this->sName = std::string( cpName ); }

			void setNotes( std::string& notes ) { this->sNotes = notes; }
			void setNotes( char *cpNotes ) { this->sNotes = std::string( cpNotes ); }

			unsigned int numFacets( void ) {
				return this->mFacets.size(); 
			}

			Model() : bSelected(false) {
				; // std::cout << "Constructor for Model\n";
			}

			Model( std::string code ) : sCode(code), bSelected(false) {
				; // std::cout << "Constructor for Model\n";
			}

			~Model() {
				; // std::cout << "Destructor for Model\n";
			}
	};

	class Models {
		public:
			std::vector<Model> mModels; 

			bool mAxis;
			float mGrid;
			float mXMargin, mYMargin, mZMargin;

			Models() {
				; // std::cout << "Constructor for Model\n";
			}

			~Models() {
				; // std::cout << "Destructor for Model\n";
			}
			int add( Model& model ) {
				this->mModels.push_back( model );
				return 0;
			}

			unsigned int number( void ) {
				return this->mModels.size(); 
			}

	};

}




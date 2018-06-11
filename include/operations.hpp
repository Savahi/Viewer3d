#pragma once

#include <vector>
#include <string>
#include <ctime>
#include "helpers.hpp"
#include "optypes.hpp"
	
namespace Spider3d {

	class Operation {
		public:

			tm tmActualStart, tmActualFinish, tmAsapStart, tmAsapFinish;
			time_t tActualStart, tActualFinish, tAsapStart, tAsapFinish;
			bool bDatesInitialized;

			std::string sCode;
			std::string sName;
			std::string sType;
			std::string sModelCode;

			OpType *opType;

			void setModelCode( std::string& sCode ) { this->sModelCode = sCode; }
			void setModelCode( char *cpCode ) { this->sModelCode = std::string(cpCode); }

			void setCode( std::string& sCode ) { this->sCode = sCode; }
			void setCode( char *cpCode ) { this->sCode = std::string( cpCode ); }

			void setName( std::string& sName ) { this->sName = sName; }
			void setName( char *cpName ) { this->sName = std::string( cpName ); }

			void setType( std::string& sType ) { this->sType = sType; }
			void setType( char *cpType ) { this->sType = std::string( cpType ); }

			Operation() : bDatesInitialized(false), opType(NULL) {
				; // std::cout << "Constructor for Operation\n";
			}

			~Operation() {
				; // std::cout << "Destructor for Operation\n";
			}
	};

	class Operations {
			public:
				std::vector<Operation> mOperations;

				int add( Operation& operation ) {
					this->mOperations.push_back( operation );
					return 0;
				}

				int clear( void ) {
					this->mOperations.clear();
				}

				Operations() {
					; // std::cout << "Constructor for Operations\n";
				}
				~Operations() {
					; // std::cout << "Destructor for Operations\n";
				}

				unsigned int number( void ) {
					return this->mOperations.size(); 
				}
		};

	}
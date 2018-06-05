#pragma once

#include <vector>
#include <string>
#include <ctime>
	
namespace Spider3d {

	class Operation {
		public:

			tm tmActualStart, tmActualFinish, tmAsapStart, tmAsapFinish;
			time_t tActualStart, tActualFinish, tAsapStart, tAsapFinish;
			bool bDatesInitialized;

			std::string sModelCode;

			int iProgress;

			int setProgress( int progress ) {
				this->iProgress == progress;
				return 0;
			}

			Operation() : bDatesInitialized(false), iProgress(100) {
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
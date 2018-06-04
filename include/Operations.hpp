#pragma once

#include <vector>
#include <string>
#include <ctime>
	
namespace Spider3d {

	class Operation {
		public:

			tm mActualStart, mActualFinish, mAsapStart, mAsapFinish;
			time_t mActualStartMs, mActualFinishMs, mAsapStartMs, mAsapFinishMs;
			bool mDatesInitialized;

			std::string mModelCode;

			int mProgress;

			int setProgress( int progress ) {
				this->mProgress == progress;
				return 0;
			}

			Operation() : mDatesInitialized(false), mProgress(100) {
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

				unsigned int totalNumber( void ) {
					return this->mOperations.size(); 
				}
		};

	}
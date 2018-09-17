#pragma once
#include "stdafx.h"

namespace Spider3d {

	class GanttOperation {
		public:
	    std::vector<std::string> fields;    
		
		GanttOperation() { ; }
		~GanttOperation() { ; }
	};

	class Gantt {
		public:
	    std::vector<std::string> fieldsNames;
	    std::map<std::string,int> fieldsPositions;
	    std::map<std::string,std::string> fieldsTitles;
	    std::map<std::string,long int> fieldsFlags;

	    std::vector<GanttOperation> operations;

		Gantt() { ; }
		~Gantt() { ; }

		int add( GanttOperation& ga ) {
			this->operations.push_back( ga );
			return 0;
		}

		int number( void ) {
			return this->operations.size();
		}

		int clear( void ) {
			this->operations.clear();
		}

	};
}

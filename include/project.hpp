#pragma once

#include <vector>
#include <string>
#include "helpers.hpp"
	
namespace Spider3d {

	class Project {
		public:
			std::string sCode;
			std::string sName;

			int iProjVer;
			bool bProjVer;

			std::string sCurTime;

			Project() {;}
			~Project() {;}
	};
}
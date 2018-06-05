#pragma once
#include "models.hpp" 
#include "operations.hpp"

namespace Spider3d {

	int loadModels( Models& models, const char *cpFile );

	int loadOperations( Operations& operations, const char *cpFile );

	void display( Models& models, Operations& operations, int argc, char *argv[] );

}
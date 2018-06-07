#pragma once
#include "models.hpp" 
#include "optypes.hpp"
#include "operations.hpp"

namespace Spider3d {

	int loadModels( Models& models, const char *cpFile );

	int loadOperations( Operations& operations, const char *cpFile );

	int loadOpTypes( OpTypes& opTypes, const char *cpFile );

	void display( Models& models, Operations& operations, OpTypes& opTypes, int argc, char *argv[] );

}
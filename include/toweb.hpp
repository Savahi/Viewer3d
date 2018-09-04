#pragma once
#include "operations.hpp"
#include "oplinks.hpp"
#include "resources.hpp"
#include "opresassignments.hpp"

namespace Spider3d {

	int loadOperations( Operations& operations, const char *cpFile );

	int loadOpLinks( OpLinks& opLinks, const char *cpFile );

	int loadResources( Resources& resources, const char *cpFile );

	int loadOpResAssignments( OpResAssignments& opResAssignments, const char *cpFile );

}
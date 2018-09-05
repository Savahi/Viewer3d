#pragma once
#include "operations.hpp"
#include "oplinks.hpp"
#include "resources.hpp"
#include "opresassignments.hpp"
#include "costs.hpp"
#include "opcosts.hpp"
#include "project.hpp"

namespace Spider3d {

	int loadProject( Project& project, const char *cpFile );

	int loadOperations( Operations& operations, const char *cpFile );

	int loadOpLinks( OpLinks& opLinks, const char *cpFile );

	int loadResources( Resources& resources, const char *cpFile );

	int loadOpResAssignments( OpResAssignments& opResAssignments, const char *cpFile );

	int loadCosts( Costs& costs, const char *cpFile );

    int loadOpCosts( OpCosts& opCosts, const char *cpFile );
}
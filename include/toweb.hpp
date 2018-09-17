#pragma once
#include "stdafx.h"

namespace Spider3d {

	int loadProject( Project& project, const char *cpFile );

	int loadGantt( Gantt& gantt, const char *cpFile );

	int loadOpLinks( OpLinks& opLinks, const char *cpFile );

	int loadResources( Resources& resources, const char *cpFile );

	int loadOpResAssignments( OpResAssignments& opResAssignments, const char *cpFile );

	int loadCosts( Costs& costs, const char *cpFile );

    int loadOpCosts( OpCosts& opCosts, const char *cpFile );
}
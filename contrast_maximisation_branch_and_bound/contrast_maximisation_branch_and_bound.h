#ifndef CONTRAST_MAXIMISATION_BRANCH_AND_BOUND_H
#define CONTRAST_MAXIMISATION_BRANCH_AND_BOUND_H

#include <chrono>
#include <ctime>
#include <vector>
#include <string>

#include "../classes/Event_Data.h"
#include "../classes/Domain.h"
#include "../classes/Camera_Parameters.h"
#include "../classes/State.h"
#include "../classes/Priority_Queue.h"

#include "upper_bound.h"
#include "contrast.h"
#include "warping.h"

using namespace std;

// Perform contrast maximisation on the event data to find the velocity within
// the velocity domain that maximises contrast.
double contrast_maximisation_branch_and_bound(Event_Data* event_data, Domain velocity_domain, Camera_Parameters cp, double warp_reference_time, double r, vector<string>& results_vector);

#endif

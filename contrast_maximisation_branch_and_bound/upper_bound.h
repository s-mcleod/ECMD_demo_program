#ifndef UPPER_BOUND_H
#define UPPER_BOUND_H

#include <algorithm>

#include "../classes/Event_Data.h"
#include "../classes/Camera_Parameters.h"
#include "../classes/Domain.h"

#include "../helper_functions/matrix_helper_functions.h"
#include "../helper_functions/general_helper_functions.h"

#include "warping.h"
#include "contrast.h"

using namespace std;

// Find the upper bound contrast of all events warped under the velocity domain.
double upper_bound(Domain domain, Event_Data* event_data, Camera_Parameters cp, double ref_time);

#endif

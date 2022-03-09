#ifndef REMOVE_EVENTS_LYING_OUTSIDE_IMAGE_PLANE_H
#define REMOVE_EVENTS_LYING_OUTSIDE_IMAGE_PLANE_H

#include <iostream>

#include "../contrast_maximisation_branch_and_bound/warping.h"
#include "../classes/Event_Data.h"

using namespace std;

void remove_polarity_stream(Event_Data* event_data, int pol_stream_to_keep);

#endif

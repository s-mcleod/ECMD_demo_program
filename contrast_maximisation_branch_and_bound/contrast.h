#ifndef CONTRAST_H
#define CONTRAST_H

#include <math.h>

#include "../classes/Event_Data.h"
#include "../classes/Camera_Parameters.h"
#include "../helper_functions/matrix_helper_functions.h"
#include "warping.h"

using namespace std;

// Calculate the contrast of an image plane.
double contrast_of_image_plane(Event_Data* event_data, double* image_plane);

// Calculate the upper bound contrast of an image plane.
double contrast_of_upper_bound_image_plane(Event_Data* event_data, double* image_plane, int number_of_lines_warped_out_of_bounds);

// A function which first warps all events then calculates the contrast of the
// image plane.
double image_plane_contrast_produced_by_velocity(Event_Data* event_data, double v_best, Camera_Parameters cp, double ref_time);

#endif

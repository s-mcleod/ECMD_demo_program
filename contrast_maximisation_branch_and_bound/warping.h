#ifndef WARPING_H
#define WARPING_H

#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include <algorithm>

#include "../classes/Camera_Parameters.h"
#include "../classes/Event_Data.h"
#include "../helper_functions/matrix_helper_functions.h"

// The rule applied to coordinate -> pixel location.
int pixel_location(double x);

// Highlights coordinate on the image plane.
int set_pixel_coordinates(double r, double c, double* image_plane, Event_Data* event_data, bool print = false);

// Returns the warped coordinates of an event at a reference time.
// Note, warp_image_plane should only be true when you are calculating the upper
// bound and we want the coordinates to be within the image plane.
double* warped_coordinates(double event_row, double event_col, double event_delta_time, double* foe, double ref_time, Camera_Parameters cp, double velocity, bool warp_within_image_plane = false, int image_height = 0, int image_width = 0);

// Warp events under velocity to the image plane.
// Returns the number of events that would be warped outside the image plane.
int warp_events_to_image_plane(Event_Data* event_data, double* image_plane, Camera_Parameters cp, double velocity, double ref_time);

// A function which warps all events according to a velocity, to respective
// image plane coordinates.
void warp(Event_Data* event_data, double velocity, Camera_Parameters cp, double ref_time, double* image_plane);

// Writes the warped coorinate locations to a csv file.
void write_warped_events_to_csv(Event_Data* event_data, double ref_time, Camera_Parameters cp, double velocity, string filename);


#endif

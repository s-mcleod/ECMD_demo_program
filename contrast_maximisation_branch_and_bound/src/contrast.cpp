#ifndef CONTRAST_CPP
#define CONTRAST_CPP

#include "../contrast.h"

// Calculate the contrast of an image plane.
double contrast_of_image_plane(Event_Data* event_data, double* image_plane){

  // Find mean contrast value of image plane.
  double mean_contrast = 0;
  double number_of_pixels = event_data->image_height *  event_data->image_width;
  for (int i = 0; i < number_of_pixels; i ++){
    mean_contrast += image_plane[i];
  }
  mean_contrast = mean_contrast/number_of_pixels;

  // Find contrast over entire image.
  double contrast = 0;
  for (int i = 0; i < number_of_pixels; i ++){
    contrast += (image_plane[i]-mean_contrast)*(image_plane[i]-mean_contrast);
  }

  return contrast/number_of_pixels;
}

// Calculate the upper bound contrast of an image plane.
double contrast_of_upper_bound_image_plane(Event_Data* event_data, double* image_plane, int number_of_lines_warped_out_of_bounds){

  int number_of_events = event_data->number_of_events;
  int number_of_pixels = event_data->image_height *  event_data->image_width;

  double lower_bound = ((number_of_events-number_of_lines_warped_out_of_bounds)/number_of_pixels)*((number_of_events-number_of_lines_warped_out_of_bounds)/number_of_pixels);

  // Find the contrast of the image plane.
  double upper_bound = 0;

  for (int i = 0; i < number_of_pixels; i ++){
    upper_bound += (image_plane[i])*(image_plane[i]);
  }
  upper_bound = upper_bound/number_of_pixels;

  return (upper_bound-lower_bound);
}

// A function which first warps all events then calculates the contrast of the image plane.
double image_plane_contrast_produced_by_velocity(Event_Data* event_data, double v_best, Camera_Parameters cp, double ref_time){
  double* image_plane = new double[event_data->image_height*event_data->image_width];
  zero_matrix(image_plane, event_data->image_height, event_data->image_width);

  // Warp events to image plane.
  warp(event_data, v_best, cp, ref_time, image_plane);

  // Find contrast of warped event image plane.
  double contrast = contrast_of_image_plane(event_data, image_plane);

  delete[] image_plane;
  return contrast;
}

#endif

#ifndef UPPER_BOUND_CPP
#define UPPER_BOUND_CPP

#include "../upper_bound.h"

double upper_bound(Domain domain, Event_Data* event_data, Camera_Parameters cp, double ref_time){

  // Initialise variables.
  int img_rows = event_data->image_height;
  int img_cols = event_data->image_width;
  int number_of_event_parameters = event_data->number_of_event_channels;
  int number_of_events = event_data->number_of_events;
  double* foe = event_data->get_foe();
  double* events = event_data->get_events();

  // Initialise image plane.
  double* image_plane = new double[img_rows*img_cols];
  zero_matrix(image_plane, img_rows, img_cols);

  int number_of_lines_warped_out_of_bounds = 0;

  // Warp every event under every velocity in the domain.
  for (int e = 0; e < number_of_events; e++){

    // Event details.
    double event_row = events[e*number_of_event_parameters+event_data->row_location];
    double event_col = events[e*number_of_event_parameters+event_data->column_location];
    double event_time = events[e*number_of_event_parameters+event_data->time_location] - event_data->start_time;

    // Find row and col distance from event and FOE.
    double delta_r = event_row - foe[0];
    double delta_c = event_col - foe[1];

    // Determine which upper bound coordinates will be closer to the FOE and further away from the FOE.
    double* closest_coordinates;
    double* furthest_coordinates;

    // Determine if both warped coordinates were out of bounds or not - default to false.
    int coordinates_warped_out_of_bounds = 0;

    // Coordinates of the warped event that is closest to the FOE.
    furthest_coordinates = warped_coordinates(event_row, event_col, event_time, foe, ref_time, cp, domain.left_domain, true, img_rows, img_cols);

    // Coordinates of the warped event that is furthest away from the FOE.
    closest_coordinates = warped_coordinates(event_row, event_col, event_time, foe, ref_time, cp, domain.right_domain, true, img_rows, img_cols);

    // Last value returned by warped_coordinates are 1 if they were warped out of bounds of the image plane.
    coordinates_warped_out_of_bounds += closest_coordinates[2] + furthest_coordinates[2];

    if (coordinates_warped_out_of_bounds > 0){
      number_of_lines_warped_out_of_bounds ++;
    }

    // If both the closest coordinates and furthest coordinates were out of bounds of the image plane, then we want to ignore this event.
    if (coordinates_warped_out_of_bounds < 2){
      // Calculating the slope of the line between the closest and furthest
      // points (rise/run).
      double row_coordinate_difference = (closest_coordinates[0])-(furthest_coordinates[0]);
      double col_coordinate_difference = (closest_coordinates[1])-(furthest_coordinates[1]);


      // Find the left and right most bounds on the image plane.
      int left_coordinate = pixel_location(furthest_coordinates[1]);
      int right_coordinate = pixel_location(closest_coordinates[1]);
      if (col_coordinate_difference < 0) {
        left_coordinate = pixel_location(closest_coordinates[1]);
        right_coordinate = pixel_location(furthest_coordinates[1]);
      }
      //Find the top and bottom most bounds on the image plane.
      int top_coordinate = pixel_location(furthest_coordinates[0]);
      int bottom_coordinate = pixel_location(closest_coordinates[0]);
      if (row_coordinate_difference < 0) {
        top_coordinate = pixel_location(closest_coordinates[0]);
        bottom_coordinate = pixel_location(furthest_coordinates[0]);
      }

      if (top_coordinate == bottom_coordinate)
        row_coordinate_difference = 0;
      if (left_coordinate == right_coordinate)
        col_coordinate_difference = 0;


      // If the closest and furthest row or column coordinates are the same,
      // plot a horisontal or vertical line (respectivley). Else, draw a diagonal
      // line.
      if (row_coordinate_difference == 0 && col_coordinate_difference == 0){
        set_pixel_coordinates(closest_coordinates[0], closest_coordinates[1], image_plane, event_data);
      }
      else if (row_coordinate_difference == 0){
        for (int col = left_coordinate; col <= right_coordinate; col++){
          set_pixel_coordinates(closest_coordinates[0], col, image_plane, event_data);
        }

      }
      else if (col_coordinate_difference == 0){
        for (int row = top_coordinate; row <= bottom_coordinate; row++){
          set_pixel_coordinates(row, closest_coordinates[1], image_plane, event_data);
        }
      }
      else {
        // m - slope of line.
        double m = (closest_coordinates[0]-furthest_coordinates[0])/(closest_coordinates[1]-furthest_coordinates[1]);
        // c - row offset of line.
        double c = -1*m*(closest_coordinates[1])+closest_coordinates[0];

        // Loop through the largest number of coordinates first (either row or column).
        // Keep track of what pixel coordinates were added too so that we don't double up in the second loop.
        bool col_has_more_coordinates = false;
        int number_of_coordinates = abs(pixel_location(furthest_coordinates[0]) - pixel_location(closest_coordinates[0]))+1;

        // If there are the same number of rows and columns, it's just the pixels in the centre that need to be highlighted.
        if (abs(pixel_location(furthest_coordinates[1]) - pixel_location(closest_coordinates[1]))+1 == number_of_coordinates){
          for (int col = left_coordinate; col <= right_coordinate; col ++){
            double row = m*col+c;
            set_pixel_coordinates(row, col, image_plane, event_data);
          }
        }
        else {
          if (abs(pixel_location(furthest_coordinates[1]) - pixel_location(closest_coordinates[1]))+1 > number_of_coordinates){
            col_has_more_coordinates = true;
            number_of_coordinates = abs(pixel_location(furthest_coordinates[1]) - pixel_location(closest_coordinates[1]))+1;
          }

          // Padding the intersecting coordinates (above and below or right and left).
          // Padding the intersecting coordinates (above and below or right and left).
          number_of_coordinates = number_of_coordinates*2;

          if (col_has_more_coordinates){
            for (int col = left_coordinate; col <= right_coordinate; col ++){
              double row = m*col+c;
              double c_row = ceil(row);
              double f_row = floor(row);
              if (c_row >= top_coordinate && c_row <= bottom_coordinate)
                set_pixel_coordinates(c_row, col, image_plane, event_data);
              if (f_row >= top_coordinate && f_row <= bottom_coordinate)
                set_pixel_coordinates(f_row, col, image_plane, event_data);
            }
          }
          else {
            for (int row = top_coordinate; row <= bottom_coordinate; row ++){
              double col = (row-c)/m;
              double c_col = ceil(col);
              double f_col = floor(col);
              if (c_col >= left_coordinate && c_col <= right_coordinate)
                set_pixel_coordinates(row, c_col, image_plane, event_data);
              if (f_col >= left_coordinate && f_col <= right_coordinate)
                set_pixel_coordinates(row, f_col, image_plane, event_data);
            }
          }
        }
      }
    }
    delete[] closest_coordinates;
    delete[] furthest_coordinates;
  }

  double contrast;
  contrast = contrast_of_upper_bound_image_plane(event_data, image_plane,number_of_lines_warped_out_of_bounds);

  delete[] image_plane;

  return contrast;
}
#endif

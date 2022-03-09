#ifndef WARPING_CPP
#define WARPING_CPP

#include "../warping.h"

// The rule applied to coordinate -> pixel location.
int pixel_location(double x){
  return (int)round(roundf(x * 100) / 100);
}

int set_pixel_coordinates(double r, double c, double* image_plane, Event_Data* event_data, bool print){
  int pixel_row = pixel_location(r);
  int pixel_col = pixel_location(c);

  if (pixel_row >= 0 && pixel_row < event_data->image_height){
    if (pixel_col >= 0 && pixel_col < event_data->image_width){
      image_plane[event_data->image_width*pixel_row + pixel_col] ++;
      return 0;
    }
    else {
      return 1;
    }
  }
  else {
    return 1;
  }
}

// Returns the warped coordinates of an event at a reference time.
// Note, warp_image_plane should only be true when you are calculating the upper bound and we want the coordinates to be within the image plane.
double* warped_coordinates(double event_row, double event_col, double event_delta_time, double* foe, double ref_time, Camera_Parameters cp, double velocity, bool warp_within_image_plane, int image_height, int image_width){

  double delta_r = event_row - foe[0];
  double delta_c = event_col - foe[1];

  double* coordinates = new double[2];
  coordinates[0] = -1;
  coordinates[1] = -1;

  double integral_denominator_ref_time = velocity*ref_time+cp.Z0;
  double integral_denominator_event_time = velocity*event_delta_time+cp.Z0;
  if (integral_denominator_ref_time == 0){
    integral_denominator_ref_time = 0.00000000000000000000000001;
  }
  if (integral_denominator_event_time == 0){
    integral_denominator_event_time = 0.00000000000000000000000001;
  }

  // Real world event coordinates.
  double real_x = delta_r*(integral_denominator_event_time)/cp.f;
  // cout<<"real_x: "<<real_x<<endl;
  double real_y = delta_c*(integral_denominator_event_time)/cp.f;

  // Integral.
  double wx_i = (cp.f*real_x)/(integral_denominator_event_time);
  // cout<<"wx_i: "<<wx_i<<endl;
  double wx_f = (cp.f*real_x)/(integral_denominator_ref_time);
  // cout<<"wx_f: "<<wx_f<<endl;
  double wx = wx_f - wx_i;
  // cout<<"wx: "<<wx<<endl;

  double wy_i = (cp.f*real_y)/(integral_denominator_event_time);
  double wy_f = (cp.f*real_y)/(integral_denominator_ref_time);
  double wy = wy_f - wy_i;

  // Returns the acutal image coordinates on the image plane.
  coordinates[0] = foe[0] + delta_r + wx;
  coordinates[1] = foe[1] + delta_c + wy;


  // Find the intersection of the line from the foe to the coordinates with the image plane and update coordinates.
  if (warp_within_image_plane){
    // Array that contains the new warped coordinates.
    // The third entry is a 1 if we had to adjust the coordinates because they were out of bounds, and 0 if there were no changes.
    double* warped_out_of_bounds_coordinates = new double[3];
    warped_out_of_bounds_coordinates[0] = coordinates[0];
    warped_out_of_bounds_coordinates[1] = coordinates[0];
    warped_out_of_bounds_coordinates[2] = 0;
    // See if either of the coordinates exceed the image plane bounds.
    if (coordinates[0] < 0 || coordinates[0] >= image_height || coordinates[1] < 0 || coordinates[1] >= image_width){
      warped_out_of_bounds_coordinates[2] = 1;
      // Slope of line between coordinates and foe.
      double m = (coordinates[0]-foe[0])/(coordinates[1]-foe[1]);

      // If there is no slope ...
      if (coordinates[1]-foe[1] == 0){
        if (coordinates[0] < 0){
          coordinates[0] = 0;
        }
        else if (coordinates[0] >= image_height){
          coordinates[0] = image_height-1;
        }
      }
      else if (coordinates[0]-foe[0] == 0){
        if (coordinates[1] < 0){
          coordinates[1] = 0;
        }
        else if (coordinates[1] >= image_width){
          coordinates[1] = image_width-1;
        }
      }
      // Find the longest distance travelled in either the row or column direction.
      else if (abs(delta_r + wx) > abs(delta_c + wy)){
        // Set the row to equal the image bound it intersects.
        if (coordinates[0] < 0){
          coordinates[0] = 0;
        }
        else if (coordinates[0] >= image_height){
          coordinates[0] = image_height-1;
        }
        // New column coordinates.
        coordinates[1] = (coordinates[0]-foe[0]+m*foe[1])/m;
      }
      else if (abs(delta_r + wx) < abs(delta_c + wy)){
        // Set the column to equal the image bound it intersects.
        if (coordinates[1] < 0){
          coordinates[1] = 0;
        }
        else if (coordinates[1] >= image_width){
          coordinates[1] = image_width-1;
        }
        // New row coordinates.
        coordinates[0] = m*(coordinates[1]-foe[1])+foe[0];
      }
      else{
        if (coordinates[0] < 0){
          coordinates[0] = 0;
        }
        else if (coordinates[0] >= image_height){
          coordinates[0] = image_height-1;
        }
        if (coordinates[1] < 0){
          coordinates[1] = 0;
        }
        else if (coordinates[1] >= image_width){
          coordinates[1] = image_width-1;
        }
      }
    }
    warped_out_of_bounds_coordinates[0] = coordinates[0];
    warped_out_of_bounds_coordinates[1] = coordinates[1];

    delete[] coordinates;
    return warped_out_of_bounds_coordinates;
  }
  return coordinates;
}

void write_warped_events_to_csv(Event_Data* event_data, double ref_time, Camera_Parameters cp, double velocity, string filename){
  ofstream myfile;
  myfile.open (filename);
  for (int e = 0; e < event_data->number_of_events; e++){
    double event_row = event_data->get_events()[e*event_data->number_of_event_channels + event_data->row_location];
    double event_col = event_data->get_events()[e*event_data->number_of_event_channels + event_data->column_location];
    double event_delta_time = event_data->get_events()[e*event_data->number_of_event_channels + event_data->time_location] - event_data->start_time;

    double* coordinates = warped_coordinates(event_row, event_col, event_delta_time, event_data->get_foe(), ref_time, cp, velocity);
    myfile<<coordinates[0]<<","<<coordinates[1]<<endl;
  }
  myfile.close();
}

// Warp events under velocity to the image plane.
// Returns the number of events that would be warped outside the image plane.
int warp_events_to_image_plane(Event_Data* event_data, double* image_plane, Camera_Parameters cp, double velocity, double ref_time){
  double delta_t = event_data->end_time - event_data->start_time;
  // Warp coordinates if valid velocity.
  int number_of_events_lying_outside_image_plane = 0;
  if (abs(velocity*delta_t) < cp.Z0 && velocity*delta_t < 0) {
    for (int e = 0; e < event_data->number_of_events; e++){
      double event_row = event_data->get_events()[e*event_data->number_of_event_channels + event_data->row_location];
      double event_col = event_data->get_events()[e*event_data->number_of_event_channels + event_data->column_location];
      double event_delta_time = event_data->get_events()[e*event_data->number_of_event_channels + event_data->time_location] - event_data->start_time;

      double* coordinates = warped_coordinates(event_row, event_col, event_delta_time, event_data->get_foe(), ref_time, cp, velocity);

      // Highlight events on event image plane.
      number_of_events_lying_outside_image_plane += set_pixel_coordinates(coordinates[0], coordinates[1], image_plane, event_data);
      delete[] coordinates;
    }
  }
  return number_of_events_lying_outside_image_plane;
}


// A function which warps all events according to an velocity, to respective
// image plane coordinates.
void warp(Event_Data* event_data, double velocity, Camera_Parameters cp, double ref_time, double* image_plane){
  zero_matrix(image_plane, event_data->image_height, event_data->image_width);
  for (int e = 0; e < event_data->number_of_events; e++){
    // cout<<event_data->image_height<<" x "<<event_data->image_width<<endl;
    // Event details.
    double event_row = event_data->get_events()[e*event_data->number_of_event_channels + event_data->row_location];
    double event_col = event_data->get_events()[e*event_data->number_of_event_channels + event_data->column_location];
    double event_delta_time = event_data->get_events()[e*event_data->number_of_event_channels + event_data->time_location] - event_data->start_time;

    // Find the warped coordinates of each event.
    double* coordinates = warped_coordinates(event_row, event_col, event_delta_time, event_data->get_foe(), ref_time, cp, velocity);
    set_pixel_coordinates(coordinates[0], coordinates[1], image_plane, event_data, true);

    delete[] coordinates;
  }
}
#endif

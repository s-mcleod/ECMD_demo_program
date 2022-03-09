#ifndef REMOVE_EVENTS_LYING_OUTSIDE_IMAGE_PLANE_CPP
#define REMOVE_EVENTS_LYING_OUTSIDE_IMAGE_PLANE_CPP

#include "../remove_events.h"

// Remove a polarity stream from the event data.
void remove_polarity_stream(Event_Data* event_data, int pol_stream_to_keep){
  vector<bool> valid_events_vector;
  int number_of_valid_events = 0;

  // Find the number of valid events that don't lie outside the image plane.
  double * events = event_data->get_events();
  for (int e = 0; e < event_data->number_of_events; e++){
    if (events[e*event_data->number_of_event_channels + event_data->polarity_location] == pol_stream_to_keep){
      valid_events_vector.push_back(true);
      number_of_valid_events ++;
    }
    else {
      valid_events_vector.push_back(false);
    }
  }

  // Create a new event list of events that weren't triggerend outside the image plane.
  double* valid_events = new double[number_of_valid_events*event_data->number_of_event_channels];
  int i = 0;
  for (int e = 0; e < event_data->number_of_events; e++){
    if (valid_events_vector[e]){
      for (int j = 0; j < event_data->number_of_event_channels; j ++){
        valid_events[i*event_data->number_of_event_channels+j] = events[e*event_data->number_of_event_channels+j];
      }
      i ++;
    }
  }
  // Set the new events.
  event_data->set_events(valid_events, number_of_valid_events);
}

#endif

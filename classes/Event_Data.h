#ifndef EVENT_DATA_H
#define EVENT_DATA_H

#include "../helper_functions/general_helper_functions.h"

class Event_Data{
private:
  double* foe;
  double* events;

public:
  // Provided by the event information csv file.
  int row_location;
  int column_location;
  int polarity_location;
  int time_location;
  int image_height;
  int image_width;
  int number_of_event_channels;

  // Provided later.
  double start_time;
  double end_time;
  int number_of_events;

  // Constructor
  Event_Data(int row_location, int column_location, int polarity_location, int time_location, int image_height, int image_width, int number_of_event_channels, double* foe ){
    this->row_location = row_location;
    this->column_location = column_location;
    this->polarity_location = polarity_location;
    this->time_location = time_location;
    this->image_height = image_height;
    this->image_width = image_width;
    this->number_of_event_channels = number_of_event_channels;
    this->foe = foe;
    events = new double[0];
  }

  // Safely set the foe;
  void set_foe(double* new_foe){
    delete[] this->foe;
    foe = new_foe;
  }

  // Get foe.
  double* get_foe(){
    return foe;
  }

  // Safely set the events.
  void set_events(double* new_events, int number_of_events){
    delete[] this->events;
    this->events = new_events;

    // Update the number of events.
    this->number_of_events = number_of_events;

    // Update the times.
    this->start_time = this->events[this->time_location];
    this->end_time = this->events[(this->number_of_events-1)*this->number_of_event_channels+this->time_location];
  }

  // Delete the events;
  void delete_events(){
    delete[] this->events;
    this->events = new double[0];
  }

  // Get events.
  double* get_events(){
    return events;
  }

  // Sets the array of event data, the number of events, and the event sequence
  // start and end times in the event data object.
  void set_event_data_from_event_csv_file(string event_csv_file){
    // Get the number of events in the csv file and set in event_data object.
    int number_of_events = number_of_lines_in_csv_file(event_csv_file);
    this->number_of_events = number_of_events;

    // Get events from file and set in event_data object.
    this->events = csv_to_matrix(event_csv_file, this->number_of_events, this->number_of_event_channels);

    // Get the event data start and end times and set in event_data object.
    this->start_time = this->get_events()[0 + this->time_location];
    this->end_time = this->get_events()[(this->number_of_events-1)*this->number_of_event_channels + this->time_location];
  }

  // Expand the event image plane.
  void exand_image_plane(int height_increase_factor, int width_increase_factor){
    // Update the image plane width and height.
    this->image_height += 2*height_increase_factor;
    this->image_width += 2*width_increase_factor;

    // Update the focus of expansion.
    foe[0] += height_increase_factor;
    foe[1] += width_increase_factor;

    // Update all the events.
    for (int i = 0; i < this->number_of_events; i++){
      events[i*this->number_of_event_channels + this->row_location] += height_increase_factor;
      events[i*this->number_of_event_channels + this->column_location] += width_increase_factor;
    }
  }

  // Print all event information.
  void print_all_event_information(){
    cout<<"row_location: "<<this->row_location<<endl;
    cout<<"column_location: "<<this->column_location<<endl;
    cout<<"polarity_location: "<<this->polarity_location<<endl;
    cout<<"time_location: "<<this->time_location<<endl;
    cout<<"image_height: "<<this->image_height<<endl;
    cout<<"image_width: "<<this->image_width<<endl;
    cout<<"number_of_event_channels: "<<this->number_of_event_channels<<endl;
    cout<<"foe: "<<this->foe[0]<<", "<<this->foe[1]<<endl;
    cout<<"start_time: "<<start_time<<endl;
    cout<<"end_time: "<<end_time<<endl;
    cout<<"number_of_events: "<<number_of_events<<endl;
  }

  // Print all events.
  void print_events(){
    for (int i = 0; i < this->number_of_events; i++){
      for (int j = 0; j < this->number_of_event_channels; j ++){
        cout<<events[i*this->number_of_event_channels + j]<<" ";
      }
      cout<<endl;
    }
  }

  // Destructor.
  ~Event_Data(){
    delete[] foe;
    delete[] events;
  }
};

#endif

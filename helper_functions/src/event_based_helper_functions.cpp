// Specialised helper functions for event based data.

#ifndef EVENT_BASED_HELPER_FUNCTIONS_CPP
#define EVENT_BASED_HELPER_FUNCTIONS_CPP

#include "../event_based_helper_functions.h"

// Get the values from the event data information file.
vector<double> get_event_data_information(string path_to_event_information){
  vector<double> variable_values;
  string line;

  ifstream file (path_to_event_information);
  if (file.is_open()){
    int i = 0;
    while (getline(file,line)){
      // The variable values are on the second line of the file.
      if (i > 0){
        string s = line;
        string delimiter = ",";
        size_t position = 0;
        string value;
        int j = 0;
        while ((position = s.find(delimiter)) != std::string::npos) {
            value = s.substr(0, position);
            variable_values.push_back(stod(value));
            s.erase(0, position + delimiter.length());
            j++;
        }
        value = s.substr(0, position);
        variable_values.push_back(stod(value));
      }
      i++;
    }
    file.close();
  }
  return variable_values;
}

// Set the event data provided by the event data information file.
Event_Data* set_event_data_from_event_information_file(string path_to_event_information){
  vector<double> event_information = get_event_data_information(path_to_event_information);
  int row_location = event_information[0];
  int column_location = event_information[1];
  int polarity_location = event_information[2];
  int time_location = event_information[3];
  int image_height = event_information[4];
  int image_width = event_information[5];
  int number_of_event_channels = event_information[6];
  double* foe = new double[2];
  foe[0] = event_information[7];
  foe[1] = event_information[8];

  Event_Data* event_data = new Event_Data(row_location, column_location, polarity_location, time_location, image_height, image_width, number_of_event_channels, foe);
  return event_data;
}

// Reset the event data provided by the event data information file.
void reset_event_data_from_event_information_file(Event_Data* event_data, string path_to_event_information){
  vector<double> event_information = get_event_data_information(path_to_event_information);
  event_data->row_location = event_information[0];
  event_data->column_location = event_information[1];
  event_data->polarity_location = event_information[2];
  event_data->time_location = event_information[3];
  event_data->image_height = event_information[4];
  event_data->image_width = event_information[5];
  event_data->number_of_event_channels = event_information[6];
  double* foe = new double[2];
  foe[0] = event_information[7];
  foe[1] = event_information[8];
  event_data->set_foe(foe);
}

#endif

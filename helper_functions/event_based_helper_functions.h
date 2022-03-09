// Specialised helper functions for event based data.

#ifndef EVENT_BASED_HELPER_FUNCTIONS_H
#define EVENT_BASED_HELPER_FUNCTIONS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../classes/Event_Data.h"

using namespace std;

// Get the values from the event data information file.
// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
vector<double> get_event_data_information(string path_to_event_information);

// Set the event data provided by the event data information file.
Event_Data* set_event_data_from_event_information_file(string path_to_event_information);

// Reset the event data provided by the event data information file.
void reset_event_data_from_event_information_file(Event_Data* event_data, string path_to_event_information);

#endif

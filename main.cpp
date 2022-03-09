#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <filesystem>
#include <chrono>
#include <sstream>

using namespace std;

#include "classes/Event_Data.h"
#include "classes/Camera_Parameters.h"
#include "classes/Domain.h"

#include "contrast_maximisation_branch_and_bound/contrast_maximisation_branch_and_bound.h"

#include "helper_functions/event_based_helper_functions.h"
#include "helper_functions/general_helper_functions.h"
#include "helper_functions/vector_helper_functions.h"

#include "event_removal/remove_events.h"

int main(int argc, char ** argv){


  // Get event data and event information from arguments.
  string path_to_event_data = argv[1];
  string path_to_event_data_csvs = path_to_event_data + "csvs/";
  string path_to_event_information = (string)argv[2]+"event_information.csv";
  string ground_truth_divergence_path = (string)argv[2]+"ground_truth_divergence.csv";

  // The polarity stream value to keep.
  int keep_pol_value = atoi(argv[3]);

  // Store all event data csv files in a sorted vector.
  vector<string> vector_csv_files;

  // Get a vector of ground truth divergence results.
  vector<double> gnd_truth_divergence = csv_to_vector_doubles(ground_truth_divergence_path);

  for (const auto & entry : std::filesystem::directory_iterator(path_to_event_data_csvs)){
    vector_csv_files.push_back(entry.path());
  }
  sort(vector_csv_files.begin(), vector_csv_files.end(), sort_by_integer_filename);
  int total_number_of_event_csvs = vector_csv_files.size();

  // Get the event files we are processing by file numbers.
  int start_file_number = 0;
  int end_file_number = vector_csv_files.size()-1;

  // Get the event data information and store in an event_data object.
  Event_Data* event_data = set_event_data_from_event_information_file(path_to_event_information);

  // Set the arbitrary event camera paramters.
  double arbitrary_focal_length = 5;
  double arbitrary_camera_height = 10;
  Camera_Parameters cp = Camera_Parameters(arbitrary_focal_length, arbitrary_camera_height);

  ofstream results_file;

  // Calculate divergence for each csv file.

  // Get divergence files ready to write to.
  string data_file_name = extract_last_directory_name(move_backwards_a_directory(move_backwards_a_directory(move_backwards_a_directory(path_to_event_data))));

  //  Each line of the results file will represent the results from one iteration of the divergence file.
  //  The lines will have the following format:
  // 0 - ground truth divergence
  // 1 - calculated divergence
  // 2 - error(%)
  // 3 - velocity
  // 4 - contrast
  // 5 - number of processed events
  // 6 - duration

  results_file.open ("Results.csv");
  vector<string> results_vector;
  results_vector.push_back("Ground truth divergence");
  results_vector.push_back("Estimated divergence");
  results_vector.push_back("Divergence error (%)");
  results_vector.push_back("Estimated velocity");
  results_vector.push_back("Maximised contrast");
  results_vector.push_back("Number of events in the event batch");
  results_vector.push_back("ECMD duration (s)");
  for (int i = 0; i < results_vector.size(); i++){
    if (i < results_vector.size()-1)
      results_file<<results_vector[i]<<", ";
    else
      results_file<<results_vector[i]<<"\n";
  }

  // Display the run time at the end of the sequence.
  vector<double> runtimes;

  for (int i = start_file_number; i <= end_file_number; i++){

    // If there were any changes to the event data file, we should reset this information.
    reset_event_data_from_event_information_file(event_data, path_to_event_information);

    string event_csv_file = vector_csv_files[i];
    cout<<endl;
    cout<<"Processing events for: "<<event_csv_file<<" ..."<<endl;

    // Set the remaining event data parameters from the event csv file.
    event_data->set_event_data_from_event_csv_file(event_csv_file);

    // Expand the image plane and correct event data.
    // This factor will be added to both sides to the width and height, so the
    // total height and width will be increased by 2x the factor
    int height_increase_factor = atoi(argv[4]);
    int width_increase_factor = atoi(argv[4]);
    event_data->exand_image_plane(height_increase_factor, width_increase_factor);

    // Remove a polarity stream.
    remove_polarity_stream(event_data, keep_pol_value);

    results_vector[5] = to_string(event_data->number_of_events);

    // Set the velocity domain.
    Domain velocity_domain;
    velocity_domain.left_domain = (-1*cp.Z0/(event_data->end_time - event_data->start_time) + 0.00000000000000001);
    velocity_domain.right_domain = 0;

    // Set the warp reference time to the duration of the sequence.
    double warp_reference_time = event_data->end_time-event_data->start_time;

    // Set the error threshold of the contrast maximisation algorithm.
    // This is the threshold difference between the current upper bound and the
    // current best image contrast.
    double contrast_threshold = 0.025;

    cout<<"Number of events: "<<event_data->number_of_events<<endl;
    // Find the optimal velocity from the contrast maximisation algorithm.
    double optimal_velocity;
    optimal_velocity = contrast_maximisation_branch_and_bound(event_data, velocity_domain, cp, warp_reference_time, contrast_threshold, results_vector);

    // Store more precise points.
    ostringstream streamObj;
    streamObj << optimal_velocity;
    results_vector[3] = streamObj.str();

    // Calculate divergence.
    double div = -1*(optimal_velocity/(arbitrary_camera_height+optimal_velocity*(event_data->end_time - event_data->start_time)))*1000000;
    results_vector[1] = to_string(div);

    // Qualitative Results.

    // Produced warped event images.
    string warped_images_path = path_to_event_data+"warped_event_images/";
    string unwarped_images_path = path_to_event_data+"unwarped_event_images/";

    // Save warped event image with optimal velocity.
    double* warped_image_plane = new double[event_data->image_height*event_data->image_width];
    zero_matrix(warped_image_plane, event_data->image_height, event_data->image_width);

    warp_events_to_image_plane(event_data, warped_image_plane, cp, optimal_velocity, warp_reference_time);
    // write_warped_events_to_csv(event_data, warp_references_time, cp, optimal_velocity, warped_images_path+extract_filename(event_csv_file)+".csv");
    matrix_to_csv(warped_image_plane, event_data->image_height, event_data->image_width, warped_images_path+extract_filename(event_csv_file)+".csv", true);

    zero_matrix(warped_image_plane, event_data->image_height, event_data->image_width);
    warp_events_to_image_plane(event_data, warped_image_plane, cp, -0.00000001, warp_reference_time);
    // write_warped_events_to_csv(event_data, warp_references_time, cp, optimal_velocity, unwarped_images_path+extract_filename(event_csv_file)+".csv");
    matrix_to_csv(warped_image_plane, event_data->image_height, event_data->image_width, unwarped_images_path+extract_filename(event_csv_file)+".csv", true);

    double contrast = contrast_of_image_plane(event_data, warped_image_plane);
    results_vector[4] = to_string(contrast);
    cout<<"Velocity: "<<optimal_velocity<<endl;
    cout<<"Contrast: "<<contrast<<endl;
    cout<<"Divergence: "<<div<<endl;

    // Error.
    if (i < gnd_truth_divergence.size()){
      cout<<"Error: "<<100*((gnd_truth_divergence[i]-div)/gnd_truth_divergence[i])<<" %"<<endl;
      results_vector[0] = to_string(gnd_truth_divergence[i]);
      results_vector[2] = to_string(100*(gnd_truth_divergence[i]-div)/gnd_truth_divergence[i]);
    }
    else {
      cout<<"Ground truth divergence result not avaliable."<<endl;
      results_vector[0] = "NA";
      results_vector[2] = "NA";
    }
    cout<<endl;

    delete[] warped_image_plane;
    event_data->delete_events();

    for (int i = 0; i < results_vector.size(); i++){
      if (i < results_vector.size()-1)
        results_file<<results_vector[i]<<", ";
      else
        results_file<<results_vector[i]<<"\n";
    }

  }
  results_file.close();

  delete event_data;

}

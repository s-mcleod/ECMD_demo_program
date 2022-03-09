#ifndef CONTRAST_MAXIMISATION_BRANCH_AND_BOUND_CPP
#define CONTRAST_MAXIMISATION_BRANCH_AND_BOUND_CPP

#include "../contrast_maximisation_branch_and_bound.h"

// Perform contrast maximisation on the event data to find the velocity within
// the velocity domain that maximises contrast.
double contrast_maximisation_branch_and_bound(Event_Data* event_data, Domain velocity_domain, Camera_Parameters cp, double warp_reference_time, double r, vector<string>& results_vector){

  int number_of_event_bound_parameters = 6;

  auto start_time = std::chrono::high_resolution_clock::now();

  double optimal_velocity = 0;

  // Initialisation of branch and bound algorithm
  // v_c - centre of velocity domain
  double v_c = velocity_domain.middle_of_domain();
  // v_best - current best velocity in the domain (best velocity => highest image contrast)
  double v_best = v_c;
  // ub - upper bound of current velocity domain
  double ub;
  ub = upper_bound(velocity_domain, event_data, cp, warp_reference_time);

  // Initialise and set up a priority queue.
  Priority_Queue pq = Priority_Queue();
  pq.push(State(ub, velocity_domain));

  int it = 0;

  // Get the current best image contrast.
  double best_image_contrast;
  best_image_contrast = image_plane_contrast_produced_by_velocity(event_data, v_best, cp, warp_reference_time);

  // Iterate through the branch and bound algorithm.
  long long avg_ub = 0;
  long long ub_elapsed_time = 0;
  long long contrast_elapsed_time = 0;

  while (!pq.empty()){
    // Remove the first element from the queue.
    State s = pq.get_head();

    // Termination conditions.
    if ((s.ub - best_image_contrast) <= r){
      optimal_velocity = v_best;

      auto finish_time = std::chrono::high_resolution_clock::now();
      auto total_duration = finish_time - start_time;
      long long total_elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(total_duration).count();
      results_vector[6] = to_string((double)total_elapsed_time/(double)1000000);

      cout<<"Duration: "<<((double)total_elapsed_time/(double)1000000)<<" s"<<endl;

      return optimal_velocity;
    }

    // Get the velocity that lies in the middle of the velocity domain.
    v_c = s.domain.middle_of_domain();

    double current_image_contrast;

    // Get the current image contrast.
    current_image_contrast = image_plane_contrast_produced_by_velocity(event_data, v_c, cp, warp_reference_time);

    // Update v_best.
    if (current_image_contrast >= best_image_contrast){
      v_best = v_c;
      best_image_contrast = current_image_contrast;
    }

    // Split the current domain into two sub-domains and add them to the queue
    // if they meet the requirements of the queue.
    Domain* split_domains = s.domain.split();

    // Calculate upper bounds.
    double upper_bound_first_domain;
    double upper_bound_second_domain;

    // Get upper bounds.
    upper_bound_first_domain = upper_bound(split_domains[0], event_data, cp, warp_reference_time);
    upper_bound_second_domain = upper_bound(split_domains[1], event_data, cp, warp_reference_time);

    if (upper_bound_first_domain >= best_image_contrast){
      pq.push(State(upper_bound_first_domain, split_domains[0]));
    }
    if (upper_bound_second_domain >= best_image_contrast){
      pq.push(State(upper_bound_second_domain, split_domains[1]));
    }
    it++;
  }

  auto finish_time = std::chrono::high_resolution_clock::now();
  auto total_duration = finish_time - start_time;
  long long total_elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(total_duration).count();
  cout<<"Duration: "<<(double)total_elapsed_time/(double)1000000<<" s"<<endl;

  optimal_velocity = v_best;

  return optimal_velocity;
}

#endif

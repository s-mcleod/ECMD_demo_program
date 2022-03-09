#!/usr/bin/env bash

g++ -std=c++17 \
main.cpp \
contrast_maximisation_branch_and_bound/src/warping.cpp \
contrast_maximisation_branch_and_bound/src/contrast.cpp \
helper_functions/src/event_based_helper_functions.cpp \
helper_functions/src/general_helper_functions.cpp \
helper_functions/src/matrix_helper_functions.cpp \
helper_functions/src/vector_helper_functions.cpp \
contrast_maximisation_branch_and_bound/src/contrast_maximisation_branch_and_bound.cpp \
contrast_maximisation_branch_and_bound/src/upper_bound.cpp \
event_removal/src/remove_events.cpp \
-o run

./run data/surf2D-7/event_batches/ data/surf2D-7/ 0 120
python3 display_images.py data/surf2D-7/event_batches/

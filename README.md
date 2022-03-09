# ECMD_demo_program
A demonstration program of our proposed exact contrast maximisation for event-based divergence estimation (ECMD) algorithm, from the paper "Globally Optimal Event-Based Divergence Estimation for Ventral Landing".  To avoid dependency issues, this algorithm is a pure CPU implementation, as opposed to the GPU CUDA version described in the paper.

## Program input / output:

A subset of the event batches generated from the Surf2D-7 full resolution dataset are provided.  ECMD sequentially estimates divergence over each event batch, and writes the output results to the "Results.csv" file.  

2 event batches (29.csv to 30.csv) are located in,

  data/surf2D-7/event_batches/csvs

and each contain 0.5 s worth of event data.  

## How to run ECMD:

Run the following in the command line:

  ./ECMD.sh

## Interpreting results:

Each row in the "Results.csv" file represents a 0.5 s event batch.  Each column represents, in order:

0 - ground truth divergence
1 - ECMD estimated divergence
2 - estimated divergence error from the ground truth (%)
3 - estimated vertical velocity
4 - maximised contrast of the motion compensated event image
5 - number of events processed in the event batch
6 - duration of ECMD

The motion compensated images produced under the estimated vertical velocity can be viewed in the folder,

  data/surf2D-7/event_batches/warped_event_images/images

after ECMD has been executed.  These images can be compared against the non-motion compensated event images located in the folder,

  data/surf2D-7/event_batches/warped_event_images/images

after ECMD has been executed.

## Dependencies

A system that can run c++17 and has the following installed,

  python3
  numpy
  PIL
  matplotlib

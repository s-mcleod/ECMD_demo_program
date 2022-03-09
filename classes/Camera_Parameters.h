#ifndef CAMERA_PARAMETERS_H
#define CAMERA_PARAMETERS_H

class Camera_Parameters{
public:
  // Focal length.
  double f;
  // Camera's height from surface.
  double Z0;

  // Find real world X, Y coordinates.
  double find_X(double x, double t, double estimated_vertical_velocity){
    return (x*(Z0+estimated_vertical_velocity*t)/f);
  }
  double find_Y(double y, double t, double estimated_vertical_velocity){
    return (y*(Z0+estimated_vertical_velocity*t)/f);
  }

  Camera_Parameters(double focal_length, double camera_height){
    f = focal_length;
    Z0 = camera_height;
  }
};

#endif

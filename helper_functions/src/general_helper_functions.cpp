#ifndef GENERAL_HELPER_FUNCTIONS_CPP
#define GENERAL_HELPER_FUNCTIONS_CPP

#include "../general_helper_functions.h"

// Function which finds if a pair of coordinates exists in an array of coordinates.
bool coordinates_exist(double row, double col, double* list_of_coordinates, int number_of_coordinates, bool print){
  for (int i = 0; i < number_of_coordinates; i++){
    if (print)
      cout<<"       *"<<list_of_coordinates[i*2]<<", "<<list_of_coordinates[i*2+1]<<endl;
    if (list_of_coordinates[i*2] == row && list_of_coordinates[i*2+1] == col){
      if (!print)
        return true;
    }
  }
  return false;
}

// Number of lines in a csv file.
int number_of_lines_in_csv_file(string path){
  string line;
  ifstream file (path);
  if (file.is_open()){
    int i = 0;
    while (getline(file,line)){
      i++;
    }
    file.close();
    return i;
  }
  else cout << "Unable to open file";
  return -1;
}

// Converts a csv file to a matrix of doubles.
double* csv_to_matrix(string path, int matrix_rows, int matrix_cols){
  double* matrix = new double[matrix_cols*matrix_rows];
  string line;

  ifstream event_file (path);
  if (event_file.is_open()){
    int i = 0;
    while (getline(event_file,line)){
      string s = line;
      string delimiter = ",";
      size_t position = 0;
      string value;
      while ((position = s.find(delimiter)) != std::string::npos) {
          value = s.substr(0, position);
          matrix[i] = stod(value);
          s.erase(0, position + delimiter.length());
          i++;
      }
      matrix[i] = stod(s);
      i++;
    }
    event_file.close();
  }
  return matrix;
}

// Saves a matrix of warped values as a csv file to load as an image.
void matrix_to_csv(double* matrix, int rows, int cols, string filename, bool on_off_pixels){
  ofstream myfile;
  myfile.open (filename);
  // Convert warped locations to intensity values.
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      // If an event is located in a pixel, set that pixel to on (255).  Else, set it to off (0).
      if (on_off_pixels){
        if (matrix[cols*r + c] > 0){
          myfile << 255;
        }
        else {
          myfile << 0;
        }
      }
      else {
        myfile << matrix[cols*r + c];
      }

      if (c != cols-1){
        myfile << ",";
      }
      else {
        myfile << "\n";
      }
    }
  }
  myfile.close();
  return;
}


// Extracts the filename from path (doesn't include the extension but assumes extension).
string extract_filename(string path){
  string filename = "";
  bool currently_reading_filename_characters = false;
  for (int i = path.size() - 1; i >= 0; i--) {
    // Reached the end of the filename.
    if (path[i] == '/'){
      return (filename);
    }
    if (currently_reading_filename_characters){
      filename = path[i] + filename;
    }
    // Reached the end of the extension.
    if (path[i] == '.'){
      currently_reading_filename_characters = true;
    }
  }
  return (filename);
}


// Extracts the last folder name in the directory.
// this/directory/structure/would/return -> return
// this/directory/structure/would/return/hello.csv -> return
// this/directory/structure/would/return/ -> return
string extract_last_directory_name(string path){
  string directory_name = "";
  bool reached_last_folder = false;
  bool currently_reading_directory_name_characters = true;
  char starting_character = path[path.size() - 1];
  bool extension_detected = false;
  for (int i = path.size() - 1; i >= 0; i--) {
    // Reached the end of the extension - ignore.
    if (path[i] == '.'){
      extension_detected = true;
      currently_reading_directory_name_characters = false;
      directory_name = "";
    }
    // Reached the end of the directory_name.
    if (path[i] == '/' && !reached_last_folder){
      if (starting_character != '/' && !extension_detected){
        return (directory_name);
      }
      currently_reading_directory_name_characters = true;
      reached_last_folder = true;
    }
    // Reached the end of the directory_name.
    else if (path[i] == '/' && reached_last_folder){
      return directory_name;
    }
    if (currently_reading_directory_name_characters){
      if (path[i] != '/')
        directory_name = path[i] + directory_name;
    }
  }
  return (directory_name);
}

// Moves backwards a directory.
// this/directory/structure/would/return -> this/directory/structure/would
// this/directory/structure/would/return/hello.csv -> this/directory/structure/would/return
// this/directory/structure/would/return/ -> this/directory/structure/would/return
string move_backwards_a_directory(string path){
  int location_of_last_backslash = 0;
  for (int i = path.size()-1; i >= 0; i--) {
    if (path[i] == '/'){
      location_of_last_backslash = i;
      break;
    }
  }
  if (location_of_last_backslash != 0){
    return (path.substr(0,location_of_last_backslash));
  }
  return path;
}


#endif

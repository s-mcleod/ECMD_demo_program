#ifndef GENERAL_HELPER_FUNCTIONS_H
#define GENERAL_HELPER_FUNCTIONS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Function which finds if a pair of coordinates exists in an array of coordinates.
bool coordinates_exist(double row, double col, double* list_of_coordinates, int number_of_coordinates, bool print = false);


// Number of lines in a csv file.
int number_of_lines_in_csv_file(string path);

// Converts a csv file to a matrix of doubles.
double* csv_to_matrix(string path, int matrix_rows, int matrix_cols);

// Saves a matrix of warped values as a csv file to load as an image.
void matrix_to_csv(double* matrix, int rows, int cols, string filename, bool on_off_pixels = false);


// Extracts the filename from path (doesn't include the extension but assumes extension).
string extract_filename(string path);

// Extracts the last folder name in the directory.
// this/directory/structure/would/return -> return
// this/directory/structure/would/return/hello.csv -> return
// this/directory/structure/would/return/ -> return
string extract_last_directory_name(string path);

// Moves backwards a directory.
// this/directory/structure/would/return -> this/directory/structure/would
// this/directory/structure/would/return/hello.csv -> this/directory/structure/would/return
// this/directory/structure/would/return/ -> this/directory/structure/would/return
string move_backwards_a_directory(string path);

#endif

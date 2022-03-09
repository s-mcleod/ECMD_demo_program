#ifndef VECTOR_HELPER_FUNCTIONS_H
#define VECTOR_HELPER_FUNCTIONS_H

#include <string>
#include <vector>

#include "general_helper_functions.h"

using namespace std;

// Sort a vector of string/integer filenames in accending order.
// E.g. ".../1.csv" ".../10.csv" ".../2.txt" would sort as:
//      ".../1.csv" ".../2.txt" ".../10.csv"
bool sort_by_integer_filename(string file1, string file2);

// Converts a csv file to a vector of doubles.
vector<double> csv_to_vector_doubles(string path);

#endif

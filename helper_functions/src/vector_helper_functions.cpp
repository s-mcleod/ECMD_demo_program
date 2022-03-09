#ifndef VECTOR_HELPER_FUNCTIONS_CPP
#define VECTOR_HELPER_FUNCTIONS_CPP

#include "../vector_helper_functions.h"

using namespace std;

// Sort a vector of string/integer filenames in accending order.
// E.g. ".../1.csv" ".../10.csv" ".../2.txt" would sort as:
//      ".../1.csv" ".../2.txt" ".../10.csv"
bool sort_by_integer_filename(string file1, string file2){
  string filename1 = extract_filename(file1);
  string filename2 = extract_filename(file2);

  return (stod(filename1) < stod(filename2));
}

// Converts a csv file to a vector of doubles.
vector<double> csv_to_vector_doubles(string path){
  vector<double> v;
  string line;

  ifstream f (path);
  if (f.is_open()){
    while (getline(f,line)){
      string s = line;
      string delimiter = ",";
      size_t pos = 0;
      string val;
      while ((pos = s.find(delimiter)) != std::string::npos) {
          val = s.substr(0, pos);
          v.push_back(stod(val));
          s.erase(0, pos + delimiter.length());
      }
      v.push_back(stod(s));
    }
    f.close();
  }
  return v;
}

#endif

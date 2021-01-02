/*
 * timeseries.cpp
 *
 * Author: Nimrod Gabbay ID 318322484
 */

#include "timeseries.h"
#include <fstream>
#include <sstream>

using namespace std;

// constructor - intializing the map with the data from the file
TimeSeries::TimeSeries(const char *CSVfileName) {
  this->fileName = CSVfileName;
  // creating a temp vector that will store the details before passing it
  // to the map
  vector<vector<string>> tmp;
  ifstream file;
  file.open(this->fileName);
  // iterating line by line
  while (file) {
    string line;
    if (!getline(file, line))
      break;
    stringstream ss(line);
    vector<string> words;
    // iterating detail by detail in the line seperated by ','
    while (ss) {
      string s;
      if (!getline(ss, s, ','))
        break;
      words.push_back(s);
    }
    tmp.push_back(words);
  }
  file.close();
  // passing the data to the map
  for (int i = 0; i < tmp[0].size(); i++) {
    vector<float> v;
    for (int j = 0; j < tmp.size() - 1; j++) {
      v.push_back(stof(tmp[j + 1][i]));
    }
    this->dict[tmp[0][i]] = v;
    this->numOfRows=v.size();
  }
}

// return a vector with the names of the features
const vector<string> TimeSeries::getFeaturesNames() const {
  vector<string> names;
  for (auto const &ele : this->dict) {
    names.push_back(ele.first);
  }
  return names;
}

// the function gets a name of feature (key in the map) and returns the vector
// with the values that belong to the key.
const vector<float> TimeSeries::getColValues(string name) const {
  return this->dict.at(name);
}

// return the map that saves the info of timeseries
const map<string, vector<float>> TimeSeries::getMap() const {
  return this->dict;
}

const int TimeSeries:: getNumOfRows() const{
  return this->numOfRows;
}

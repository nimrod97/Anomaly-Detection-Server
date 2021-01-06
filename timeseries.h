/*
 * timeseries.h
 *
 * Author: Nimrod Gabbay 
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;
#include <list>
#include <map>
#include <vector>

class TimeSeries {

public:
  TimeSeries(const char *CSVfileName);
  const vector<string> getFeaturesNames() const;
  const vector<float> getColValues(string name) const;
  const map<string, vector<float>> getMap() const;
  const int getNumOfRows()const;

private:
  const char *fileName;
  map<string, vector<float>> dict;
  int numOfRows;
};

#endif /* TIMESERIES_H_ */

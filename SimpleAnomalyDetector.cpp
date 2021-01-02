/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Nimrod Gabbay ID 318322484
 */

#include "SimpleAnomalyDetector.h"
#include <vector>

using namespace std;
SimpleAnomalyDetector::SimpleAnomalyDetector() {
  // TODO Auto-generated constructor stub
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
  // TODO Auto-generated destructor stub
}

/**
 * @brief the function gets timeseries (like a file), checks about
 * correlations between features in ts and learns about them
 *
 * @param ts timeseries
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
  map<string, vector<float>> dict = ts.getMap();
  map<string, vector<float>>::iterator it1;
  map<string, vector<float>>::iterator it2;
  map<string, vector<float>>::iterator it3;
  correlatedFeatures cf1;
  // iterating from begin to end
  for (it1 = dict.begin(); it1 != dict.end(); it1++) {
    it3 = it1;
    vector<float> v1 = it1->second;
    float a1[v1.size()];
    // copying the data to float array for the pearson func
    copy(v1.begin(), v1.end(), a1);
    float maxCorrl = 0;
    // iterating from begin+1 to end
    for (it2 = ++it3; it2 != dict.end(); it2++) {
      vector<float> v2 = it2->second;
      float a2[v2.size()];
      // copying the data to float array for the pearson func
      copy(v2.begin(), v2.end(), a2);
      // check correlation
      float corrl = abs(pearson(a1, a2, v1.size()));
      if (corrl > 0.5 && corrl > maxCorrl) {
        maxCorrl = corrl;
        cf1.feature1 = it1->first;
        cf1.feature2 = it2->first;
      }
    }
    // correlation occured
    if (maxCorrl != 0) {
      cf1.corrlation = maxCorrl;
      SimpleAnomalyDetector::cf.push_back(cf1);
    }
  }
}

/**
 * @brief the function gets timeseries and for every coreelated features,
 * checks for anomaly detection. if there is a deviation between one point to
 * the reg line that bigger than the thereshold, anomaly report will be added.
 * the funtion will return a vector with all the reports
 *
 * @param ts time series
 * @return vector<AnomalyReport>
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
  vector<AnomalyReport> vec;
  string f1;
  string f2;
  vector<float> v1;
  vector<float> v2;
  // iterating through every correlated features
  for (int i = 0; i < this->cf.size(); i++) {
    f1 = this->cf[i].feature1;
    f2 = this->cf[i].feature2;
    v1 = ts.getColValues(f1);
    v2 = ts.getColValues(f2);
    Point *points[v1.size()];
    for (int j = 0; j < v1.size(); j++) {
      // saving the points and checking about anomaly detection
      points[j] = new Point(v1[j], v2[j]);
      if (dev(*(points[j]), this->cf[i].lin_reg) > this->cf[i].threshold) {
        string description = f1 + "-" + f2;
        long timeStep = j + 1;
        // adding the report to the vector
        vec.push_back(AnomalyReport(description, timeStep));
      }
    }
    // freeing memory
    for (int k = 0; k < v1.size(); k++)
      delete points[k];
  }
  return vec;
}

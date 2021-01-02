/*
 * HybridAnomalyDetector.cpp
 *
 * Author: Nimrod Gabbay ID 318322484
 */

#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
  thereshold=0.9;
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
  // TODO Auto-generated destructor stub
}

void HybridAnomalyDetector::learnNormal(const TimeSeries &ts) {
  // calling to parent before adding stuff to the function
  SimpleAnomalyDetector::learnNormal(ts);
  for (int i = 0; i < this->cf.size(); i++) {
    vector<float> x = ts.getColValues(cf[i].feature1);
    vector<float> y = ts.getColValues(cf[i].feature2);
    Point *points[x.size()];
    // creating array of points from the correlated features
    for (int j = 0; j < x.size(); j++)
      points[j] = new Point(x[j], y[j]);
    if (cf[i].corrlation >= this->thereshold) {
      // creating reg line from the points
      cf[i].lin_reg = linear_reg(points, x.size());
      float maxDist = 0;
      // getting the max dest between all the points to the reg line
      for (int k = 0; k < x.size(); k++) {
        if (dev((*points[k]), cf[i].lin_reg) > maxDist)
          maxDist = dev(*(points[k]), cf[i].lin_reg);
      }
      cf[i].threshold = maxDist * 1.1;
      // freeing memory
      for (int k = 0; k < x.size(); k++)
        delete points[k];
    } else { // mincircle case - corrlation between 0.5 to defined thershold
      Circle c = findMinCircle(points, x.size());
      cf[i].threshold = c.radius * 1.1;
      cf[i].xCen = c.center.x;
      cf[i].yCen = c.center.y;
    }
  }
}

vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts){
  // calling to parent function - SimpleAnomalyDetector case
  vector<AnomalyReport> vec =SimpleAnomalyDetector::detect(ts);
  // min circle case - corrlation between 0.5 to defined thershold
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
      if ((this->cf[i].corrlation < this->thereshold &&
           (distanceBetweenTwoPoints((*(points[j])), Point{this->cf[i].xCen, this->cf[i].yCen}) >
            this->cf[i].threshold))) {
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

void HybridAnomalyDetector::setThershold(float x){
  this->thereshold=x;
}

float HybridAnomalyDetector::getThershold(){
  return this->thereshold;
}

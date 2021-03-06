/*
 * SimpleAnomalyDetector.h
 *
 * Author: Nimrod Gabbay
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "AnomalyDetector.h"
#include "anomaly_detection_util.h"
#include <algorithm>
#include <math.h>
#include <string.h>
#include <vector>

struct correlatedFeatures {
  string feature1, feature2; // names of the correlated features
  float corrlation;
  Line lin_reg;
  float threshold;
  float xCen;
  float yCen;
};

class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {
protected:
  vector<correlatedFeatures> cf;

public:
  SimpleAnomalyDetector();
  virtual ~SimpleAnomalyDetector();

  virtual void learnNormal(const TimeSeries &ts);
  virtual vector<AnomalyReport> detect(const TimeSeries &ts);
  vector<correlatedFeatures> getNormalModel() { return cf; }
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */

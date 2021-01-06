/*
 * HybridAnomalyDetector.h
 *
 * Author: Nimrod Gabbay
 */

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
	float thereshold;
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
	virtual void learnNormal(const TimeSeries &ts);
	virtual vector<AnomalyReport> detect(const TimeSeries &ts);
	void setThershold(float x);
	float getThershold();

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */

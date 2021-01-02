/*
 * HybridAnomalyDetector.h
 *
 * Author: Nimrod Gabbay ID 318322484
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

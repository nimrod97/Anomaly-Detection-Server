/*
 * anomaly_detection_util.h
 *
 * Author: Nimrod Gabbay ID 318322484
 */

#ifndef ANOMALYDETECTORUTIL_H_
#define ANOMALYDETECTORUTIL_H_

float avg(float *x, int size);

// returns the variance of X and Y
float var(float *x, int size);

// returns the covariance of X and Y
float cov(float *x, float *y, int size);

// returns the Pearson correlation coefficient of X and Y
float pearson(float *x, float *y, int size);

class Point {
public:
  float x, y;
  Point(float x, float y) : x(x), y(y) {}
};

class Line {
public:
  float a, b;
  Line() : a(0), b(0){};
  Line(float a, float b) : a(a), b(b) {}
  float f(float x) { return a * x + b; }
  // finding the intersection point between 2 lines
  Point intersection(Line l1) {
    float x = this->a - l1.a;
    float w = this->b - l1.b;
    float xPoint = (w * -1) / x;
    float yPoint = f(xPoint);
    return Point(xPoint, yPoint);
  }
};


// performs a linear regression and returns the line equation
Line linear_reg(Point **points, int size);

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size);

// returns the deviation between point p and the line
float dev(Point p, Line l);

#endif

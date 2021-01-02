/*
 * anomaly_detection_util.cpp
 *
 * Author: Nimrod Gabbay ID 318322484
 */

#include "anomaly_detection_util.h"
#include <cmath>
#include <math.h>

// returns the average of X
float avg(float *x, int size) {
  float sum = 0;
  for (int i = 0; i < size; i++) {
    sum += *(x + i);
  }
  return sum / size;
}

// returns the variance of X and Y
float var(float *x, int size) {
  float sum1 = 0, avg1 = 0, avg2 = 0;
  for (int i = 0; i < size; i++) {
    sum1 += pow(*(x + i), 2);
  }
  avg1 = sum1 / size;
  avg2 = avg(x, size);
  return avg1 - pow(avg2, 2);
}

// returns the covariance of X and Y
float cov(float *x, float *y, int size) {
  float cov = 0, sum = 0;
  // getting the average of X and Y
  float Ex = avg(x, size);
  float Ey = avg(y, size);
  for (int i = 0; i < size; i++) {
    sum += (*(x + i) - Ex) * (*(y + i) - Ey);
  }
  cov = sum / size;
  return cov;
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float *x, float *y, int size) {
  float sigmaX = sqrt(var(x, size));
  float sigmaY = sqrt(var(y, size));
  return cov(x, y, size) / (sigmaX * sigmaY);
}

// performs a linear regression and returns the line equation
Line linear_reg(Point **points, int size) {
  // creating arrays of each coordinate
  float *coorX = new float[size];
  float *coorY = new float[size];
  for (int i = 0; i < size; i++) {
    coorX[i] = points[i]->x;
    coorY[i] = points[i]->y;
  }
  // getting the average of X and Y
  float avgX = avg(coorX, size);
  float avgY = avg(coorY, size);
  // getting the "a" and "b" of the line equation
  float a = cov(coorX, coorY, size) / var(coorX, size);
  float b = avgY - a * avgX;
  delete[] coorX;
  delete[] coorY;
  return Line(a, b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size) {
  // initializng a line from the given points
  Line line = linear_reg(points, size);
  // getting the y coordinate of the given x on the line
  float fx = line.f(p.x);
  // getting the deviation
  float dev = std::abs(p.y - fx);
  return dev;
}

// returns the deviation between point p and the line
float dev(Point p, Line l) {
  float fx = l.f(p.x);
  float dev = std::abs(p.y - fx);
  return dev;
}

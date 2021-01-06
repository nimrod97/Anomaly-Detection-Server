/*
 * minCircle.h
 *
 * Author: Nimrod Gabbay
 */

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <math.h>
#include <vector>
#include "anomaly_detection_util.h"
using namespace std;

class Circle {
public:
  Point center;
  float radius;
  Circle(Point c, float r) : center(c), radius(r) {}
};


float distanceBetweenTwoPoints(const Point &p1, const Point &p2);
Circle getCircleFrom2(const Point &p1, const Point &p2);
Circle getCircleFrom3(const Point &p1, const Point &p2, const Point &p3);
bool pIsInside(const Circle &c, const Point &p);
Circle minCircle(vector<Point> vec);
Circle helper(Point **points, size_t size, vector<Point> vec);
Circle findMinCircle(Point **points, size_t size);


#endif /* MINCIRCLE_H_ */

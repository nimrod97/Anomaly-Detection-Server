/*
 * minCircle.cpp
 *
 * Author: Nimrod Gabbay ID 318322484
 */

#include "minCircle.h"

/**
 * @brief return the distance of 2 points
 *
 * @param p1 - point
 * @param p2 - point
 * @return float distance between the two points
 */
float distanceBetweenTwoPoints(const Point &p1, const Point &p2) {
  return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

/**
 * @brief return the circle that two points are generate.
 * we know that two points can generate only one unique circle.
 *
 * @param p1 - point
 * @param p2 - point
 * @return the circle that the two points are generate.
 */
Circle getCircleFrom2(const Point &p1, const Point &p2) {
  Point center = {(p1.x + p2.x) / 2, (p1.y + p2.y) / 2};
  return Circle(center, distanceBetweenTwoPoints(p1, p2) / 2);
}

/**
 * @brief return the circle that three points are generate.
 * we know that three points can generate only one unique circle.
 *
 * @param p1 point
 * @param p2 - point
 * @param p3 - point
 * @return the circle that the three points are generate.
 *
 */
Circle getCircleFrom3(const Point &p1, const Point &p2, const Point &p3) {
  // cases of slopes of 0 and indefinite - the center of the circle must lie in
  // the middle of the diameter
  if (p2.x - p1.x == 0) {
    if (p3.y - p1.y == 0)
      return Circle(Point((p2.x + p3.x) / 2, (p2.y + p3.y) / 2),
                    distanceBetweenTwoPoints(p2, p3) / 2);
    if (p3.y - p2.y == 0)
      return Circle(Point((p1.x + p3.x) / 2, (p1.y + p3.y) / 2),
                    distanceBetweenTwoPoints(p1, p3) / 2);
  }
  if (p3.x - p1.x == 0) {
    if (p2.y - p1.y == 0)
      return Circle(Point((p2.x + p3.x) / 2, (p2.y + p3.y) / 2),
                    distanceBetweenTwoPoints(p2, p3) / 2);
    if (p3.y - p2.y == 0)
      return Circle(Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2),
                    distanceBetweenTwoPoints(p1, p2) / 2);
  }
  if (p3.x - p2.x == 0) {
    if (p2.y - p1.y == 0)
      return Circle(Point((p1.x + p3.x) / 2, (p1.y + p3.y) / 2),
                    distanceBetweenTwoPoints(p1, p3) / 2);
    if (p3.y - p1.y == 0)
      return Circle(Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2),
                    distanceBetweenTwoPoints(p1, p2) / 2);
  }
  // building two lines and building two vertical lines to the middle of the
  // lines. the center of the circle must lie in the intersection point between
  // the vertical lines according to a theorem - in a circle that blocks
  // triangle, the center of the circle is the meeting point of the vertical
  // lines to each edge.

  float m = (p2.y - p1.y) / (p2.x - p1.x);
  // y-y1=m(x-x1) --> y=mx-mx1+y1
  float mx1 = p1.x * m;
  float b = mx1 * -1 + p1.y;
  Line l1(m, b);
  float mVertical1 = -1 / m;
  Point middle1 = {(p1.x + p2.x) / 2, (p1.y + p2.y) / 2};
  // y-middle1.y=m(x-middle1.x)--> y=mx-m*middle1.x+middle1.y
  float b1 = mVertical1 * -1 * middle1.x + middle1.y;
  // vertical line to the middle of the line of p1 and p2
  Line vertical1(mVertical1, b1);
  float m2 = (p3.y - p1.y) / (p3.x - p1.x);
  // y-y1=m(x-x1) -->y=mx-mx1+y1
  mx1 = p1.x * m2;
  b = mx1 * -1 + p1.y;
  Line l2(m2, b);
  float mVertical2 = -1 / m2;
  Point middle2 = {(p1.x + p3.x) / 2, (p1.y + p3.y) / 2};
  // y-middle2.y=m(x-middle2.x)--> y=mx-m*middle2.x+middle2.y
  float b2 = mVertical2 * -1 * middle2.x + middle2.y;
  // vertical line to the middle of the line of p1 and p3
  Line vertical2(mVertical2, b2);
  // finding the center of the circle
  Point center = vertical1.intersection(vertical2);
  // finding the radius
  float R = distanceBetweenTwoPoints(p1, center);
  return Circle(center, R);
}

/**
 * @brief the function gets a circle and a point, and checks if the point is in
 * the circle.
 *
 * @param c - circle
 * @param p - point
 * @return the function returns true if the point is in the circle. otherwise,
 * it returns false.
 */
bool pIsInside(const Circle &c, const Point &p) {
  return (distanceBetweenTwoPoints(c.center, p) <= c.radius);
}

/**
 * @brief the function gets vector of points that have to be on the edge of the
 * circle, and returns the minimum circle that includes them.
 *
 * @param vec - vector of points
 * @return the minimum circle that these points are on the edge of it.
 */
Circle minCircle(vector<Point> vec) {
  if (vec.size() <= 3) {
    if (vec.empty())
      return Circle(Point(0, 0), 0);
    if (vec.size() == 1)
      return Circle(vec[0], 0);
    if (vec.size() == 2)
      return getCircleFrom2(vec[0], vec[1]);
  }
  // check if we can create a circle by 2 points only
  for (int i = 0; i < 3; i++) {
    for (int j = i + 1; j < 3; j++) {
      Circle circ = getCircleFrom2(vec[i], vec[j]);
      bool flag = true;
      for (int k = 0; k < vec.size(); k++) {
        if (!pIsInside(circ, vec[k])) {
          flag = false;
          break;
        }
      }
      if (flag)
        return circ;
    }
  }
  return getCircleFrom3(vec[0], vec[1], vec[2]);
}

/**
 * @brief the recursive function.
 * the function gets array of points, the size of the array,
 * and a vector of points that have to be on the edge of the circle.
 * the function returns the minimun circle that encloses all the points.
 *
 * @param points - the points that have to be in the circle.
 * @param size - the size of the array of points.
 * @param vec - the points that have to be on the edge of the circle.
 * @return the minimun circle that encloses all the points.
 */
Circle helper(Point **points, size_t size, vector<Point> vec) {
  // edge cases
  if (size == 0 || vec.size() == 3) {
    return minCircle(vec);
  }

  Point p = {points[size - 1]->x, points[size - 1]->y};
  Circle c = helper(points, size - 1, vec);
  if (pIsInside(c, p))
    return c;

  // else, the point must be on the edge of the circle
  vec.push_back(p);

  return helper(points, size - 1, vec);
}

/**
 * @brief the main function that calls the recursive function.
 *
 * @param points - an array points that we want to find a minimum enclosing
 * circle for them.
 * @param size - the size of the array points.
 * @return
 */
Circle findMinCircle(Point **points, size_t size) {
  return helper(points, size, {});
}

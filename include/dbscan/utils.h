#ifndef __utils_h
#define __utils_h

#include <vector>

using std::vector;


class Point {
  public:
    float x;
    float y;
    int cid; // -1: initial, 0: noise, 1+: assigned
    Point(float x_, float y_, int cid_ = -1) {
        x = x_;
        y = y_;
        cid = cid_;
    }
    
    float dist(const Point& p2) const {
        float dx = x - p2.x;
        float dy = y - p2.y;
        return hypot(dx, dy);
    }
};


/* class for utility functions */
class Utils {
  public:
    static void visualizeParticles(const vector<Point>& particles, float max_bound);

  public:
    Utils();
    ~Utils();

  private:
};

#endif


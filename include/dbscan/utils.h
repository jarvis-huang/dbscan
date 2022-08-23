#ifndef __utils_h
#define __utils_h

#include <vector>
#include <random>
#include <boost/timer/timer.hpp>

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


class MyTimer {
private:
    boost::timer::cpu_timer t_m;
public:
    MyTimer() {t_m = boost::timer::cpu_timer();}
    void tic();
    int toc(); // return in us
};


class MyUniformDist {
    // https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
private:
    std::random_device rd;
    std::mt19937 gen;  // here you could set the seed, but std::random_device already does that
    std::uniform_real_distribution<float> dis;
public:
    MyUniformDist(float low=0.0f, float high=1.0f);
    float sample();
};


class MyNormalDist {
    // https://en.cppreference.com/w/cpp/numeric/random/normal_distribution
private:
    std::random_device rd;
    std::mt19937 gen;  // here you could set the seed, but std::random_device already does that
    std::normal_distribution<float> dis;
public:
    MyNormalDist(float mean=0.0f, float stddev=1.0f);
    float sample();
};


/* class for utility functions */
class Utils {
  public:
    static void visualizeParticles(const vector<Point>& particles, float max_bound, float pauseT = 10.0f);

  public:
    Utils();
    ~Utils();

  private:
};

#endif


#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <boost/format.hpp>

using std::vector;
using std::string;

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

vector<int> rangeQuery(const vector<Point>& points, const Point& p_query, float eps) {
    vector<int> indices_in_range;
    int n_pts = (int)points.size();
    for (int i=0; i<n_pts; i++) {
        float d = points[i].dist(p_query);
        if (d <= eps)
            indices_in_range.push_back(i);
    }
    std::cout << "---" << std::endl;
    for (int i : indices_in_range) {
        Point p = points[i];
        std::cout << boost::format("(%.1f, %.1f)\n") % p.x % p.y;
    }
    return indices_in_range;
}

int main() {
    vector<Point> points;
    points.push_back(Point(0, 0));
    points.push_back(Point(0.1, 0.1));
    points.push_back(Point(0.2, 0.2));
    points.push_back(Point(1, 1));
    points.push_back(Point(1.1, 1.1));
    points.push_back(Point(2, 2));
    points.push_back(Point(2.2, 2.2));
    points.push_back(Point(2.1, 2.1));
    points.push_back(Point(1.9, 1.9));
    points.push_back(Point(1.2, 1.2));
    
    Point p_query(2, 2);
    float eps = 0.25;
    rangeQuery(points, p_query, eps);
}

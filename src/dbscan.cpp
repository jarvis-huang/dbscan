#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <boost/format.hpp>
#include <boost/timer/timer.hpp>
#include "dbscan/matplotlibcpp.h" // Plotting
#include "dbscan/utils.h" // utility functions
#include "dbscan/partgen.h" // data generation

using std::vector;
using std::deque;
using std::string;


class DBSCAN {
  private:
    float eps;
    size_t minPts;
    
    vector<int> rangeQuery(const vector<Point>& points, const Point& p_query) {
        vector<int> indices_in_range;
        int n_pts = (int)points.size();
        for (int i=0; i<n_pts; i++) {
            float d = points[i].dist(p_query);
            if (d <= eps)
                indices_in_range.push_back(i);
        }
        /*
        std::cout << "---" << std::endl;
        for (int i : indices_in_range) {
            Point p = points[i];
            std::cout << boost::format("(%.1f, %.1f)\n") % p.x % p.y;
        }
        */
        return indices_in_range;
    }

  public:
    DBSCAN(float eps_, size_t minPts_) {
        eps = eps_;
        minPts = minPts_;
    }
    
    void cluster(vector<Point>& points) {
        int C = 0; // current cluster id (valid cluster starts at 1)
        size_t npts = points.size();
        for (size_t i=0; i<npts; i++) {
            Point& p = points[i];
            if (p.cid != -1) {
                continue; // Previously processed in inner loop
            }
            vector<int> neighbors_v = rangeQuery(points, p);
            deque<int> neighbors(neighbors_v.begin(), neighbors_v.end());
            if (neighbors.size()<minPts) { // density check
                p.cid = 0; // label as noise
                continue;
            } else {
                C++; // next cluster label
                p.cid = C;
                while (!neighbors.empty()) {
                    Point& q = points[neighbors[0]];
                    if (q.cid <= 0) { // not preoviously processed
                        q.cid = C; // assign its cluster id
                        vector<int> more_neighbors = rangeQuery(points, q); // Find neighbors
                        if (more_neighbors.size()>=minPts) { // density check
                            neighbors.insert(neighbors.end(), more_neighbors.begin(), more_neighbors.end()); // merge more_neighbors into neighbors
                        }
                    }
                    neighbors.pop_front(); // delete first point from neighbors
                }
            }
        }
    }
};


int main() {
    float max_bound = 10.0f;
    int num_particles = 500;
    int n_clusters = 5;
    boost::timer::auto_cpu_timer t(6, "%t sec CPU\n");
    ParticleGenerator partgen(max_bound, num_particles, n_clusters);
    vector<Point> points = partgen.getParticles();
    
    /*
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
    */
    
    float eps = 0.8; //0.3;
    size_t minPts = 5; //3;
    //boost::timer::auto_cpu_timer* t = new boost::timer::auto_cpu_timer(3, "%t sec CPU");
    //boost::timer::auto_cpu_timer t(3, "%t sec CPU");
    DBSCAN dbscan(eps, minPts);
    dbscan.cluster(points);
    return 0;
    //delete t;
    std::cout << "---\n";
    std::cout << boost::format("(x, y) cluster_id\n");
    for (auto p: points) {
        std::cout << boost::format("(%.1f, %.1f) %d\n") % p.x % p.y % p.cid;
    }

    Utils::visualizeParticles(points, max_bound);
}

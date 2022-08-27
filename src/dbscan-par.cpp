#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <boost/format.hpp>
#include "dbscan/matplotlibcpp.h" // Plotting
#include "dbscan/utils.h" // utility functions
#include "dbscan/partgen.h" // data generation
#include <CL/cl.hpp>
#include "opencl_playground/myocl.hpp"

//#define CL_TARGET_OPENCL_VERSION 120
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
    
    /* Modify points with cluster info in-place. */
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
    int num_particles = 10000;
    int n_clusters = 6;
    MyTimer t;
    ParticleGenerator partgen(max_bound, num_particles, n_clusters);
    vector<Point> points = partgen.getParticles();
    
    cl::Device default_device = util::getDevice("CPU");
    cl::Context context({default_device});
    cl_int err;
    //cl::Context context(CL_DEVICE_TYPE_CPU, nullptr, nullptr/*&errCallback*/, nullptr, &err);
    //util::check_error(err, "", "", 0);
    std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
    cl::CommandQueue queue(context, default_device);
    cl::Program program = util::makeProgramFromKernelCode("../src/dbscan.cl", context);
    
    float eps = 0.5; //0.3;
    size_t minPts = 150; //3;
    DBSCAN dbscan(eps, minPts);
    dbscan.cluster(points);
    int ms_elapsed = t.toc();
    std::cout << boost::format("elapsed %d ms CPU\n") % ms_elapsed;
    
    bool printing = false;
    if (printing) {
        std::cout << "---\n";
        std::cout << boost::format("(x, y) cluster_id\n");
        for (auto p: points) {
            std::cout << boost::format("(%.1f, %.1f) %d\n") % p.x % p.y % p.cid;
        }
    }
    
    Utils::visualizeParticles(points, max_bound, 10.0f);
}

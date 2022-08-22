#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <boost/format.hpp>
#include "dbscan/matplotlibcpp.h" // Plotting
#include "dbscan/utils.h" // utility functions

using std::vector;
using std::string;
namespace plt = matplotlibcpp;

void Utils::visualizeParticles(const vector<Point>& particles, float max_bound) {
    string colors = "krgbcmy";
    int max_cid = 0;
    for (auto& particle: particles) {
        int cid = particle.cid;
        if (cid > max_cid)
            max_cid = cid;
    }
    if (max_cid > static_cast<int>(colors.size())-1) {
        std::cout << boost::format("Too many clusters (%d)\n") % max_cid;
        return;
    }
                
    plt::figure_size(600, 600);
    plt::xlim(0.0f, max_bound);
    plt::ylim(0.0f, max_bound);
    plt::axis("equal");
    plt::grid(true);
    for (auto& particle: particles) {
        float x = particle.x;
        float y = particle.y;
        int cid = particle.cid;
        string c;
        if (cid <= 0)
            c = "k";
        else
            c = colors.substr(cid, 1);
        plt::plot({x}, {y}, {{"color", c}, {"marker", "o"}, 
            {"linestyle", ""}, {"markersize", "2"}});
    }
    plt::show(false); // non-blocking
    plt::pause(10.0);
    plt::close();
}

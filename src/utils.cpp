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

void MyTimer::tic() {
    t_m.stop();
    t_m.resume();
}

int MyTimer::toc() { // return in ms
    t_m.stop();
    boost::timer::cpu_times const elapsed_times(t_m.elapsed());
    boost::timer::nanosecond_type const elapsed(elapsed_times.system
    + elapsed_times.user);
    double us = static_cast<double> (elapsed / 1000);
    return static_cast<int> (round (us / 1000));
}


MyUniformDist::MyUniformDist(float low, float high) {
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<float>(low, high);
}

float MyUniformDist::sample() {
    return dis(gen);
}

MyNormalDist::MyNormalDist(float mean, float stddev) {
    gen = std::mt19937(rd());
    dis = std::normal_distribution<float>(mean, stddev);
}

float MyNormalDist::sample() {
    return dis(gen);
}

void Utils::visualizeParticles(const vector<Point>& particles, float max_bound, float pauseT) {
    string colors = "krgbcmy";
    int max_cid = 0;
    for (auto& particle: particles) {
        int cid = particle.cid;
        if (cid > max_cid)
            max_cid = cid;
    }
    int allowed_max_cid = static_cast<int>(colors.size())-1;
    if (max_cid > allowed_max_cid) {
        std::cout << boost::format("Too many clusters (%d > %d)\n") % max_cid % allowed_max_cid;
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
    plt::pause(pauseT);
    plt::close();
}

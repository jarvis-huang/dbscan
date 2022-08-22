#ifndef __partgen_h
#define __partgen_h

#include <vector>
#include <Eigen/Dense>
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include "dbscan/utils.h"

using std::vector;


class NormalDist {
  private:
    boost::mt19937 rng;
    boost::normal_distribution<> nd;
  public:
    NormalDist(float mean = 0.0f, float sd = 1.0f) {
        nd = boost::normal_distribution<>(mean, sd);
    }
    float sample() {
        return nd(rng);
    }
};


class ParticleGenerator {
  private:
    float max_bound_m;
    int num_particles_m;
    int n_clusters_m;
    Eigen::MatrixXf centers_m;
    vector<Point> particles_m;
    NormalDist nd_m;
    // private functions
    Eigen::MatrixXf genClusterCenters();
    vector<Point> genParticles(Eigen::VectorXf center, int count);
     
  public:
    ParticleGenerator(float max_bound, int num_particles, int n_clusters);
    vector<Point> getParticles() const {return particles_m;}
};


#endif

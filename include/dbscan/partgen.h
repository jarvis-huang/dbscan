#ifndef __partgen_h
#define __partgen_h

#include <vector>
#include <Eigen/Dense>
#include "dbscan/utils.h"

using std::vector;


class ParticleGenerator {
  private:
    float max_bound_m;
    int num_particles_m;
    int n_clusters_m;
    Eigen::MatrixXf centers_m;
    vector<Point> particles_m;
    // private functions
    Eigen::MatrixXf genClusterCenters();
    vector<Point> genParticles(Eigen::VectorXf center, int count);
     
  public:
    ParticleGenerator(float max_bound, int num_particles, int n_clusters);
    vector<Point> getParticles() const {return particles_m;}
    //~ParticleGenerator();
};


#endif

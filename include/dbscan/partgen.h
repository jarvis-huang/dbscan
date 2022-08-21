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
    // private functions
    Eigen::MatrixXf genClusterCenters();
     
  public:
    ParticleGenerator(float max_bound, int num_particles, int n_clusters);
    //~ParticleGenerator();
};


#endif

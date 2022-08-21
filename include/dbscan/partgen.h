#ifndef __partgen_h
#define __partgen_h

#include <vector>


using std::vector;


class ParticleGenerator {
  private:
    float max_bound;
    int num_particles;
    int n_clusters;
     
  public:
    ParticleGenerator(float max_bound, int num_particles, int n_clusters);
    ~ParticleGenerator();
};


#endif

#include <iostream>
#include <Eigen/Dense>
#include <boost/format.hpp>
#include "dbscan/partgen.h"


void removeRow(Eigen::MatrixXf& matrix, unsigned int rowToRemove)
{
    unsigned int numRows = matrix.rows()-1;
    unsigned int numCols = matrix.cols();

    if( rowToRemove < numRows )
        matrix.block(rowToRemove,0,numRows-rowToRemove,numCols) = matrix.block(rowToRemove+1,0,numRows-rowToRemove,numCols);

    matrix.conservativeResize(numRows,numCols);
}


ParticleGenerator::ParticleGenerator(float max_bound, int num_particles, int n_clusters) {
    max_bound_m = max_bound;
    num_particles_m = num_particles;
    n_clusters_m = n_clusters;
    
    assert (1 <= n_clusters_m && n_clusters_m <= 10);
        
    // pick cluster centers
    centers_m = genClusterCenters();
    //std::cout << "cluster centers: \n" << centers_m << std::endl;
    
    // generate particles for each cluster
    float avg_particles = static_cast<float>(num_particles) / n_clusters;
    std::cout << boost::format("n_clusters=%d, avg_particles=%.1f\n") % n_clusters % avg_particles;
    
    while (true) {
        particles_m.clear();
        vector<int> counts; // num. of particles for each cluster
        int total = 0;
        for (int i = 0; i<n_clusters-1; i++) {
            int this_count = static_cast<int>(avg_particles + nd_m.sample() * avg_particles/6);
            counts.push_back(this_count);
            total += this_count;
        }
        if (total<num_particles) {
            // accept this sampling
            int running = 0;
            for (int j = 0; j<n_clusters-1; j++) {
                vector<Point> parts = genParticles(centers_m.row(j), counts[j]);
                particles_m.insert(particles_m.end(), parts.begin(), parts.end());
                running += counts[j];
            }
            
            int rem_count = num_particles - running;
            vector<Point> parts  = genParticles(centers_m.row(n_clusters_m-1), rem_count);
            particles_m.insert(particles_m.end(), parts.begin(), parts.end());
            break;
        }
    }
        
    /*
    # filter particles so that all fit in [0, max_bound) bound
    print(f"before filtering = {self.particles.shape[0]}")
    self.particles = self.filterParticles(self.particles)
    print(f"after filtering = {self.particles.shape[0]}")
    */
}

vector<Point> ParticleGenerator::genParticles(Eigen::VectorXf center, int count) {
    float sigma = max_bound_m / 15;
    vector<Point> points;
    float center_x = center(0);
    float center_y = center(1);
    std::cout << boost::format("center=(%.2f, %.2f), count=%d\n") % center_x % center_y % count;
    for (int i=0; i<count; i++) {
        float x = nd_m.sample() * sigma + center_x;
        float y = nd_m.sample() * sigma + center_y;
        // Filter away out-of-bound particles
        if (0<=x && x<=max_bound_m && 0<=y && y<=max_bound_m)
            points.push_back(Point(x, y));
    }
    return points;
}

Eigen::MatrixXf ParticleGenerator::genClusterCenters() {
    float min_separation = max_bound_m * 1.414 / n_clusters_m;
    Eigen::MatrixXf picks;
    while (true) {
        bool success = true;
        // we leave 10% margin area on each side to be particle-free
        picks = Eigen::MatrixXf::Random(n_clusters_m, 2); // [-1, 1)
        picks.array() += 1;
        picks = picks / 2; // [0, 1)
        picks = picks*max_bound_m*0.8; // N x 2 (x, y)
        picks.array() += max_bound_m*0.1;
        for (unsigned int i=0; i<(unsigned int)n_clusters_m; i++) {
            Eigen::MatrixXf diff = picks.rowwise() - picks.row(i);
            removeRow(diff, i); // bypass self row
            float actual_separation = diff.rowwise().norm().minCoeff();
            if (actual_separation < min_separation) {
                success = false;
                break;
            }
        }
        if (success) break;
    }
    return picks;
}

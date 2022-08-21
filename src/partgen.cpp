#include <iostream>
#include <Eigen/Dense>
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
    
    std::cout << "cluster centers = " << centers_m << std::endl;
    
    // generate particles for each cluster
    /*
    avg_particles = num_particles / n_clusters
    print(f"n_clusters={n_clusters}, avg_particles={avg_particles}")
    while True:
        self.particles = np.empty((0 ,2))
        counts = avg_particles + np.random.randn(n_clusters-1)*avg_particles/6
        if sum(counts)<num_particles:
            # accept this sampling
            running = 0
            for i in range(n_clusters-1):
                count = int(np.round(counts[i]))
                center = self.centers[i,:]
                parts = self.genParticles(center, count)
                self.particles = np.vstack((self.particles, parts))
                running+=count
            
            count = num_particles - running
            center = self.centers[-1,:]
            parts = self.genParticles(center, count)
            self.particles = np.vstack((self.particles, parts))
            
            break
        
    # filter particles so that all fit in [0, max_bound) bound
    print(f"before filtering = {self.particles.shape[0]}")
    self.particles = self.filterParticles(self.particles)
    print(f"after filtering = {self.particles.shape[0]}")
    */
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

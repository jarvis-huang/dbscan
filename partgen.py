import numpy as np
import matplotlib.pyplot as plt
from numpy import linalg as LA

class ParticleGenerator:
    # Generates particles within x:[0, max_bound), y:[0, max_bound)
    def __init__(self, max_bound, num_particles, n_clusters):
        self.side = max_bound
        assert 1 <= n_clusters <= 10, "n_clusters must be 1~10."
        
        # pick cluster centers
        self.centers = self.genClusterCenters(n_clusters)
        self.showCenters(self.centers)
        
        # generate particles for each cluster
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

    def genClusterCenters(self, n_clusters):
        min_separation = self.side * 1.414 / n_clusters
        while True:
            success = True
            # we leave 10% margin area on each side to be particle-free
            picks = self.side*0.1 + np.random.rand(n_clusters, 2)*self.side*0.8 # N x 2 (2 => x, y)
            for i in range(n_clusters):
                diff = picks - picks[i,:]
                diff = np.vstack((diff[:i, :], diff[i+1:, :]))
                d = LA.norm(diff, axis=1)
                if np.min(d) < min_separation:
                    success = False
                    break
            if success:
                break
        return picks
        
    def showCenters(self, centers):
        plt.plot(centers[:,0], centers[:,1], 'ro', markersize=4)
        plt.grid(True)
        plt.axis("square")
        plt.xlim([0, self.side])
        plt.ylim([0, self.side])
        
    # returns (count x 2) array
    def genParticles(self, center, count):
        print(f"center=({center[0]:.2f}, {center[1]:.2f}), count={count}")
        sigma = self.side / 15
        return center + np.random.randn(count, 2) * sigma
    
    
    def filterParticles(self, particles):
        particles = particles[particles[:, 0] > 0]
        particles = particles[particles[:, 0] < self.side]
        particles = particles[particles[:, 1] > 0]
        particles = particles[particles[:, 1] < self.side]
        return particles
        
    def getParticles(self):
        return self.particles
        
        

class ParticleVisualizer:
    def __init__(self, max_bound):
        self.side = max_bound
        self.colors = "krgbcmy"
        
    def showParticles(self, particles):
        fig = plt.figure()
        if particles.shape[1] == 2:  # no cluster info
            plt.plot(particles[:,0], particles[:,1], 'ko', markersize=2)
        else:  # has cluster info
            max_cluster = int(np.max(particles[:, 2]))
            if max_cluster > len(self.colors)-1:
                print(f"Too many clusters ({max_cluster:d}).")
                return
            for particle in particles:
                cluster_id = int(particle[-1])
                c = self.colors[cluster_id]
                plt.plot(particle[0], particle[1], c+'o', markersize=2)
        plt.grid(True)
        plt.axis("square")
        plt.xlim([0, self.side])
        plt.ylim([0, self.side])
        plt.title(f"num_clusters={max_cluster:d}")
        plt.waitforbuttonpress(0)
        plt.close(fig)

if __name__ == "__main__":
    pg = ParticleGenerator(max_bound=10.0, num_particles=100, n_clusters=4)


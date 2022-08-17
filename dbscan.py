import numpy as np
from numpy import linalg as LA
from typing import Callable

# TODO
# - [ ] Use named columns or DataFrame
# - [ ] Create a class for data point with cluster x, y, label
# - [ ] Add unit tests.

def myDistance(p1: np.ndarray, p2: np.ndarray):
    assert p1.ndim == p2.ndim
    if p1.ndim == 1 or p1.ndim == 2:
        assert p1.size == p2.size == 2
        return LA.norm(p1 - p2)
    else:
        assert False, "myDistance(): p1 and p2 can only be 1-d or 2-d."
        return 0.0


def myDistanceL1(p1: np.ndarray, p2: np.ndarray):
    assert p1.ndim == p2.ndim
    if p1.ndim == 1 or p1.ndim == 2:
        assert p1.size == p2.size == 2
        p_diff = np.abs(p1 - p2)
        return np.sum(p_diff)
    else:
        assert False, "myDistance(): p1 and p2 can only be 1-d or 2-d."
        return 0.0
        
        
class DBSCAN:
    def __init__(self, eps: float, minPts: int, distFunc: Callable = None):
        self.eps = eps
        self.minPts = minPts
        self.distFunc = distFunc
        
    def cluster(self, points):
        C = 0
        npts = points.shape[0]
        label_col = -np.ones((npts, 1))
        index_col = np.arange(npts)[:, None]
        # x, y, label, index
        # label (-1: unprocessed, 0: noise, >=1: assigned cluster)
        all_points = np.hstack((points, label_col, index_col))
        #print("starting:\n", all_points)
        for point in all_points:
            label = point[2]
            if label != -1: continue # Previously processed in inner loop
            neighbors = self.rangeQuery(all_points, point) # Find neighbors
            if neighbors.shape[0] < self.minPts: # Density check
                point[2] = 0 # Label as Noise
                continue
            else:
                C += 1 # next cluster label
                point[2] = C # Label initial point
                seedset = neighbors
                while seedset.size > 0: # if not empty, explore & expand seedset
                    q = seedset[0, :]
                    if q[2] <= 0: # Not previously processed
                        q[2] = C # assign cluster label in seedset
                        index = q[-1]
                        all_points[index, 2] = C # assign cluster label in original DB
                        more_neighbors = self.rangeQuery(all_points, q) # Find neighbors
                        if more_neighbors.shape[0] >= self.minPts: # Density check
                            # merge more_neighbors into seedset
                            seedset = np.vstack((seedset, more_neighbors))
                    seedset = seedset[1:, :] # delete current point from seedset
                            
        
    def rangeQuery(self, points: np.ndarray, p: np.ndarray) -> np.ndarray:
        # Dimension sanity checking
        assert p.size == 4
        assert points.ndim == 1 or points.ndim == 2
        if points.ndim == 1:
            assert points.size == 4
        if points.ndim == 2:
            assert points.shape[1] == 4

        # standard L2-norm distance func
        if self.distFunc is None:
            difference = points[:, :2] - p[:2]
            distancia = LA.norm(difference, axis=1)
            return np.squeeze(points[np.argwhere(distancia <= self.eps)])
        # custom distance func
        else:
            points_in_range = np.empty((0, 4))
            for point in points:
                if self.distFunc(point[:2], p[:2]) <= self.eps:
                    points_in_range = np.vstack((points_in_range, point))
            return points_in_range


if __name__ == "__main__":
    np.set_printoptions(suppress=True) # suppress printing in scientific notation
    x = np.empty((10,4))
    x[:, :2] = np.random.rand(10, 2)
    x[:, 2] = -1
    x[:, 3] = range(10)
    print("original\n", x)
    eps = 0.8
    minPts = 5
    dbscan = DBSCAN(eps, minPts)
    points_in_range = dbscan.rangeQuery(x, np.array([0, 0]))
    print("filtered\n", points_in_range)
    #print("added label")
    #dbscan.cluster(x)

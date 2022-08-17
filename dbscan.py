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
        # x, y, label
        # label (-1: unprocessed, 0: noise, >=1: assigned cluster)
        all_points = np.hstack((points, label_col))
        print("starting:\n", all_points)
        for i, point in enumerate(all_points):
            label = point[2]
            if label != -1:
                continue  # Previously processed in inner loop
            neighbors = self.rangeQuery(points, point[:2])  # Find neighbors
            if neighbors.size < self.minPts:  # Density check
                point[2] = 0  # Label as Noise
                continue
            else:
                C += 1  # next cluster label
                point[2] = C  # Label initial point
                # print(f"seed={point[0]:.1f}, {point[1]:.1f}")
                seedset = neighbors
                # print("seedset=\n", seedset)
                while seedset.size > 0:
                    # While not empty, explore & expand seedset
                    q = all_points[seedset[0]]
                    # print(f"q={q[0]:.1f}, {q[1]:.1f}")
                    if q[2] <= 0:  # Not previously processed
                        q[2] = C  # assign cluster label (will also 
                                  # modify all_points)
                        more_neighbors = self.rangeQuery(points, q[:2]) # Find neighbors
                        if more_neighbors.size >= self.minPts: # Density check
                            # merge more_neighbors into seedset
                            seedset = np.append(seedset, more_neighbors)
                    seedset = seedset[1:]  # delete current point from seedset
        print("final:\n", all_points)                

    # Returns indices of neighbors
    def rangeQuery(self, points: np.ndarray, p: np.ndarray) -> np.ndarray:
        # Dimension sanity checking
        assert p.size == 2
        assert points.ndim == 1 or points.ndim == 2
        if points.ndim == 1:
            assert points.size == 2
        if points.ndim == 2:
            assert points.shape[1] == 2

        # standard L2-norm distance func
        if self.distFunc is None:
            difference = points - p
            distancia = LA.norm(difference, axis=1)
            return np.squeeze(np.argwhere(distancia <= self.eps))
        # custom distance func
        else:
            points_in_range = np.empty(0)
            for i, point in enumerate(points):
                if self.distFunc(point, p) <= self.eps:
                    points_in_range = np.append(points_in_range, i)
            return points_in_range


if __name__ == "__main__":
    # suppress printing in scientific notation
    np.set_printoptions(suppress=True)
    
    # x = np.random.rand(10, 2)
    x = np.array([[0, 0], [0.1, 0.1], [0.2, 0.2],
                  [1, 1], [1.1, 1.1], [1.2, 1.2],
                  [2, 2], [2.1, 2.1], [2.2, 2.2], [1.9, 1.9]])
    # print("original\n", x)
    eps = 0.5
    minPts = 2
    dbscan = DBSCAN(eps, minPts)
    # points_in_range = dbscan.rangeQuery(x, np.array([2, 2))
    # print("filtered\n", points_in_range)
    dbscan.cluster(x)

import numpy as np
from numpy import linalg as LA
from typing import Callable


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
        pass
        
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
            return np.squeeze(points[np.argwhere(distancia <= self.eps)])
        # custom distance func
        else:
            points_in_range = np.empty((0, 2))
            for point in points:
                if self.distFunc(point, p) <= self.eps:
                    points_in_range = np.vstack((points_in_range, point))
            return points_in_range


if __name__ == "__main__":
    x = np.random.rand(10, 2)
    print("original\n", x)
    eps = 0.8
    minPts = 10
    dbscan = DBSCAN(eps, minPts)
    points_in_range = dbscan.rangeQuery(x, np.array([0, 0]))
    print("filtered\n", points_in_range)

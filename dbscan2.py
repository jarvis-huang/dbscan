import numpy as np
from numpy import linalg as LA
from typing import Callable

def myDistance(p1: np.ndarray, p2: np.ndarray):
    assert p1.ndim==p2.ndim
    if p1.ndim==1 or p1.ndim==2:
        assert p1.size == p2.size == 2
        return LA.norm(p1 - p2)
    else:
        assert False, "myDistance(): p1 and p2 can only be 1-d or 2-d."
        return 0.0
    

def rangeQuery(points: np.ndarray, p: np.ndarray,
               eps: float, distFunc: Callable) -> np.ndarray:
    assert p.size == 2
    if distFunc is None:
        difference = points - p
        distancia = LA.norm(difference, axis=1)
    return np.squeeze(points[np.argwhere(distancia<=eps)])
    

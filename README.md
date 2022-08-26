# dbscan
my implementation of DBSCAN clustering algorithm

# OpenCL Strategy
1. Host memory holds all points. each point holds 3 floats (x, y and cid).
2. Copy host memory to device memory.
3. kernel computes distance from all points to one point_ref.
4. kernel modifies the cid in place.
5. Repeatedly call kernel with different point_ref.

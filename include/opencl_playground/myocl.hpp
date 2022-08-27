#ifndef MYOCL_H
#define MYOCL_H

#include <CL/cl.hpp>
#include <chrono>    // for time measurement
#include <fstream>   // for file I/O
#include <iostream>  // for printing
#include "opencl_playground/err_code.h"

using namespace std::chrono;

namespace util {
inline void checkErr(cl_int err, const char* name) {
  if (err != CL_SUCCESS) {
    std::cerr << "ERROR: " << name << " (" << err << ")" << std::endl;
    exit(EXIT_FAILURE);
  }
}

std::vector<cl::Platform> getAllPlatforms();
cl::Device getDevice(std::string device_type);
cl::Program makeProgramFromKernelCode(const char* filename,
                                      cl::Context context);

class Timer {
 public:
  Timer() {}
  void Tic() { t0 = high_resolution_clock::now(); }
  void Toc() { t1 = high_resolution_clock::now(); }
  double ElapsedSec() {
    int t_ms = duration_cast<milliseconds>(t1 - t0).count();
    return double(t_ms) / 1000.0;
  }
  int ElapsedMs() { return duration_cast<milliseconds>(t1 - t0).count(); }
  int ElapsedUs() { return duration_cast<microseconds>(t1 - t0).count(); }

 private:
  std::chrono::time_point<high_resolution_clock> t0, t1;
};
}  // namespace util

#endif  // MYOCL_H

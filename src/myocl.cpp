#include "opencl_playground/myocl.hpp"

namespace util {
std::vector<cl::Platform> getAllPlatforms() {
  // get all platforms (drivers)
  std::vector<cl::Platform> all_platforms;
  cl::Platform::get(&all_platforms);
  checkErr(all_platforms.size() != 0 ? CL_SUCCESS : -1,
           "No platforms found. Check OpenCL installation!");
  return all_platforms;
}

// Version to use default platform to get device
cl::Device getDevice(std::string device_type) {
  std::vector<cl::Device> all_devices;
  std::vector<cl::Platform> all_platforms = getAllPlatforms();
  uint32_t dev_type = CL_DEVICE_TYPE_CPU;
  if (device_type == "CPU") {
    dev_type = CL_DEVICE_TYPE_CPU;
  } else if (device_type == "GPU") {
    dev_type = CL_DEVICE_TYPE_GPU;
  } else {
    std::cerr << "ERROR: unknown device type: " << device_type << std::endl;
    exit(1);
  }
  for (auto& platform : all_platforms) {
    platform.getDevices(dev_type, &all_devices);
    if (all_devices.size() != 0) {
      std::cout << "Using platform: " << platform.getInfo<CL_PLATFORM_NAME>()
                << "\n";
      break;
    }
  }
  checkErr(all_devices.size() != 0 ? CL_SUCCESS : -1,
           "No devices found. Check OpenCL installation!");
  cl::Device default_device = all_devices[0];
  auto max_work_items = default_device.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
  std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>()
            << " (" << max_work_items[0] << ", " << max_work_items[1] << ", "
            << max_work_items[2] << ")\n";
  return default_device;
}

cl::Program makeProgramFromKernelCode(const char* filename,
                                      cl::Context context) {
  // read kernel code from disk, convert it to a string
  std::ifstream f_kernel(filename);
  checkErr(f_kernel.is_open() ? CL_SUCCESS : -1, filename);
  std::string kernel_code(std::istreambuf_iterator<char>(f_kernel),
                          (std::istreambuf_iterator<char>()));

  cl_int err;
  cl::Program program(context, kernel_code, true,
                      &err);  // builds (compiles and links) the kernel code

  if (err != CL_SUCCESS) {
    std::cout << " Error building: " << filename << "\n";
    // Print detailed compile error msg:
    std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
    std::cout << " A: " << devices.size() << std::endl;
    std::string log = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]);
    std::cout << " B\n";
    std::cerr << log << std::endl;
    std::cout << " C\n";
    exit(1);
  }

  return program;
}
}  // namespace util

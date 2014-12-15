
#ifdef USE_OPENCL

clReleaseMemObject(ocl_input);
clReleaseMemObject(ocl_output);
clReleaseProgram(ocl_program);
clReleaseKernel(ocl_kernel);
clReleaseCommandQueue(ocl_commands);
clReleaseContext(ocl_context);

#endif

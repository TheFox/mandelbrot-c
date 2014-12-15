
// Write our data set into the input array in device memory 
printf("OpenCL write our data set into the input array\n");
ocl_error = clEnqueueWriteBuffer(ocl_commands, ocl_input, CL_TRUE, 0, ocl_count * sizeof(float), ocl_data, 0, NULL, NULL);
if(ocl_error != CL_SUCCESS){
	printf("OpenCL ERROR: Failed to write to source array. %d\n", ocl_error);
	return EXIT_FAILURE;
}

// Set the arguments to our compute kernel
printf("OpenCL set the arguments\n");
ocl_error = 0;
ocl_error = clSetKernelArg(ocl_kernel, 0, sizeof(cl_mem), &ocl_input);
if(ocl_error != CL_SUCCESS){
	printf("OpenCL ERROR: Failed to set kernel arguments. 0 %d\n", ocl_error);
	return EXIT_FAILURE;
}
ocl_error |= clSetKernelArg(ocl_kernel, 1, sizeof(cl_mem), &ocl_output);
if(ocl_error != CL_SUCCESS){
	printf("OpenCL ERROR: Failed to set kernel arguments. 1 %d\n", ocl_error);
	return EXIT_FAILURE;
}
ocl_error |= clSetKernelArg(ocl_kernel, 2, sizeof(unsigned int), &ocl_count);
if(ocl_error != CL_SUCCESS){
	printf("OpenCL ERROR: Failed to set kernel arguments. 2 %d\n", ocl_error);
	return EXIT_FAILURE;
}

// Get the maximum work group size for executing the kernel on the device
printf("OpenCL get maximum work group size\n");
size_t ocl_local;
ocl_error = clGetKernelWorkGroupInfo(ocl_kernel, ocl_device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(ocl_local), &ocl_local, NULL);
if(ocl_error != CL_SUCCESS){
	printf("OpenCL ERROR: Failed to retrieve kernel work group info. %d\n", ocl_error);
	return EXIT_FAILURE;
}

// Execute the kernel over the entire range of our 1d input data set
// using the maximum number of work group items for this device
size_t ocl_global = ocl_count;
printf("OpenCL execute the kernel: %zu, %zu\n", ocl_global, ocl_local);
ocl_error = clEnqueueNDRangeKernel(ocl_commands, ocl_kernel, 1, NULL, &ocl_global, &ocl_local, 0, NULL, NULL);
if(ocl_error){
	printf("OpenCL ERROR: Failed to execute kernel. %d\n", ocl_error);
	return EXIT_FAILURE;
}

// Wait for the command commands to get serviced before reading back results
printf("OpenCL wait...\n");
clFinish(ocl_commands);
printf("OpenCL wait done\n");

// Read back the results from the device to verify the output
printf("OpenCL malloc results\n");
float *ocl_results = (float *)malloc(ocl_count * sizeof(float));

printf("OpenCL read back\n");
ocl_error = clEnqueueReadBuffer(ocl_commands, ocl_output, CL_TRUE, 0, ocl_count * sizeof(float), ocl_results, 0, NULL, NULL);  
if(ocl_error != CL_SUCCESS){
	printf("OpenCL ERROR: Failed to read output array. %d\n", ocl_error);
	return EXIT_FAILURE;
}

// Validate our results
printf("OpenCL validate\n");
unsigned int ocl_correct = 0;
for(ocl_i = 0; ocl_i < ocl_count; ocl_i++){
	if(ocl_results[ocl_i] == ocl_data[ocl_i] * ocl_data[ocl_i]){
		ocl_correct++;
	}
}
printf("OpenCL validate done\n");

// Print a brief summary detailing the results
printf("OpenCL computed %d/%d correct values\n", ocl_correct, ocl_count);

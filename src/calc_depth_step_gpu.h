
// http://stackoverflow.com/questions/7001424/opencl-problem-with-double-type
#ifdef cl_khr_fp64
	#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#elif defined(cl_amd_fp64)
	#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#else
	//#error "Double precision floating point not supported by OpenCL implementation."
#endif

puts("OpenCL is active");

int ocl_error;

char *ocl_file_path = (char *)malloc(4096);
strcpy(ocl_file_path, program_dirname);
#ifdef USE_OPENCL_BINARY
	strcat(ocl_file_path, "/kernel.s");
#else
	strcat(ocl_file_path, "/kernel.cl");
#endif

size_t ocl_file_size = 0;
char *ocl_kernel_source = NULL;
char *ocl_kernel_source_write = NULL;

char *ocl_buffer = (char *)malloc(4096);
memset(ocl_buffer, 0, 4096);

printf("OpenCl load: '%s'\n", ocl_file_path);
FILE *ocl_file = fopen(ocl_file_path, "r");
if(ocl_file){
	fseek(ocl_file, 0L, SEEK_END);
	ocl_file_size = ftell(ocl_file);
	fseek(ocl_file, 0L, SEEK_SET);
	
	ocl_kernel_source = (char *)malloc(ocl_file_size + 1);
	if (ocl_kernel_source == NULL) {
		printf("ERROR: cant malloc ocl_kernel_source\n");
		return EXIT_FAILURE;
	}
	memset(ocl_kernel_source, 0, ocl_file_size + 1);
	ocl_kernel_source_write = ocl_kernel_source;
	
	printf("OpenCL ocl_file_size = %lu\n", ocl_file_size);
	
	size_t ocl_nread;
	while((ocl_nread = fread(ocl_buffer, 1, 4096, ocl_file)) > 0){
		memcpy(ocl_kernel_source_write, ocl_buffer, ocl_nread);
		ocl_kernel_source_write += ocl_nread;
	}
	fclose(ocl_file);
}

if(!ocl_kernel_source){
	printf("OpenCL ERROR: can't read kernel source code\n");
	return EXIT_FAILURE;
}

// Fill our data set with random float values
int ocl_i = 0;
unsigned int ocl_count = DATA_SIZE;
printf("OpenCL malloc data\n");
float *ocl_data = (float *)malloc(DATA_SIZE * sizeof(float));

printf("OpenCL fill data %ld\n", (unsigned long)DATA_SIZE);
for(ocl_i = 0; ocl_i < ocl_count; ocl_i++){
	ocl_data[ocl_i] = rand() / (float)RAND_MAX;
}
printf("OpenCL fill data done\n");

// Connect to a compute device
printf("OpenCL find device\n");
cl_device_id ocl_device_id;
ocl_error = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &ocl_device_id, NULL);
if(ocl_error != CL_SUCCESS){
	printf("OpenCL WARNING: use failover, CPU. %d\n", ocl_error);
	ocl_error = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_CPU, 1, &ocl_device_id, NULL);
	if(ocl_error != CL_SUCCESS){
		printf("OpenCL ERROR: Failed to create a device group. %d\n", ocl_error);
		return EXIT_FAILURE;
	}
}
printf("OpenCL device: %p\n", ocl_device_id);

char *ocl_device_name = (char *)malloc(512);
clGetDeviceInfo(ocl_device_id, CL_DEVICE_NAME, 512, ocl_device_name, NULL);
printf("OpenCL ocl_device_name: '%s'\n", ocl_device_name);

// Create a compute context
printf("OpenCL create a compute context\n");
cl_context ocl_context = clCreateContext(0, 1, &ocl_device_id, NULL, NULL, &ocl_error);
if(!ocl_context){
	printf("OpenCL ERROR: Failed to create a compute context. %d\n", ocl_error);
	return EXIT_FAILURE;
}
printf("OpenCL context: %p\n", ocl_context);

// Create a command commands
printf("OpenCL create a command commands\n");
cl_command_queue ocl_commands = clCreateCommandQueue(ocl_context, ocl_device_id, 0, &ocl_error);
if(!ocl_commands){
	printf("OpenCL ERROR: Failed to create a command commands. %d\n", ocl_error);
	return EXIT_FAILURE;
}
printf("OpenCL commands: %p\n", ocl_commands);

printf("OpenCL create program: %lu\n", ocl_file_size);
cl_int ocl_binary_status = 0;
cl_program ocl_program;

#ifdef USE_OPENCL_BINARY
	//ocl_program = clCreateProgramWithBinary(ocl_context, 1, &ocl_device_id, (const size_t *)&ocl_binary_size, (const unsigned char **)&ocl_binary_buf, &ocl_binary_status, &ocl_error);
	ocl_program = clCreateProgramWithBinary(ocl_context, 1, &ocl_device_id, (const size_t *)&ocl_file_size, (const unsigned char **)&ocl_kernel_source, &ocl_binary_status, &ocl_error);
#else
	ocl_program = clCreateProgramWithSource(ocl_context, 1, (const char **)&ocl_kernel_source, NULL, &ocl_error);
#endif

if(!ocl_program){
	printf("OpenCL ERROR: Failed to create compute program. %d %d\n", ocl_binary_status, ocl_error);
	switch(ocl_binary_status){
		case CL_INVALID_BINARY:
			puts("\t CL_INVALID_BINARY");
			break;
	}
	switch(ocl_error){
		case CL_INVALID_VALUE:
			puts("\t CL_INVALID_VALUE");
			break;
	}
	return EXIT_FAILURE;
}
printf("OpenCL program: %d %d\n", ocl_binary_status, ocl_error);

// Build the program executable
printf("OpenCL build the program executable\n");
const char *options = "-I /usr/include";
ocl_error = clBuildProgram(ocl_program, 0, NULL, options, &pfn_notify, NULL);
//ocl_error = clBuildProgram(ocl_program, 1, &ocl_device_id, options, &pfn_notify, NULL);
printf("OpenCL build the program executable done\n");
if(ocl_error != CL_SUCCESS){
	size_t ocl_len;
	memset(ocl_buffer, 0, 4096);
	
	printf("OpenCL ERROR: Failed to build program executable. %d\n", ocl_error);
	switch(ocl_error){
		case CL_BUILD_PROGRAM_FAILURE:
			puts("\t CL_BUILD_PROGRAM_FAILURE");
			break;
	}
	//clGetProgramBuildInfo(ocl_program, ocl_device_id, CL_PROGRAM_BUILD_LOG, sizeof(ocl_buffer), ocl_buffer, &ocl_len);
	clGetProgramBuildInfo(ocl_program, ocl_device_id, CL_PROGRAM_BUILD_LOG, 4096, ocl_buffer, &ocl_len);
	//clGetProgramBuildInfo(ocl_program, 0, CL_PROGRAM_BUILD_LOG, sizeof(ocl_buffer), ocl_buffer, &ocl_len);
	printf("info: '%s'\n", ocl_buffer);
	return EXIT_FAILURE;
}

// Create the compute kernel in the program we wish to run
printf("OpenCL create kernel\n");
cl_kernel ocl_kernel = clCreateKernel(ocl_program, "square", &ocl_error);
if(!ocl_kernel || ocl_error != CL_SUCCESS){
	printf("OpenCL ERROR: Failed to create compute kernel. %d\n", ocl_error);
	return EXIT_FAILURE;
}

// Create the input and output arrays in device memory for our calculation
printf("OpenCL create the input and output\n");
cl_mem ocl_input = clCreateBuffer(ocl_context, CL_MEM_READ_ONLY, sizeof(float) * ocl_count, NULL, &ocl_error);
if(!ocl_input){
	printf("OpenCL ERROR: Failed to allocate input device memory. %d\n", ocl_error);
	return EXIT_FAILURE;
}
cl_mem ocl_output = clCreateBuffer(ocl_context, CL_MEM_WRITE_ONLY, sizeof(float) * ocl_count, NULL, &ocl_error);
if(!ocl_output){
	printf("OpenCL ERROR: Failed to allocate output device memory. %d\n", ocl_error);
	return EXIT_FAILURE;
}

// Write our data set into the input array in device memory 
printf("OpenCL write our data set into the input array\n");
ocl_error = clEnqueueWriteBuffer(ocl_commands, ocl_input, CL_TRUE, 0, sizeof(float) * ocl_count, ocl_data, 0, NULL, NULL);
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
	printf("OpenCL ERROR: Failed to execute kernel\n");
	return EXIT_FAILURE;
}

// Wait for the command commands to get serviced before reading back results
printf("OpenCL wait...\n");
clFinish(ocl_commands);
printf("OpenCL wait done\n");

// Read back the results from the device to verify the output
printf("OpenCL malloc results\n");
float *ocl_results = (float *)malloc(DATA_SIZE * sizeof(float));

printf("OpenCL read back\n");
ocl_error = clEnqueueReadBuffer(ocl_commands, ocl_output, CL_TRUE, 0, sizeof(float) * ocl_count, ocl_results, 0, NULL, NULL);  
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
clReleaseMemObject(ocl_input);
clReleaseMemObject(ocl_output);
clReleaseProgram(ocl_program);
clReleaseKernel(ocl_kernel);
clReleaseCommandQueue(ocl_commands);
clReleaseContext(ocl_context);
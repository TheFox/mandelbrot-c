
#include "mandelbrot.hpp"

int main(int argc, char const *argv[]){
	/*
	Imlib_Image img1;
	img1 = imlib_create_image(15, 15);
	imlib_context_set_image(img1);
	//imlib_image_set_has_alpha(1);
	
	int o = 2;
	int test_y = 15-o;
	
	imlib_context_set_color(255, 0, 0, 255);
	imlib_image_draw_line(0, test_y, 0, test_y, 0);
	
	
	Imlib_Color color;
	imlib_image_query_pixel(0, 0, &color);
	
	printf("pix: %d %d %d %d\n", color.red, color.green, color.blue, color.alpha);
	
	imlib_save_image("pic.png");
	
	return 0;
	*/
	
	
	print_copyright();
	
	if(argc <= 8){
		printf("Usage: %s I_WIDTH I_HEIGHT DEPTH_MIN DEPTH_MAX MB_WIDTH_MID MB_WIDTH_ZOOM MB_HEIGHT_MID MB_HEIGHT_MAX\n", *argv);
		return 1;
	}
	
	const int image_width = atof(argv[1]);
	const int image_height = atof(argv[2]);
	const int depth_min = atof(argv[3]);
	const int depth_max = atof(argv[4]);
	const mbnum mb_width_mid = atof(argv[5]);
	const mbnum mb_width_zoom_org = atof(argv[6]);
	const mbnum mb_height_mid = atof(argv[7]);
	const mbnum mb_height_zoom_org = atof(argv[8]);
	
#ifdef USE_OPENMP
	puts("OpenMP is active");
	omp_set_num_threads(OPENMP_NUM_THREADS);
	omp_set_dynamic(0);
#endif
	
#include "init_default_vars.h"
	
	char *start_time_text = (char *)malloc(128);
	memset(start_time_text, 0, 128);
	time_t start_time = time(NULL);
	struct tm *start_time_tm = localtime(&start_time);
	strftime(start_time_text, 128, "S time: %F %T %z %Z", start_time_tm);
	
	print_config(image_width, image_width_mid, image_height, image_height_mid, color_diff, depth_min, depth_max, depth_diff, depth_step, mb_width_mid, mb_width_zoom_org, mb_width_min, mb_width_max, mb_width_step, mb_height_mid, mb_height_zoom_org, mb_height_min, mb_height_max, mb_height_step);
	
	printf("find width 0 point\n");
	float image_width_mb_iter = 0;
	int image_width_mb_0 = 0;
	if(mb_width_max > 0)
		for(image_width_mb_iter = mb_width_min; image_width_mb_iter <= 0; image_width_mb_iter += mb_width_step)
			//printf("image_width_mb_iter: %d %f\n", image_width_mb_0, image_width_mb_iter);
			image_width_mb_0++;
	
	printf("find height 0 point\n");
	float image_height_mb_iter = 0;
	int image_height_mb_0 = 0;
	if(mb_height_max > 0)
		for(image_height_mb_iter = mb_height_max; image_height_mb_iter >= 0; image_height_mb_iter -= mb_height_step)
			//printf("y iter: %d %f\n", image_height_mb_0, image_height_mb_iter);
			image_height_mb_0++;
	
	printf("mb_0 x: %d %f\n", image_width_mb_0, image_width_mb_iter);
	printf("mb_0 y: %d %f\n", image_height_mb_0, image_height_mb_iter);
	
#ifdef USE_OPENCL
	puts("OpenCL is active");
	
	int ocl_err;
	
	//size_t ocl_global;
	//size_t ocl_local;
	
	//cl_context ocl_context;
	//cl_command_queue ocl_commands;
	//cl_program ocl_program;
	//cl_kernel ocl_kernel;
	//cl_mem ocl_input;
	//cl_mem ocl_output;
	
	
	char the_path[256];

	getcwd(the_path, 255);
	printf("path: '%s'\n", the_path);
	
	
	//const char *ocl_file_path = "build_Darwin/bin/kernel.s";
	const char *ocl_file_path = "kernel.s";
	
	size_t ocl_file_size = 0;
	char *ocl_kernel_source = NULL;
	char *ocl_kernel_source_write = NULL;
	
	char *ocl_buffer = (char *)malloc(4096);
	memset(ocl_buffer, 0, 4096);
	
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
		printf("OpenCL ERROR: cant read kernel source code\n");
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
	cl_device_id ocl_device_id;
	ocl_err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &ocl_device_id, NULL);
	//ocl_err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_CPU, 1, &ocl_device_id, NULL);
	if(ocl_err != CL_SUCCESS){
		ocl_err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_CPU, 1, &ocl_device_id, NULL);
		if(ocl_err != CL_SUCCESS){
			printf("OpenCL ERROR: Failed to create a device group\n");
			return EXIT_FAILURE;
		}
	}
	
	char *ocl_device_name = (char *)malloc(512);
	clGetDeviceInfo(ocl_device_id, CL_DEVICE_NAME, 512, ocl_device_name, NULL);
	printf("OpenCL ocl_device_name: '%s'\n", ocl_device_name);
	
	// Create a compute context
	printf("OpenCL create a compute context\n");
	cl_context ocl_context = clCreateContext(0, 1, &ocl_device_id, NULL, NULL, &ocl_err);
	if(!ocl_context){
		printf("OpenCL ERROR: Failed to create a compute context\n");
		return EXIT_FAILURE;
	}
	
	// Create a command commands
	printf("OpenCL create a command commands\n");
	cl_command_queue ocl_commands = clCreateCommandQueue(ocl_context, ocl_device_id, 0, &ocl_err);
	if(!ocl_commands){
		printf("OpenCL ERROR: Failed to create a command commands\n");
		return EXIT_FAILURE;
	}
	
	/*
	
	char *ocl_binary_buf;
	cl_int ocl_binary_status;
	fp = fopen(fileName, "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	ocl_binary_buf = (char *)malloc(MAX_BINARY_SIZE);
	binary_size = fread(ocl_binary_buf, 1, MAX_BINARY_SIZE, fp);
	fclose(fp);
	
	*/
	
	printf("OpenCL create program\n");
	cl_int ocl_binary_status;
	cl_program ocl_program;
	//ocl_program = clCreateProgramWithSource(ocl_context, 1, (const char **)&ocl_kernel_source, NULL, &ocl_err);
	//ocl_program = clCreateProgramWithBinary(ocl_context, 1, &ocl_device_id, (const size_t *)&ocl_binary_size, (const unsigned char **)&ocl_binary_buf, &ocl_binary_status, &ocl_err);
	ocl_program = clCreateProgramWithBinary(ocl_context, 1, &ocl_device_id, (const size_t *)&ocl_file_size, (const unsigned char **)&ocl_kernel_source, &ocl_binary_status, &ocl_err);
	if(!ocl_program){
		printf("OpenCL ERROR: Failed to create compute program. %d\n", ocl_err);
		return EXIT_FAILURE;
	}
	printf("OpenCL create program: %d %d\n", ocl_binary_status, ocl_err);
	
	// Build the program executable
	printf("OpenCL build the program executable\n");
	ocl_err = clBuildProgram(ocl_program, 0, NULL, NULL, NULL, NULL);
	if(ocl_err != CL_SUCCESS){
		size_t ocl_len;
		memset(ocl_buffer, 0, 4096);
		
		printf("OpenCL ERROR: Failed to build program executable\n");
		clGetProgramBuildInfo(ocl_program, ocl_device_id, CL_PROGRAM_BUILD_LOG, sizeof(ocl_buffer), ocl_buffer, &ocl_len);
		printf("%s\n", ocl_buffer);
		return EXIT_FAILURE;
	}
	
	// Create the compute kernel in the program we wish to run
	printf("OpenCL create kernel\n");
	cl_kernel ocl_kernel = clCreateKernel(ocl_program, "square", &ocl_err);
	if(!ocl_kernel || ocl_err != CL_SUCCESS){
		printf("OpenCL ERROR: Failed to create compute kernel\n");
		return EXIT_FAILURE;
	}
	
	// Create the input and output arrays in device memory for our calculation
	printf("OpenCL create the input and output\n");
	cl_mem ocl_input = clCreateBuffer(ocl_context, CL_MEM_READ_ONLY, sizeof(float) * ocl_count, NULL, NULL);
	cl_mem ocl_output = clCreateBuffer(ocl_context, CL_MEM_WRITE_ONLY, sizeof(float) * ocl_count, NULL, NULL);
	if(!ocl_input || !ocl_output){
		printf("OpenCL ERROR: Failed to allocate device memory\n");
		return EXIT_FAILURE;
	}
	
	// Write our data set into the input array in device memory 
	printf("OpenCL write our data set into the input array\n");
	ocl_err = clEnqueueWriteBuffer(ocl_commands, ocl_input, CL_TRUE, 0, sizeof(float) * ocl_count, ocl_data, 0, NULL, NULL);
	if(ocl_err != CL_SUCCESS){
		printf("OpenCL ERROR: Failed to write to source array\n");
		return EXIT_FAILURE;
	}
	
	// Set the arguments to our compute kernel
	printf("OpenCL set the arguments\n");
	ocl_err = 0;
	ocl_err = clSetKernelArg(ocl_kernel, 0, sizeof(cl_mem), &ocl_input);
	if(ocl_err != CL_SUCCESS){
		printf("OpenCL ERROR: Failed to set kernel arguments. 0 %d\n", ocl_err);
		return EXIT_FAILURE;
	}
	ocl_err |= clSetKernelArg(ocl_kernel, 1, sizeof(cl_mem), &ocl_output);
	if(ocl_err != CL_SUCCESS){
		printf("OpenCL ERROR: Failed to set kernel arguments. 1 %d\n", ocl_err);
		return EXIT_FAILURE;
	}
	ocl_err |= clSetKernelArg(ocl_kernel, 2, sizeof(unsigned int), &ocl_count);
	if(ocl_err != CL_SUCCESS){
		printf("OpenCL ERROR: Failed to set kernel arguments. 2 %d\n", ocl_err);
		return EXIT_FAILURE;
	}
	
	// Get the maximum work group size for executing the kernel on the device
	printf("OpenCL get maximum work group size\n");
	size_t ocl_local;
	ocl_err = clGetKernelWorkGroupInfo(ocl_kernel, ocl_device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(ocl_local), &ocl_local, NULL);
	if(ocl_err != CL_SUCCESS){
		printf("OpenCL ERROR: Failed to retrieve kernel work group info. %d\n", ocl_err);
		return EXIT_FAILURE;
	}
	
	// Execute the kernel over the entire range of our 1d input data set
	// using the maximum number of work group items for this device
	size_t ocl_global = ocl_count;
	printf("OpenCL execute the kernel: %zu, %zu\n", ocl_global, ocl_local);
	ocl_err = clEnqueueNDRangeKernel(ocl_commands, ocl_kernel, 1, NULL, &ocl_global, &ocl_local, 0, NULL, NULL);
	if(ocl_err){
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
	ocl_err = clEnqueueReadBuffer(ocl_commands, ocl_output, CL_TRUE, 0, sizeof(float) * ocl_count, ocl_results, 0, NULL, NULL);  
	if(ocl_err != CL_SUCCESS){
		printf("OpenCL ERROR: Failed to read output array. %d\n", ocl_err);
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
	
	// Print a brief summary detailing the results
	printf("OpenCL computed %d/%d correct values\n", ocl_correct, ocl_count);
	clReleaseMemObject(ocl_input);
	clReleaseMemObject(ocl_output);
	clReleaseProgram(ocl_program);
	clReleaseKernel(ocl_kernel);
	clReleaseCommandQueue(ocl_commands);
	clReleaseContext(ocl_context);
	
	return 0;
#endif
	
#include "init_image_plain.h"
#include "init_mb_xy_grid.h"
	
	puts("start");
	
	float depth_percent = 0.0;
	int depth_base = depth_min;
	for(int depth_i = 0; depth_i <= depth_diff; depth_i++){
		
#ifdef DEBUG
		printf("  depth_i: %d/%d %d %f \r", depth_i, depth_diff, depth_base, depth_percent);
		fflush(stdout);
#endif
		
#include "calc_depth_step.h"
		
		depth_base++;
		depth_percent += depth_step;
	}
	puts("");
	
#include "draw_image.h"
	
	//sleep(10);
	
	puts("end");
	return EXIT_SUCCESS;
}

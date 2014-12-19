
ocl_error |= clSetKernelArg(ocl_kernel, 4, sizeof(int), &depth_base);
if(ocl_error != CL_SUCCESS){
	printf("OpenCL ERROR: Failed to set kernel arguments. 4 %d\n", ocl_error);
	switch(ocl_error){
		case CL_INVALID_ARG_INDEX:
			puts("\t CL_INVALID_ARG_INDEX");
			break;
	}
	return EXIT_FAILURE;
}

for(pos_x = 0; pos_x < image_width; pos_x++){
	const mbnum_t mb_x = mb_x_grid_r[pos_x];
	
	//printf("\t\t x %d\n", pos_x);
	
	ocl_error = clSetKernelArg(ocl_kernel, 5, sizeof(mbnum_t), &mb_x);
	if(ocl_error != CL_SUCCESS){
		printf("OpenCL ERROR: Failed to set kernel arguments. 5 %d\n", ocl_error);
		switch(ocl_error){
			case CL_INVALID_ARG_INDEX:
				puts("\t CL_INVALID_ARG_INDEX");
				break;
		}
		return EXIT_FAILURE;
	}

	// Write our data set into the input array in device memory 
	//printf("OpenCL write our data set into the input array\n");
	ocl_error = clEnqueueWriteBuffer(ocl_commands, ocl_input, CL_TRUE, 0, ocl_count_ts, mb_y_grid_r, 0, NULL, NULL);
	if(ocl_error != CL_SUCCESS){
		printf("OpenCL ERROR: Failed to write to source array. %d\n", ocl_error);
		return EXIT_FAILURE;
	}
	
	// Execute the kernel over the entire range of our 1d input data set
	// using the maximum number of work group items for this device
	size_t ocl_global = ocl_count;
	//printf("OpenCL execute the kernel: %zu, %zu\n", ocl_global, ocl_local);
	ocl_error = clEnqueueNDRangeKernel(ocl_commands, ocl_kernel, 1, NULL, &ocl_global, &ocl_local, 0, NULL, NULL);
	if(ocl_error){
		printf("OpenCL ERROR: Failed to execute kernel. %d\n", ocl_error);
		return EXIT_FAILURE;
	}
	
	// Wait for the command commands to get serviced before reading back results
	//printf("OpenCL wait...\n");
	clFinish(ocl_commands);
	//printf("OpenCL wait done\n");
	
	// Read back the results from the device to verify the output
	//printf("OpenCL read back\n");
	memset(ocl_results, 0, ocl_count_ts);
	ocl_error = clEnqueueReadBuffer(ocl_commands, ocl_output, CL_TRUE, 0, ocl_count_ts, ocl_results, 0, NULL, NULL);  
	if(ocl_error != CL_SUCCESS){
		printf("OpenCL ERROR: Failed to read output array. %d\n", ocl_error);
		return EXIT_FAILURE;
	}
	
	// Validate our results
	//printf("OpenCL validate\n");
	//unsigned int ocl_correct = 0;
	
	for(pos_y = 0; pos_y < image_height; pos_y++){
		/*
		const mbnum_t mb_y = mb_y_grid_r[pos_y];
		const int point_depth = point_iteration(mb_x, mb_y, depth_max);
		char point_iteration_b = 0;
		if(point_depth > depth_base)
			point_iteration_b = 1;
		
		printf("\t\t\t y %d    %c  %d = % .2f\n", pos_y, point_iteration_b == 1 ? 'Y' : 'n', point_depth, ocl_results[pos_y]);
		*/
		
		//printf("\t\t\t y %d    %d\n", pos_y, ocl_results[pos_y]);
		//if((float)point_depth == ocl_results[pos_y]) ocl_correct++;
		//if(ocl_results[pos_y] == 1.0) image_plain[pos_x][pos_y] = depth_percent;
		if(ocl_results[pos_y]) image_plain[pos_x][pos_y] = depth_percent;
	}
	//printf("OpenCL validate done\n");

	// Print a brief summary detailing the results
	//printf("OpenCL computed %d/%d correct values\n", ocl_correct, ocl_count);
	//if(ocl_correct < ocl_count) calc_errors++;
	
	//break;
}

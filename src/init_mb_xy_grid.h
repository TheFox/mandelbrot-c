#ifdef USE_MB_XY_GRID
	size_t mb_x_grid_s = image_width * mbnum_s;
	size_t mb_y_grid_s = image_height * mbnum_s;
	mbnum *mb_x_grid_w = (mbnum *)malloc(mb_x_grid_s);
	mbnum *mb_y_grid_w = (mbnum *)malloc(mb_y_grid_s);
	printf("mb_x_grid_s: %lu\n", mb_x_grid_s);
	printf("mb_y_grid_s: %lu\n", mb_y_grid_s);
	
	#pragma omp parallel for
	for(pos_x = 0; pos_x < image_width; pos_x++)
		mb_x_grid_w[pos_x] = mb_width_min + mb_width_step * pos_x;
	
	#pragma omp parallel for
	for(pos_y = 0; pos_y < image_height; pos_y++)
		mb_y_grid_w[pos_y] = mb_height_min + mb_height_step * (image_height - pos_y);
	
	const mbnum *mb_x_grid_r = (const mbnum *)mb_x_grid_w;
	const mbnum *mb_y_grid_r = (const mbnum *)mb_y_grid_w;
#endif
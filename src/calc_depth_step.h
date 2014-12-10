
for(pos_x = 0; pos_x < image_width; pos_x++){
#ifdef USE_MB_XY_GRID
	const mbnum mb_x = mb_x_grid_r[pos_x];
#else
	const mbnum mb_x = mb_width_min + mb_width_step * pos_x;
#endif
	
	//printf("\t mb_x: %d %f\n", pos_x, mb_x);

#ifdef USE_OPENMP
	#pragma omp parallel for
#endif
	for(pos_y = 0; pos_y < image_height; pos_y++){
#ifdef USE_MB_XY_GRID
		const mbnum mb_y = mb_y_grid_r[pos_y];
#else
		const mbnum mb_y = mb_height_min + mb_height_step * (image_height - pos_y);
#endif
		
		//printf("\t\t mb_y %d %f\n", pos_y, mb_y);
		
		const int point_depth = point_iteration(mb_x, mb_y, depth_max);
		//printf("\t\t mb_y d: %d\n", point_depth);
		if(point_depth > depth_base){
		//if(point_iteration(mb_x, mb_y, depth_max) > depth_base){
			/*printf("\t\t mb_y ok\n");
			printf("\t\t mb_y p: %d %d\n", pos_x, pos_y);
			printf("\t\t               = %p\n", (void *)&image_plain[pos_x]);
			printf("\t\t               = %p\n", (void *)&image_plain[pos_x][pos_y]);*/
			image_plain[pos_x][pos_y] = depth_percent;
		}
		
		//printf("\t\t mb_y done\n");
	}
}

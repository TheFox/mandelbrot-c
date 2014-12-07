
#include "mandelbrot_mt.hpp"

int main(int argc, char const *argv[]){
	print_copyright();
	
	if(argc <= 8){
		printf("Usage: %s P_WIDTH P_HEIGHT DEPTH_MIN DEPTH_MAX MB_WIDTH_MID MB_WIDTH_ZOOM MB_HEIGHT_MID MB_HEIGHT_MAX\n", *argv);
		return 1;
	}
	
	const int image_width = atof(argv[1]);
	const int image_height = atof(argv[2]);
	const int depth_min = atof(argv[3]);
	const int depth_max = atof(argv[4]);
	const mbnum mb_width_mid = atof(argv[5]);
	const mbnum mb_width_zoom = atof(argv[6]) / 2.0;
	const mbnum mb_height_mid = atof(argv[7]);
	const mbnum mb_height_zoom = atof(argv[8]) / 2.0;
	
#include "init_default_vars.h"
	
	print_config(image_width, image_width_mid, image_height, image_height_mid, color_diff, depth_min, depth_max, depth_diff, depth_step, mb_width_mid, mb_width_zoom, mb_width_min, mb_width_max, mb_width_step, mb_height_mid, mb_height_zoom, mb_height_min, mb_height_max, mb_height_step);
	
	printf("image plain: %.2f MB (%lu)\n", (float)image_plain_s_total / (float)1024 / (float)1024, image_plain_s_total);
	
#include "init_image_plain.h"
#include "init_mb_xy_grid.h"
	
	
	puts("start");
	
	float depth_percent = 0.0;
	int depth_base = depth_min;
	for(int depth_i = 0; depth_i <= depth_diff; depth_i++){
		depth_base++;
		
#ifdef DEBUG
		printf("\rdepth_i: %d/%d %f", depth_i, depth_diff, depth_percent);
		fflush(stdout);
#endif
		
		for(pos_x = 0; pos_x < image_width; pos_x++){
#ifdef USE_MB_XY_GRID
			const mbnum mb_x = mb_x_grid_r[pos_x];
#else
			const mbnum mb_x = mb_width_min + mb_width_step * pos_x;
#endif
			
			//printf("\t mb_x: %d %f\n", pos_x, mb_x);
			
			#pragma omp parallel for
			for(pos_y = 0; pos_y < image_height; pos_y++){
#ifdef USE_MB_XY_GRID
				const mbnum mb_y = mb_y_grid_r[pos_y];
#else
				const mbnum mb_y = mb_height_min + mb_height_step * (image_height - pos_y);
#endif
				
				//printf("\t\t mb_y %d %f\n", pos_y, mb_y);
				
				const int point_depth = point_iteration(mb_x, mb_y, depth_max);
				if(point_depth > depth_base)
					image_plain[pos_x][pos_y] = depth_percent;
			}
		}
		
		depth_percent += depth_step;
	}
	puts("");
	
	/*
	for(pos_x = 0; pos_x < image_width; pos_x++)
		for(pos_y = 0; pos_y < image_height; pos_y++){
			red   = (int)(image_plain[pos_x][pos_y] * 255.0);
			green = (int)(image_plain[pos_x][pos_y] * 255.0);
			blue  = (int)(image_plain[pos_x][pos_y] * 255.0);
			
			imlib_context_set_color(red, green, blue, 255);
			imlib_image_draw_line(pos_x, pos_y, pos_x, pos_y, 0);
		}
	
	char *file_name = (char *)malloc(128);
	memset(file_name, 0, 128);
	sprintf(file_name, "pics/mbs_r%dx%d_d%d-%d_x%.2f-%.2f_y%.2f-%.2f.png",
		image_width, image_height,
		depth_min, depth_max,
		mb_width_mid, mb_width_zoom,
		mb_height_mid, mb_height_zoom);
	*/
	
	
	puts("end");
	return EXIT_SUCCESS;
}

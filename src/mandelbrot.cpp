
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
	
	
#include "print_copyright.h"
	
	if(argc <= 8){
		printf("Usage: %s I_WIDTH I_HEIGHT DEPTH_MIN DEPTH_MAX MB_WIDTH_MID MB_WIDTH_ZOOM MB_HEIGHT_MID MB_HEIGHT_MAX\n", *argv);
		return 1;
	}
	
	const int image_width = atof(argv[1]);
	const int image_height = atof(argv[2]);
	const int depth_min = atof(argv[3]);
	const int depth_max = atof(argv[4]);
	const mbnum_t mb_width_mid = atof(argv[5]);
	const mbnum_t mb_width_zoom_org = atof(argv[6]);
	const mbnum_t mb_height_mid = atof(argv[7]);
	const mbnum_t mb_height_zoom_org = atof(argv[8]);
	
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
	
#include "print_config.h"
	
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
#include "calc_depth_step_gpu.h"
	
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
		
#ifdef USE_OPENCL
//#include "calc_depth_step_gpu.h"
#include "calc_depth_step_cpu.h"
#else
#include "calc_depth_step_cpu.h"
#endif
		
		depth_base++;
		depth_percent += depth_step;
	}
	puts("");
	
#include "draw_image.h"
	
	//sleep(10);
	
	puts("end");
	return EXIT_SUCCESS;
}


#include "functions.hpp"

void print_copyright(){
	printf("%s %d.%d.%d (%s %s)\n", PROJECT_NAME,
		PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH,
		__DATE__, __TIME__);
	printf("%s\n", PROJECT_COPYRIGHT);
	printf("\n");
}

void print_config(int image_width, int image_width_mid, int image_height, int image_height_mid, int color_diff, int depth_min, int depth_max, int depth_diff, float depth_step, mbnum mb_width_mid, mbnum mb_width_zoom_org, mbnum mb_width_min, mbnum mb_width_max, mbnum mb_width_step, mbnum mb_height_mid, mbnum mb_height_zoom_org, mbnum mb_height_min, mbnum mb_height_max, mbnum mb_height_step){
	printf("PID: %d\n", getpid());
	
	printf("DEBUG: ");
#ifdef DEBUG
	printf("yes");
#else
	printf("no");
#endif
	printf("\n");
	
	printf("USE_OPENMP: ");
#ifdef USE_OPENMP
	printf("yes");
#else
	printf("no");
#endif
	printf("\n");
	
	printf("OPENMP_NUM_THREADS: ");
#ifdef OPENMP_NUM_THREADS
	printf("%d", OPENMP_NUM_THREADS);
#else
	printf("N/A");
#endif
	printf("\n");
	
	printf("USE_MB_XY_GRID: ");
#ifdef USE_MB_XY_GRID
	printf("yes");
#else
	printf("no");
#endif
	printf("\n");
	
	printf("USE_LOG: ");
#ifdef USE_LOG
	printf("yes");
#else
	printf("no");
#endif
	printf("\n");
	
	printf("GRID: ");
#ifdef GRID
	printf("yes");
#else
	printf("no");
#endif
	printf("\n");
	printf("GRID_STEP_PIXEL: %d\n", GRID_STEP_PIXEL);
	printf("GRID_STEP_SIZE: %d\n", GRID_STEP_SIZE);
	
	printf("TEXT: ");
#ifdef TEXT
	printf("yes");
#else
	printf("no");
#endif
	printf("\n");
	printf("TEXT_OFFSET_X: %d\n", TEXT_OFFSET_X);
	printf("TEXT_OFFSET_Y: %d\n", TEXT_OFFSET_Y);
	
	printf("image_width:  %d (%d)\n", image_width, image_width_mid);
	printf("image_height: %d (%d)\n", image_height, image_height_mid);
	printf("color_diff: %d\n", color_diff);
	printf("\n");
	
	printf("depth_min:  %d\n", depth_min);
	printf("depth_max:  %d\n", depth_max);
	printf("depth_diff: %d\n", depth_diff);
	printf("depth_step: %f\n", depth_step);
	printf("\n");
	
	printf("mb_width mid:   %.32" MBNUM_FORMAT "\n", mb_width_mid);
	printf("mb_width zoom:  %.32" MBNUM_FORMAT "\n", mb_width_zoom_org);
	printf("mb_width min:   %.32" MBNUM_FORMAT "\n", mb_width_min);
	printf("mb_width max:   %.32" MBNUM_FORMAT "\n", mb_width_max);
	printf("mb_width step:  %.32" MBNUM_FORMAT "\n", mb_width_step);
	printf("\n");
	
	printf("mb_height mid:  %.32" MBNUM_FORMAT "\n", mb_height_mid);
	printf("mb_height zoom: %.32" MBNUM_FORMAT "\n", mb_height_zoom_org);
	printf("mb_height min:  %.32" MBNUM_FORMAT "\n", mb_height_min);
	printf("mb_height max:  %.32" MBNUM_FORMAT "\n", mb_height_max);
	printf("mb_height step: %.32" MBNUM_FORMAT "\n", mb_height_step);
	printf("\n");
}

#ifdef USE_LOG
int point_iteration(const mbnum cx, const mbnum cy, const int depth_max){
	int step = 0;
	mbnum x = 0;
	mbnum y = 0;
	
	mbnum x_sqr = x * x;
	mbnum y_sqr = y * y;
	
	for(step = 0; step < depth_max && x_sqr + y_sqr < 4.0; step++){
		mbnum log_x;
		mbnum log_y;
		mbnum abs_x;
		mbnum abs_y;
		char sign = 0;
		
		if(x > 0){
			abs_x = fabs(x);
		}
		else{
			abs_x = x;
			sign = 1;
		}
		
		if(y > 0){
			abs_y = fabs(y);
		}
		else{
			abs_y = y;
			sign = sign == 1 ? 0 : 1;
		}
		
		if(sign == 1)
			y = -exp(log(abs_x) + log(abs_y));
		else
			y = exp(log(abs_x) + log(abs_y));
		
		y += y;
		y += cy;
		
		x = x_sqr - y_sqr + cx;
		
		if(x > 0){
			abs_x = fabs(x);
		}
		else{
			abs_x = x;
			sign = 1;
		}
		
		if(y > 0){
			abs_y = fabs(y);
		}
		else{
			abs_y = y;
			sign = sign == 1 ? 0 : 1;
		}
		
		log_x = log(abs_x);
		log_y = log(abs_y);
		
		if(sign == 1){
			x_sqr = -exp(log_x + log_x);
			y_sqr = -exp(log_y + log_y);
		}
		else{
			x_sqr = exp(log_x + log_x);
			y_sqr = exp(log_y + log_y);
		}
		
	}
	
	return step;
}
#else
int point_iteration(const mbnum cx, const mbnum cy, const int depth_max){
	//puts("point_iteration");
	
	int step = 0;
	mbnum x = 0;
	mbnum y = 0;
	
	mbnum x_sqr = x * x;
	mbnum y_sqr = y * y;
	
	for(step = 0; step < depth_max && x_sqr + y_sqr < 4.0; step++){
		y = x * y;
		y += y;
		y += cy;
		
		x = x_sqr - y_sqr + cx;
		
		x_sqr = x * x;
		y_sqr = y * y;
	}
	
	//puts("point_iteration end");
	
	return step;
}
#endif

void data_file_name(char *file_name, int image_width, int image_height, int depth_min, int depth_max, mbnum mb_width_mid, mbnum mb_width_zoom_org, mbnum mb_height_mid, mbnum mb_height_zoom_org, int depth_i){
	sprintf(file_name, "data/mbs_r%dx%d_d%d-%d_x%.2" MBNUM_FORMAT "-%.2" MBNUM_FORMAT "_y%.2" MBNUM_FORMAT "-%.2" MBNUM_FORMAT "_%08d.txt",
		image_width, image_height,
		depth_min, depth_max,
		mb_width_mid, mb_width_zoom_org,
		mb_height_mid, mb_height_zoom_org,
		depth_i
	);
}

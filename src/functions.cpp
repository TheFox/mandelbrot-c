
#include "functions.hpp"

#ifdef USE_LOG
int point_iteration(const mbnum_t cx, const mbnum_t cy, const int depth_max){
	int step = 0;
	mbnum_t x = 0;
	mbnum_t y = 0;
	
	mbnum_t x_sqr = x * x;
	mbnum_t y_sqr = y * y;
	
	for(step = 0; step < depth_max && x_sqr + y_sqr < 4.0; step++){
		mbnum_t log_x;
		mbnum_t log_y;
		mbnum_t abs_x;
		mbnum_t abs_y;
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
int point_iteration(const mbnum_t cx, const mbnum_t cy, const int depth_max){
	//puts("point_iteration");
	
	int step = 0;
	mbnum_t x = 0;
	mbnum_t y = 0;
	
	mbnum_t x_sqr = x * x;
	mbnum_t y_sqr = y * y;
	
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

void data_file_name(char *file_name, int image_width, int image_height, int depth_min, int depth_max, mbnum_t mb_width_mid, mbnum_t mb_width_zoom_org, mbnum_t mb_height_mid, mbnum_t mb_height_zoom_org, int depth_i){
	sprintf(file_name, "data/mbs_r%dx%d_d%d-%d_x%.2" MBNUM_FORMAT "-%.2" MBNUM_FORMAT "_y%.2" MBNUM_FORMAT "-%.2" MBNUM_FORMAT "_%08d.txt",
		image_width, image_height,
		depth_min, depth_max,
		mb_width_mid, mb_width_zoom_org,
		mb_height_mid, mb_height_zoom_org,
		depth_i
	);
}

//void pfn_notify(const char *errinfo, const void *private_info, size_t cb, void *user_data){
void pfn_notify(cl_program ocl_program, void *user_data){
	fprintf(stderr, "OpenCL Error (via pfn_notify): %p\n", user_data);
}

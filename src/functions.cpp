
#include "functions.hpp"

float sqr(const float x){
	return(x * x);
}

int point_iteration(const mbnum cx, const mbnum cy, const int depth_max){
	int step = 0;
	mbnum x = 0;
	mbnum y = 0;
	
	//printf("\n");
	
	mbnum x_sqr = x * x;
	mbnum y_sqr = y * y;
	
#ifdef USE_LOG
	mbnum log_x;
	mbnum log_y;
#endif
	//mbnum x_sqr2;
	//mbnum y_sqr2;
	for(step = 0; step < depth_max && x_sqr + y_sqr < 4.0; step++){
		y = x * y;
		y += y;
		y += cy;
		
		x = x_sqr - y_sqr + cx;
		
		//if(x > 0 && y > 0)
		//x_sqr = x * x;
		//y_sqr = y * y;
		
#ifdef USE_LOG
		/*if(x < 0){
			x_sqr = x * x;
		}
		else{
			log_x = log(x);
			x_sqr = exp(log_x + log_x);
		}
		
		if(y < 0){
			y_sqr = y * y;
		}
		else{
			log_y = log(y);
			y_sqr = exp(log_y + log_y);
		}*/
		
		if(x < 0)
			x = abs(x);
		log_x = log(x);
		x_sqr = exp(log_x + log_x);
		
		if(y < 0)
			y = abs(y);
		log_y = log(y);
		y_sqr = exp(log_y + log_y);
		
#else
		x_sqr = x * x;
		y_sqr = y * y;
#endif
		
		
		//x_sqr = exp(log_x + log_x);
		//y_sqr = exp(log_y + log_y);
		
		/*
		printf("point: %f %f - %f %f\n", cx, cy, x, y);
		printf("\t A %f %f\n", x_sqr, y_sqr);
		printf("\t B %f %f %d\n\n", x_sqr2, y_sqr2, x_sqr == x_sqr2 && y_sqr == y_sqr2 ? 1 : 0);
		*/
		
	}
	
	return step;
}

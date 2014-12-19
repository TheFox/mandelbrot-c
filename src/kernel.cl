
//#include <stdio.h>

//kernel void point_iteration(global float *input, global float *output, const unsigned int count,
kernel void point_iteration(global float *input, global char *output, const unsigned int count,
	const int depth_max, const int depth_base, const float cx){
//kernel void square(global long double *input, global long double *output, const unsigned int count){
//kernel void square(global float32 *input, global float32 *output, const unsigned int count){
	int id = get_global_id(0);
	
	//printf("square id = %d/%d '%d' %d\n", id, count, depth_max, depth_base);
	/*
	printf("square id = %d/%d   '%f'\n", id, count, input[id]);
	if(id < count){
		//printf("\t square. %d\n", 0);
		output[id] = input[id] * input[id];
	}*/
	
	if(id < count){
		
		int step = 0;
		float x = 0;
		float y = 0;
		
		float x_sqr = x * x;
		float y_sqr = y * y;
		
		for(step = 0; step < depth_max && x_sqr + y_sqr < 4.0; step++){
			y = x * y;
			y += y;
			y += input[id];
			
			x = x_sqr - y_sqr + cx;
			
			x_sqr = x * x;
			y_sqr = y * y;
		}
		
		//char s = 0;
		if(step > depth_base){
			//output[id] = 1.0;
			output[id] = 1;
			//output[id] = (float)step;
			//s = 1;
		}
		else{
			//output[id] = 0.0;
			output[id] = 0;
		}
		
		//printf("square id = %d/%d m=%d b=%d    %d   %d\n", id, count, depth_max, depth_base, step, s);
	}
}

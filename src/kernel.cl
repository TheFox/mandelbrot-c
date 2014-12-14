
//#include <stdio.h>

kernel void square(global float *input, global float *output, const unsigned int count){
//kernel void square(global long double *input, global long double *output, const unsigned int count){
	int id = get_global_id(0);
	
	printf("square id = %d, count = %d\n", id, count);
	
	if(id < count){
		//puts("\t compute");
		printf("\t square. %d\n", 0);
		output[id] = input[id] * input[id];
	}
}


kernel void square(global float *input, global float *output, const unsigned int count){
	int id = get_global_id(0);
	//printf("square id = %d, count = %d\n", id, count);
	if(id < count)
		output[id] = input[id] * input[id];
}

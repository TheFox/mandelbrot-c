
// https://randomascii.wordpress.com/2011/08/13/faster-fractals-through-algebra/

#include "mandelbrot.h"

int main(int argc, char const *argv[]){
	if(argc <= 8){
		printf("Usage: %s P_WIDTH P_HEIGHT depth_min depth_max MB_WIDTH_MIN MB_WIDTH_MAX MB_HEIGHT_MIN MB_HEIGHT_MAX\n", *argv);
		return 1;
	}
	
	GHashTable *table = g_hash_table_new(g_str_hash, g_str_equal);
	g_hash_table_insert(table, "SOME_KEY", "SOME_VALUE");
	gchar *value = (gchar *)g_hash_table_lookup(table, "SOME_KEY");
	
	printf("'%s'\n", value);
	
	return 0;
	
	//const int image_width = 0;
	//const int image_height = 0;
	//const int depth_min = 0;
	//const int depth_max = 0;
	//const float mb_width_min = -2.0;
	//const float mb_width_max = 0.5;
	//const float mb_height_min = -1.25;
	//const float mb_height_max = 1.25;
	
	const int image_width = atof(argv[1]);
	const int image_height = atof(argv[2]);
	const int depth_min = atof(argv[3]);
	const int depth_max = atof(argv[4]);
	const float mb_width_min = atof(argv[5]);
	const float mb_width_max = atof(argv[6]);
	const float mb_height_min = atof(argv[7]);
	const float mb_height_max = atof(argv[8]);
	
	const float mb_width_step = (mb_width_max - mb_width_min) / image_width;
	const float mb_height_step = (mb_height_max - mb_width_min) / image_height;
	
	const int depth_diff = depth_max - depth_min;
	
	printf("image_width: %d\n", image_width);
	printf("image_height: %d\n", image_height);
	printf("depth_min: %d\n", depth_min);
	printf("depth_max: %d\n", depth_max);
	printf("depth_diff: %d\n", depth_diff);
	printf("mb_width_min: %f\n", mb_width_min);
	printf("mb_width_max: %f\n", mb_width_max);
	printf("mb_height_min: %f\n", mb_height_min);
	printf("mb_height_max: %f\n", mb_height_max);
	
	printf("mb width:  %.2f-%.2f s=%.2f\n", mb_width_min, mb_width_max, mb_width_step);
	printf("mb height: %.2f-%.2f s=%.2f\n", mb_height_min, mb_height_max, mb_height_step);
	
	
	
	puts("start");
	
	Imlib_Image image;
	const int color_diff = COLOR_MAX - COLOR_MIN;
	
	printf("color_diff: %d\n", color_diff);
	
	puts("create image...");
	image = imlib_create_image(image_width, image_height);
	if(image){
		puts("image ok");
		imlib_context_set_image(image);
		
		//imlib_context_set_mask_alpha_threshold();
		
		imlib_image_set_has_alpha(1);
		char alpha = imlib_image_has_alpha();
		printf("alpha: %s\n", alpha ? "OK" : "N/A");
		
		//imlib_context_set_color(255, 255, 0, 255);
		//imlib_image_draw_line(0, 0, image_width, image_height, 0);
		
		//for(int depth_i = depth_min; depth_i <= depth_max; depth_i++){
		for(int depth_i = 0; depth_i <= depth_diff; depth_i++){
			const int depth_base = depth_min + depth_i;
			//const float depth_percent = (float)((float)depth_i / (float)depth_max);
			const float depth_percent = (float)((float)depth_i / (float)depth_diff);
			const float depth_color = depth_percent * color_diff;
			const float blue = COLOR_MIN + depth_color;
			//imlib_context_set_color(0, 0, 255, blue);
			imlib_context_set_color(0, 0, blue, 255);
			
			printf("depth_i: %.2f %3f (%d/%d %d %6.2f)\n", depth_percent, blue, depth_i, depth_max, color_diff, depth_color);
			
			for(int pos_x = 0; pos_x < image_width; pos_x++){
				const float val_x = mb_width_min + mb_width_step * pos_x;
				
				for(int pos_y = 0; pos_y < image_height; pos_y++){
					const float val_y = mb_height_min + mb_height_step * pos_y;
					
					//printf("\txy: %d %d (%.2f %.2f)\n", pos_x, pos_y, val_x, val_y);
					
					const int point_depth = point_iteration(val_x, val_y, depth_max);
					if(point_depth > depth_base)
						//printf("\t%d %d = %d\n", pos_x, pos_y, point_depth);
						imlib_image_draw_line(pos_x, pos_y, pos_x, pos_y, 255);
					
				}
			}

#ifdef SAVE_DEPTH_STEP
			imlib_save_image("pic.png");
#endif			
			
		}
		
		char file_name[100];
		
		sprintf(file_name, "pics/pic_r%dx%d_d%d-%d_x%.2f-%.2f_y%.2f-%.2f.png", image_width, image_height, depth_min, depth_max, mb_width_min, mb_width_max, mb_height_min, mb_height_max);
		
		puts("save image 1");
		imlib_save_image("pic.png");
		
		puts("save image 2");
		imlib_save_image(file_name);
	}
	
	puts("end");
	return EXIT_SUCCESS;
}

float sqr(const float x){
	return(x * x);
}

int point_iteration(const float cx, const float cy, const int depth_max){
	int step = 0;
	float x = 0;
	float y = 0;
	
	/*
	float iter_val = 0;
	float tx = 0;
	float ty = 0;
	while(iter_val <= 4.0 && step < depth_max){
		tx = x * x - y * y + cx;
		ty = 2 * x * y + cy;
		x = tx;
		y = ty;
		iter_val = x * x + y * y;
		step++;
		
		//printf("\t %d %f %f\n", step, x, y);
	}
	*/
	
	float x_sqr = x * x;
	float y_sqr = y * y;
	while(x_sqr + y_sqr < 4.0 && step < depth_max){
		y = x * y;
		y += y;
		y += cy;
		
		
		//y = sqr(x + y) - x_sqr - y_sqr;
		
		x = x_sqr - y_sqr + cx;
		
		/*
		x_sqr = x * x;
		y_sqr = y * y;
		
		x_sqr = pow(x, 2);
		y_sqr = pow(y, 2);
		*/
		
		x_sqr = sqr(x);
		y_sqr = sqr(y);
		
		step++;
		
		//printf("\t %d %f %f  \n", step, x, y);
	}
	
	
	return step;
}


#include "mandelbrot.h"

int main(int argc, char const *argv[]){
	puts("start");
	
	Imlib_Image image;
	const int color_diff = COLOR_MAX - COLOR_MIN;
	
	printf("color_diff: %d\n", color_diff);
	
	puts("create image...");
	image = imlib_create_image(PICTURE_WIDTH, PICTURE_HEIGHT);
	if(image){
		puts("image ok");
		imlib_context_set_image(image);
		
		//imlib_context_set_mask_alpha_threshold();
		
		imlib_image_set_has_alpha(1);
		char alpha = imlib_image_has_alpha();
		printf("alpha: %s\n", alpha ? "OK" : "N/A");
		
		imlib_context_set_color(255, 0, 0, 255);
		//imlib_image_draw_line(0, 0, PICTURE_WIDTH, PICTURE_HEIGHT, 0);
		
		const float width_org = -2.0;
		const float width_max = 0.5;
		const float width_step = (width_max - width_org) / PICTURE_WIDTH;
		
		const float height_org = -1.25;
		const float height_max = 1.25;
		const float height_step = (width_max - width_org) / PICTURE_HEIGHT;
		
		printf("width:  o=%.2f s=%.2f m=%.2f\n", width_org, width_step, width_max);
		printf("height: o=%.2f s=%.2f m=%.2f\n", height_org, height_step, height_max);
		
		for(int depth_i = DEPTH_MIN; depth_i <= DEPTH_MAX; depth_i++){
			const float depth_percent = (float)((float)depth_i / (float)DEPTH_MAX);
			const float depth_color = depth_percent * color_diff;
			const float blue = COLOR_MIN + depth_color;
			imlib_context_set_color(0, 0, blue, 255);
			
			printf("depth_i: %.2f %3f (%d/%d %d %6.2f)\n", depth_percent, blue, depth_i, DEPTH_MAX, color_diff, depth_color);
			
			for(int pos_x = 0; pos_x < PICTURE_WIDTH; pos_x++){
				const float val_x = width_org + width_step * pos_x;
				
				for(int pos_y = 0; pos_y < PICTURE_HEIGHT; pos_y++){
					const float val_y = height_org + height_step * pos_y;
					
					//printf("\txy: %d %d (%.2f %.2f)\n", pos_x, pos_y, val_x, val_y);
					
					const int point_depth = point_iteration(val_x, val_y);
					if(point_depth > depth_i)
						//printf("\t%d %d = %d\n", pos_x, pos_y, point_depth);
						imlib_image_draw_line(pos_x, pos_y, pos_x, pos_y, 255);
					
				}
			}
			
			//if(depth_i % 20 == 0)
			//imlib_save_image("pic.png");
		}
		
		puts("save image");
		imlib_save_image("pic.png");
	}
	
	puts("end");
	return EXIT_SUCCESS;
}

int point_iteration(const float cx, const float cy){
	//puts("\t\tpoint_iteration");
	//printf("\t\tpoint_iteration: %f %f\n", cx, cy);
	
	float iter_val = 0;
	float x = 0, y = 0;
	int step = 0;
	
	while(iter_val <= 4.0 && step < DEPTH_MAX){
		float tx = x * x - y * y + cx;
		float ty = 2 * x * y + cy;
		x = tx;
		y = ty;
		iter_val = x * x + y * y;
		step++;
		
		//printf("\t\t\t %d %f %f\n", step, x, y);
	}
	
	//printf("\t\tpoint_iteration step: %d\n", step);
	
	return step;
}

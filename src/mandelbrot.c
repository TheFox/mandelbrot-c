
#include "mandelbrot.h"
	
int main(int argc, char const *argv[]){
	puts("start");
	
	Imlib_Image image;
	
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
		imlib_image_draw_line(0, 0, PICTURE_WIDTH, PICTURE_HEIGHT, 0);
		
		float width_org = -2.0;
		float width_max = 0.5;
		float width_step = (width_max - width_org) / PICTURE_WIDTH;
		
		float height_org = -1.25;
		float height_max = 1.25;
		float height_step = (width_max - width_org) / PICTURE_HEIGHT;
		
		printf("width:  %.2f %.2f/%.2f\n", width_org, width_step, width_max);
		printf("height: %.2f %.2f/%.2f\n", height_org, height_step, height_max);
		
		for(int depth = 0; depth < DEPTH; depth++){
			printf("depth: %d\n", depth);
			
			for(int pos_x = 0; pos_x < PICTURE_WIDTH; pos_x++){
				float val_x = width_org + pos_x
				
				for(int pos_y = 0; pos_y < PICTURE_HEIGHT; pos_y++){
					printf("\txy: %d %d\r", pos_x, pos_y);
					
					
				}
			}
			
		}
		
		
		puts("save");
		imlib_save_image("pic.png");
	}
	
	puts("end");
	return EXIT_SUCCESS;
}


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
	
	
	print_copyright();
	
	if(argc <= 8){
		printf("Usage: %s P_WIDTH P_HEIGHT DEPTH_MIN DEPTH_MAX MB_WIDTH_MID MB_WIDTH_ZOOM MB_HEIGHT_MID MB_HEIGHT_MAX\n", *argv);
		return 1;
	}
	
	const int image_width = atof(argv[1]);
	const int image_height = atof(argv[2]);
	const int depth_min = atof(argv[3]);
	const int depth_max = atof(argv[4]);
	const mbnum mb_width_mid = atof(argv[5]);
	const mbnum mb_width_zoom = atof(argv[6]) / 2.0;
	const mbnum mb_height_mid = atof(argv[7]);
	const mbnum mb_height_zoom = atof(argv[8]) / 2.0;
	
#include "init_default_vars.h"
	
	char *start_time_text = (char *)malloc(128);
	memset(start_time_text, 0, 128);
	time_t start_time = time(NULL);
	struct tm *start_time_tm = localtime(&start_time);
	strftime(start_time_text, 128, "S time: %F %T %z %Z", start_time_tm);
	
	print_config(image_width, image_width_mid, image_height, image_height_mid, color_diff, depth_min, depth_max, depth_diff, depth_step, mb_width_mid, mb_width_zoom, mb_width_min, mb_width_max, mb_width_step, mb_height_mid, mb_height_zoom, mb_height_min, mb_height_max, mb_height_step);
	
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
	
	printf("image plain: %.2f MB (%lu)\n", (float)image_plain_s_total / (float)1024 / (float)1024, image_plain_s_total);
	
#include "init_image_plain.h"
#include "init_mb_xy_grid.h"
	
#ifdef USE_OPENMP
	puts("OpenMP is active");
	omp_set_num_threads(OPENMP_NUM_THREADS);
	omp_set_dynamic(0);
#endif
	
	puts("start");
	
	float depth_percent = 0.0;
	int depth_base = depth_min;
	for(int depth_i = 0; depth_i <= depth_diff; depth_i++){
		depth_base++;
		
#ifdef DEBUG
		printf("\rdepth_i: %d/%d %f", depth_i, depth_diff, depth_percent);
		fflush(stdout);
#endif
		
		for(pos_x = 0; pos_x < image_width; pos_x++){
#ifdef USE_MB_XY_GRID
			const mbnum mb_x = mb_x_grid_r[pos_x];
#else
			const mbnum mb_x = mb_width_min + mb_width_step * pos_x;
#endif
			
			//printf("\t mb_x: %d %f\n", pos_x, mb_x);
			
			#pragma omp parallel for
			for(pos_y = 0; pos_y < image_height; pos_y++){
#ifdef USE_MB_XY_GRID
				const mbnum mb_y = mb_y_grid_r[pos_y];
#else
				const mbnum mb_y = mb_height_min + mb_height_step * (image_height - pos_y);
#endif
				
				//printf("\t\t mb_y %d %f\n", pos_y, mb_y);
				
				const int point_depth = point_iteration(mb_x, mb_y, depth_max);
				if(point_depth > depth_base)
					image_plain[pos_x][pos_y] = depth_percent;
			}
		}
		
		depth_percent += depth_step;
	}
	puts("");
	
	Imlib_Image image;
	image = imlib_create_image(image_width, image_height);
	if(image){
		puts("create image");
		
		imlib_context_set_image(image);
		imlib_image_set_has_alpha(1);
		
		char red = 0;
		char green = 0;
		char blue = 0;
		
		for(pos_x = 0; pos_x < image_width; pos_x++)
			for(pos_y = 0; pos_y < image_height; pos_y++){
				red   = (int)(image_plain[pos_x][pos_y] * 255.0);
				green = (int)(image_plain[pos_x][pos_y] * 255.0);
				blue  = (int)(image_plain[pos_x][pos_y] * 255.0);
				
				imlib_context_set_color(red, green, blue, 255);
				imlib_image_draw_line(pos_x, pos_y, pos_x, pos_y, 0);
			}
		
		
#ifdef GRID
		imlib_context_set_color(255, 0, 0, 255);
		if(image_width_mb_0 > 0 && image_width_mb_0 < image_width)
			imlib_image_draw_line(image_width_mb_0, 0, image_width_mb_0, image_height, 0);
		if(image_height_mb_0 > 0 && image_height_mb_0 < image_height)
			imlib_image_draw_line(0, image_height_mb_0, image_width, image_height_mb_0, 0);
		
		imlib_context_set_color(0, 255, 0, 255);
		imlib_image_draw_line(image_width_mid, 0, image_width_mid, image_height, 0);
		imlib_image_draw_line(0, image_height_mid, image_width, image_height_mid, 0);
		
		#pragma omp parallel for
		for(pos_x = image_width_mid - GRID_STEP_PIXEL; pos_x > 0; pos_x -= GRID_STEP_PIXEL)
			imlib_image_draw_line(pos_x, image_height_mid - GRID_STEP_SIZE, pos_x, image_height_mid + GRID_STEP_SIZE, 0);
		
		#pragma omp parallel for
		for(pos_x = image_width_mid + GRID_STEP_PIXEL; pos_x < image_width; pos_x += GRID_STEP_PIXEL)
			imlib_image_draw_line(pos_x, image_height_mid - GRID_STEP_SIZE, pos_x, image_height_mid + GRID_STEP_SIZE, 0);
		
		#pragma omp parallel for
		for(pos_y = image_height_mid - GRID_STEP_PIXEL; pos_y > 0; pos_y -= GRID_STEP_PIXEL)
			imlib_image_draw_line(image_width_mid - GRID_STEP_SIZE, pos_y, image_width_mid + GRID_STEP_SIZE, pos_y, 0);
		
		#pragma omp parallel for
		for(pos_y = image_height_mid + GRID_STEP_PIXEL; pos_y < image_height; pos_y += GRID_STEP_PIXEL)
			imlib_image_draw_line(image_width_mid - GRID_STEP_SIZE, pos_y, image_width_mid + GRID_STEP_SIZE, pos_y, 0);
#endif
		
#ifdef TEXT
		imlib_add_path_to_font_path("/usr/share/fonts");
		imlib_add_path_to_font_path("/usr/local/share/fonts");
		imlib_add_path_to_font_path("/Library/Fonts");
		
		Imlib_Font font = imlib_load_font("Arial/10");
		//Imlib_Font font = imlib_load_font("notepad/30");
		if(font){
			imlib_context_set_color(255, 0, 0, 255);
			
			printf("font ok\n");
			imlib_context_set_font(font);
			
			
			time_t picture_time = time(NULL);
			struct tm *picture_time_tm = localtime(&picture_time);
			
			
			char *text = (char *)malloc(128);
			memset(text, 0, 128);
			
			strcpy(text, "TEST TEXT");
			
			int text_w, text_h;
			imlib_get_text_size(text, &text_w, &text_h);
			printf("font: '%d' '%d'\n", text_w, text_h);
			//imlib_text_draw(320 - (text_w / 2) - up_x, 240 - (text_h / 2) - up_y, text);
			//imlib_text_draw(TEXT_OFFSET_X, TEXT_OFFSET_Y, text);
			
			int text_offset_y = TEXT_OFFSET_Y;
			strcpy(text, PROJECT_COPYRIGHT);
			imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
			
			
			text_offset_y += text_h;
			imlib_text_draw(TEXT_OFFSET_X, text_offset_y, start_time_text);
			
			text_offset_y += text_h;
			strftime(text, 128, "P time: %F %T %z %Z", picture_time_tm);
			imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
			
			text_offset_y += text_h;
			sprintf(text, "img wh: %d %d px", image_width, image_height);
			imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
			
#ifdef GRID
			text_offset_y += text_h;
			sprintf(text, "grid step xy: %f %f", mb_width_step * grid_step_pixel_f, mb_height_step * grid_step_pixel_f);
			imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
#endif
			
			
			text_offset_y += text_h;
			sprintf(text, "cr(x): %f %f", mb_width_min, mb_width_max);
			imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
			
			text_offset_y += text_h;
			sprintf(text, "cr mid: %f", mb_width_mid);
			imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
			
			text_offset_y += text_h;
			sprintf(text, "cr zoom: %f", mb_width_zoom);
			imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
			
			text_offset_y += text_h;
			sprintf(text, "cr step: %f", mb_width_step);
			imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
			
			
			text_offset_y += text_h;
			sprintf(text, "ci(y): %f %f", mb_height_min, mb_height_max);
			imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
			
			text_offset_y += text_h;
			sprintf(text, "ci mid: %f", mb_height_mid);
			imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
			
			text_offset_y += text_h;
			sprintf(text, "ci zoom: %f", mb_height_zoom);
			imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
			
			text_offset_y += text_h;
			sprintf(text, "ci step: %f", mb_height_step);
			imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
			
			
			text_offset_y += text_h;
			sprintf(text, "depth: %d %d", depth_min, depth_max);
			imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
			
			text_offset_y += text_h;
			sprintf(text, "depth step: %f", depth_step);
			imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
			
			imlib_free_font();
		}
		else
			printf("font failed\n");
#endif
		
		int image_size = imlib_get_cache_size();
		printf("size: %d\n", image_size);
		
		char *file_name = (char *)malloc(128);
		memset(file_name, 0, 128);
		sprintf(file_name, "pics/mbs_r%dx%d_d%d-%d_x%.2f-%.2f_y%.2f-%.2f.png",
			image_width, image_height,
			depth_min, depth_max,
			mb_width_mid, mb_width_zoom,
			mb_height_mid, mb_height_zoom);
		
		
		puts("save image: pic.png");
		imlib_save_image("pic.png");
		
		printf("save image: %s\n", file_name);
		imlib_save_image(file_name);
	}
	
	//sleep(10);
	
	puts("end");
	return EXIT_SUCCESS;
}

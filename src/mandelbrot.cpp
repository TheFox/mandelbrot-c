
#include "mandelbrot.h"

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
	
	
	printf("%s %d.%d.%d (%s %s)\n", PROJECT_NAME,
		PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH,
		__DATE__, __TIME__);
	printf("%s\n", PROJECT_COPYRIGHT);
	printf("\n");
	
	if(argc <= 8){
		printf("Usage: %s P_WIDTH P_HEIGHT DEPTH_MIN DEPTH_MAX MB_WIDTH_MID MB_WIDTH_ZOOM MB_HEIGHT_MID MB_HEIGHT_MAX\n", *argv);
		return 1;
	}
	
	const int image_width = atof(argv[1]);
	const int image_height = atof(argv[2]);
	const int depth_min = atof(argv[3]);
	const int depth_max = atof(argv[4]);
	const float mb_width_mid = atof(argv[5]);
	const float mb_width_zoom = atof(argv[6]) / 2.0;
	//const float mb_width_min = atof(argv[5]);
	//const float mb_width_max = atof(argv[6]);
	const float mb_height_mid = atof(argv[7]);
	const float mb_height_zoom = atof(argv[8]) / 2.0;
	//const float mb_height_min = atof(argv[7]);
	//const float mb_height_max = atof(argv[8]);
	
	const int image_width_mid = image_width / 2;
	const int image_height_mid = image_height / 2;
	const int color_diff = COLOR_MAX - COLOR_MIN;
	
	const float mb_width_min = mb_width_mid - mb_width_zoom;
	const float mb_width_max = mb_width_mid + mb_width_zoom;
	const float mb_width_step = (mb_width_max - mb_width_min) / image_width;
	
	const float mb_height_min = mb_height_mid - mb_height_zoom;
	const float mb_height_max = mb_height_mid + mb_height_zoom;
	const float mb_height_step = (mb_height_max - mb_height_min) / image_height;
	
	const int depth_diff = depth_max - depth_min;
	const float depth_step = 1.0 / (float)depth_diff;
	
#ifdef TEXT
#ifdef GRID
	const float grid_step_pixel_f = (float)GRID_STEP_PIXEL;
#endif
#endif
	
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
	
	printf("USE_MB_XY_GRID: ");
#ifdef USE_MB_XY_GRID
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
	
	printf("image_width: %d (%d)\n", image_width, image_width_mid);
	printf("image_height: %d (%d)\n", image_height, image_height_mid);
	printf("color_diff: %d\n", color_diff);
	printf("\n");
	
	printf("depth_min:  %d\n", depth_min);
	printf("depth_max:  %d\n", depth_max);
	printf("depth_diff: %d\n", depth_diff);
	printf("depth_step: %f\n", depth_step);
	printf("\n");
	
	printf("mb_width mid:   %.32f\n", mb_width_mid);
	printf("mb_width zoom:  %.32f\n", mb_width_zoom);
	printf("mb_width min:   %.32f\n", mb_width_min);
	printf("mb_width max:   %.32f\n", mb_width_max);
	printf("mb_width step:  %.32f\n", mb_width_step);
	printf("\n");
	
	printf("mb_height mid:  %.32f\n", mb_height_mid);
	printf("mb_height zoom: %.32f\n", mb_height_zoom);
	printf("mb_height min:  %.32f\n", mb_height_min);
	printf("mb_height max:  %.32f\n", mb_height_max);
	printf("mb_height step: %.32f\n", mb_height_step);
	printf("\n");
	
	//return 0;
	
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
	
	//return 0;
	
	
	const size_t voidp_s = sizeof(voidp);
	const size_t voidpp_s = sizeof(voidpp);
	const size_t ucharp_s = sizeof(char *);
	//const size_t ucharp_s = sizeof(ucharp);
	const size_t uchar_s = sizeof(char);
	const size_t float_s = sizeof(float);
	const size_t image_plain_s        = image_width * ucharp_s;
	//const size_t image_plain_width_s  = image_height * ucharp_s;
	const size_t image_plain_width_s  = image_height * float_s;
	//const size_t image_plain_height_s = 4 * uchar_s;
	//const size_t image_plain_item_s   = 4            * ucharp_s;
	const size_t image_plain_s_total = 
		image_width * ucharp_s
		+ image_width * image_height * ucharp_s
		+ image_width * image_height * 4;
	// 8192 * 8 + 8192 * 8192 * 8 + 8192 * 8192 * 4
	
	printf("size\n");
	printf("\t voidp_s:  %lu\n", voidp_s);
	printf("\t voidpp_s: %lu\n", voidpp_s);
	printf("\t ucharp_s: %lu\n", ucharp_s);
	printf("\t uchar_s:  %lu\n", uchar_s);
	printf("\t uchar ***: %lu\n", sizeof(unsigned char ***));
	
	printf("image plain: %.2f MB (%lu)\n", (float)image_plain_s_total / (float)1024 / (float)1024, image_plain_s_total);
	printf("\t image_plain_s:        %lu\n", image_plain_s);
	printf("\t image_plain_width_s:  %lu\n", image_plain_width_s);
	//printf("\t image_plain_height_s: %lu\n", image_plain_height_s);
	//printf("\t item_s:   %lu\n", image_plain_item_s);
	
	/*
	void **image_plain = (void **)malloc(image_plain_s);
	memset(image_plain, 0, image_plain_s);
	
	void *image_plain_ip0 = (void *)malloc(image_plain_width_s);
	image_plain[0] = image_plain_ip0;
	memset(image_plain_ip0, 0, image_plain_width_s);
	
	void *image_plain_ip00 = (void *)malloc(image_plain_height_s);
	image_plain_ip0[0] = image_plain_ip00;
	memset(image_plain_ip00, 0, image_plain_height_s);
	*/
	
	size_t size = 0;
	
	printf("alloc image_plain\n");
	//char ***image_plain = (char ***)malloc(image_plain_s);
	//char **image_plain = (char **)malloc(image_plain_s);
	float **image_plain = (float **)malloc(image_plain_s);
	memset(image_plain, 0, image_plain_s);
	size += image_plain_s;
	int x;
	int y;
	#pragma omp parallel for
	for(x = 0; x < image_width; x++){
		//printf("\t alloc image_plain[%d]\n", x);
		//image_plain[x] = (char *)malloc(image_plain_width_s);
		image_plain[x] = (float *)malloc(image_plain_width_s);
		memset(image_plain[x], 0, image_plain_width_s);
		size += image_plain_width_s;
		
		/*
		for(y = 0; y < image_height; y++){
			//printf("\t\t alloc image_plain[%d][%d]\n", x, y);
			
			image_plain[x][y] = (char *)malloc(image_plain_height_s);
			//memset(image_plain[x][y], 0, image_plain_height_s);
			//size += image_plain_height_s;
			image_plain[x][y][0] = 0;
			size++;
		}*/
	}
	printf("alloc image_plain done\n");
	printf("size: %lu\n", size);
	
	/*
	//strcpy(image_plain[0], "ABC");
	//strcpy(image_plain[0][1], "DEF");
	
	printf("ip[0]    %p\n", image_plain[0]);
	printf("ip[0][0] %p '%s'\n", image_plain[0][0], image_plain[0][0]);
	printf("ip[0][1] %p '%s'\n", image_plain[0][1], image_plain[0][1]);
	
	printf("ip[1]    %p\n", image_plain[1]);
	printf("ip[1][0] %p '%s'\n", image_plain[1][0], image_plain[1][0]);
	printf("ip[1][1] %p '%s'\n", image_plain[1][1], image_plain[1][1]);
	
	return 0;
	*/
	
#ifdef USE_OPENMP
	puts("OpenMP is active");
	omp_set_num_threads(8);
#endif
	
	
	
	/*
	imlib_context_set_color(255, 255, 0, 255);
	imlib_image_draw_line(0, 0, 1, 1, 0);
	return 0;*/
	
	int pos_x = 0;
	int pos_y = 0;
	
#ifdef USE_MB_XY_GRID
	size_t mb_x_grid_s = image_width * float_s;
	size_t mb_y_grid_s = image_height * float_s;
	float *mb_x_grid_w = (float *)malloc(mb_x_grid_s);
	float *mb_y_grid_w = (float *)malloc(mb_y_grid_s);
	printf("mb_x_grid_s: %lu\n", mb_x_grid_s);
	printf("mb_y_grid_s: %lu\n", mb_y_grid_s);
	
	#pragma omp parallel for
	for(pos_x = 0; pos_x < image_width; pos_x++)
		mb_x_grid_w[pos_x] = mb_width_min + mb_width_step * pos_x;
	
	#pragma omp parallel for
	for(pos_y = 0; pos_y < image_height; pos_y++)
		mb_y_grid_w[pos_y] = mb_height_min + mb_height_step * (image_height - pos_y);
	
	const float *mb_x_grid_r = (const float *)mb_x_grid_w;
	const float *mb_y_grid_r = (const float *)mb_y_grid_w;
#endif
	
	//for(pos_x = 0; pos_x < image_width; pos_x++) printf("x: %d %f\n", pos_x, mb_x_grid_r[pos_x]);
	//for(pos_y = 0; pos_y < image_width; pos_y++) printf("y: %d %f\n", pos_y, mb_y_grid_r[pos_y]);
	
	//return 0;
	
	puts("start");
	
	float depth_percent = 0.0;
	int depth_base = depth_min;
	//for(int depth_i = depth_min; depth_i <= depth_max; depth_i++){
	for(int depth_i = 0; depth_i <= depth_diff; depth_i++){
		depth_base++;
		//const int depth_base = depth_min + depth_i;
		//const float depth_percent = (float)((float)depth_i / (float)depth_max);
		//const float depth_percent = (float)((float)depth_i / (float)depth_diff);
		//const char depth_percent = (char)((float)depth_i / (float)depth_diff * 100.0);
		//const float depth_percent = (float)((float)depth_i / (float)depth_diff);
		//const float depth_color = depth_percent * color_diff;
		//const float blue = COLOR_MIN + depth_color;
		//imlib_context_set_color(0, 0, 255, blue);
		//imlib_context_set_color(0, 0, blue, 255);
		
#ifdef DEBUG
		//printf("depth_i: %.2f %3f (%d/%d %d %6.2f)\n", depth_percent, blue, depth_i, depth_max, color_diff, depth_color);
		printf("\rdepth_i: %d/%d %f ", depth_i, depth_diff, depth_percent);
		fflush(stdout);
#endif
		
		for(pos_x = 0; pos_x < image_width; pos_x++){
#ifdef USE_MB_XY_GRID
			const float mb_x = mb_x_grid_r[pos_x];
#else
			const float mb_x = mb_width_min + mb_width_step * pos_x;
#endif
			
			//printf("\t mb_x: %d %f\n", pos_x, mb_x);
			
			#pragma omp parallel for
			for(pos_y = 0; pos_y < image_height; pos_y++){
#ifdef USE_MB_XY_GRID
				const float mb_y = mb_y_grid_r[pos_y];
#else
				const float mb_y = mb_height_min + mb_height_step * (image_height - pos_y);
#endif
				
				//printf("\t\t mb_y %d %f\n", pos_y, mb_y);
				
				const int point_depth = point_iteration(mb_x, mb_y, depth_max);
				if(point_depth > depth_base)
					//image_plain[pos_x][pos_y][0] = blue;
					//image_plain[pos_x][pos_y][0] = depth_percent;
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
		
		//int color_nr = 0;
		char red = 0;
		char green = 0;
		char blue = 0;
		
		for(x = 0; x < image_width; x++){
			//printf("\t x = %d\n", x);
			
			//#pragma omp parallel for
			for(y = 0; y < image_height; y++){
				//printf("\t\t y = %d %f\n", y, image_plain[x][y]);
				
				red   = (int)(image_plain[x][y] * 255.0);
				green = (int)(image_plain[x][y] * 255.0);
				blue  = (int)(image_plain[x][y] * 255.0);
				
				//imlib_context_set_color(image_plain[x][y][0], image_plain[x][y][1], image_plain[x][y][2], image_plain[x][y][3]);
				imlib_context_set_color(red, green, blue, 255);
				imlib_image_draw_line(x, y, x, y, 0);
			}
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
		for(x = image_width_mid - GRID_STEP_PIXEL; x > 0; x -= GRID_STEP_PIXEL)
			imlib_image_draw_line(x, image_height_mid - GRID_STEP_SIZE, x, image_height_mid + GRID_STEP_SIZE, 0);
		
		#pragma omp parallel for
		for(x = image_width_mid + GRID_STEP_PIXEL; x < image_width; x += GRID_STEP_PIXEL)
			imlib_image_draw_line(x, image_height_mid - GRID_STEP_SIZE, x, image_height_mid + GRID_STEP_SIZE, 0);
		
		#pragma omp parallel for
		for(y = image_height_mid - GRID_STEP_PIXEL; y > 0; y -= GRID_STEP_PIXEL)
			imlib_image_draw_line(image_width_mid - GRID_STEP_SIZE, y, image_width_mid + GRID_STEP_SIZE, y, 0);
		
		#pragma omp parallel for
		for(y = image_height_mid + GRID_STEP_PIXEL; y < image_height; y += GRID_STEP_PIXEL)
			imlib_image_draw_line(image_width_mid - GRID_STEP_SIZE, y, image_width_mid + GRID_STEP_SIZE, y, 0);
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
			
			time_t current_time;
			current_time = time(NULL);
			//struct tm *current_time_tm = gmtime(&current_time);
			struct tm *current_time_tm = localtime(&current_time);
			
			
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
			strftime(text, 128, "%F %T %z %Z", current_time_tm);
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
			mb_width_min, mb_width_max,
			mb_height_min, mb_height_max);
		
		
		puts("save image: pic.png");
		imlib_save_image("pic.png");
		
		printf("save image: %s\n", file_name);
		imlib_save_image(file_name);
	}
	
	//sleep(10);
	
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
	
	float x_sqr = x * x;
	float y_sqr = y * y;
	for(step = 0; step < depth_max && x_sqr + y_sqr < 4.0; step++){
		y = x * y;
		y += y;
		y += cy;
		
		x = x_sqr - y_sqr + cx;
		
		x_sqr = sqr(x);
		y_sqr = sqr(y);
	}
	
	return step;
}

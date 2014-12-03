
// https://randomascii.wordpress.com/2011/08/13/faster-fractals-through-algebra/

#include "mandelbrot.h"

int main(int argc, char const *argv[]){
	/*
	GHashTable *table = g_hash_table_new(g_str_hash, g_str_equal);
	g_hash_table_insert(table, "SOME_KEY", "SOME_VALUE");
	gchar *value = (gchar *)g_hash_table_lookup(table, "SOME_KEY");
	
	float val_x = 3.14;
	printf("%f\n", val_x >> 1);
	
	return 0;
	
	
	float val_x = 3.10000000;
	
	printf("%f\n", val_x * val_x * val_x * val_x * val_x * val_x);
	printf("%f\n", (val_x * val_x * val_x) * (val_x * val_x * val_x));
	printf("%f\n", (val_x * val_x * val_x) * (val_x * val_x) * val_x);
	
	return 0;
	
	
	#pragma omp parallel
	
	omp_set_num_threads(4);
	
	const int id = omp_get_thread_num();
	printf("Hello World from thread %d\n", id);
	
	return 0;
	
	
	printf("My process ID : %d\n", getpid());
	
	
	
	float y = 0.1;
	
	printf("%e\n", y);
	printf("%.255f\n", y);
	printf("float %lu\n", sizeof(float));
	printf("double %lu\n", sizeof(double));
	
	return x;
	
	omp_set_num_threads(32);
	
	int i = 0;
	#pragma omp parallel for
	for(i = 0; i < 32; i++){
		const int id = omp_get_thread_num();
		printf("A i=%d ID=%d\n", i, id);
	}
	
	
	unsigned char x = 65;
	unsigned char *x_p = &x;
	printf("x %lu\n", sizeof(x));
	printf("x %d\n", x);
	printf("x_p %lu\n", sizeof(x_p));
	printf("x_p %p\n", x_p);
	printf("x_p %s\n", x_p);
	
	printf("alloc\n");
	int arr1[10][3];
	printf("alloc ok\n");
	
	printf("char %lu\n", sizeof(char));
	printf("int %lu\n", sizeof(int));
	printf("int* %lu\n", sizeof(int*));
	printf("array %lu\n", sizeof(arr1));
	printf("items %lu\n", sizeof(arr1) / sizeof(arr1[0]));
	printf("items[0] %lu\n", sizeof(arr1[0]));
	printf("items[0] %lu\n", sizeof(arr1[0]) / sizeof(arr1[0][0]));
	
	memset(arr1, 0, 120);
	printf("item[0] %d %p\n", arr1[0][0], arr1[0]);
	printf("item[1] %d %p\n", arr1[1][0], arr1[1]);
	printf("item[2] %d %p\n", arr1[2][0], arr1[2]);
	printf("item[3] %d %p\n", arr1[3][0], arr1[3]);
	printf("item[4] %d %p\n", arr1[4][0], arr1[4]);
	printf("item[5] %d %p\n", arr1[5][0], arr1[5]);
	printf("item[6] %d %p\n", arr1[6][0], arr1[6]);
	printf("item[7] %d %p\n", arr1[7][0], arr1[7]);
	printf("item[8] %d %p\n", arr1[8][0], arr1[8]);
	printf("item[9] %d %p\n", arr1[9][0], arr1[9]);
	
	//return 0;
	
	
	char a = 250;
	unsigned char b = (unsigned char)a;
	
	printf("'%x' '%d'\n", a, a);
	printf("'%x' '%d'\n", b, b);
	
	return 0;
	*/
	
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
	
	
	printf("%s %d.%d.%d (%s %s)\n", Mandelbrot_NAME,
		Mandelbrot_VERSION_MAJOR, Mandelbrot_VERSION_MINOR, Mandelbrot_VERSION_PATCH,
		__DATE__, __TIME__);
	printf("%s\n", Mandelbrot_COPYRIGHT);
	printf("\n");
	
	if(argc <= 8){
		printf("Usage: %s P_WIDTH P_HEIGHT DEPTH_MIN DEPTH_MAX MB_WIDTH_MID MB_WIDTH_ZOOM MB_HEIGHT_MID MB_HEIGHT_MAX\n", *argv);
		return 1;
	}
	
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
	
	
	printf("PID: %d\n", getpid());
	
	printf("DEBUG: ");
#ifdef DEBUG
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
	
	printf("OMP: ");
#ifdef _OMP_H
	printf("yes");
#else
	printf("no");
#endif
	printf("\n");
	
	printf("image_width: %d (%d)\n", image_width, image_width_mid);
	printf("image_height: %d (%d)\n", image_height, image_height_mid);
	printf("color_diff: %d\n", color_diff);
	printf("\n");
	
	printf("depth_min:  %d\n", depth_min);
	printf("depth_max:  %d\n", depth_max);
	printf("depth_diff: %d\n", depth_diff);
	printf("depth_step: %f\n", depth_step);
	printf("\n");
	
	printf("mb_width mid:   %f\n", mb_width_mid);
	printf("mb_width zoom:  %f\n", mb_width_zoom);
	printf("mb_width min:   %f\n", mb_width_min);
	printf("mb_width max:   %f\n", mb_width_max);
	printf("mb_width step:  %f\n", mb_width_step);
	printf("\n");
	
	printf("mb_height mid:  %f\n", mb_height_mid);
	printf("mb_height zoom: %f\n", mb_height_zoom);
	printf("mb_height min:  %f\n", mb_height_min);
	printf("mb_height max:  %f\n", mb_height_max);
	printf("mb_height step: %f\n", mb_height_step);
	printf("\n");
	
	float image_width_mb_0_iter = 0;
	int image_width_mb_0 = 0;
	if(mb_width_max > 0)
		for(image_width_mb_0_iter = mb_width_min; image_width_mb_0_iter <= 0; image_width_mb_0_iter += mb_width_step){
			//printf("image_width_mb_0_iter: %d %f\n", image_width_mb_0, image_width_mb_0_iter);
			image_width_mb_0++;
		}
	
	float image_height_mb_0_iter = 0;
	int image_height_mb_0 = 0;
	if(mb_height_max > 0)
		for(image_height_mb_0_iter = mb_height_min; image_height_mb_0_iter <= 0; image_height_mb_0_iter += mb_height_step){
			//printf("image_height_mb_0_iter: %d %f\n", image_height_mb_0, image_height_mb_0_iter);
			image_height_mb_0++;
		}
	//return 0;
	
	printf("mb_0 x: %d %f\n", image_width_mb_0, image_width_mb_0_iter);
	printf("mb_0 y: %d %f\n", image_height_mb_0, image_height_mb_0_iter);
	
	
	
	
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
	
	for(pos_x = 0; pos_x < image_width; pos_x++)
		mb_x_grid_w[pos_x] = mb_width_min + mb_width_step * pos_x;
	
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
		printf("\rdepth_i: %d/%d %f   ", depth_i, depth_diff, depth_percent);
		fflush(stdout);
#endif
		
		for(pos_x = 0; pos_x < image_width; pos_x++){
#ifdef USE_MB_XY_GRID
			const float mb_x = mb_x_grid_r[pos_x];
#else
			const float mb_x = mb_width_min + mb_width_step * pos_x;
#endif
			
			//printf("\t mb_x: %d %f\n", pos_x, mb_x);
			
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
	
	Imlib_Image image;
	image = imlib_create_image(image_width, image_height);
	if(image){
		puts("create image...");
		
		imlib_context_set_image(image);
		imlib_image_set_has_alpha(1);
		
		//int color_nr = 0;
		char red = 0;
		char green = 0;
		char blue = 0;
		
		for(x = 0; x < image_width; x++){
			//printf("\t x = %d\n", x);
			
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
		
		imlib_context_set_color(255, 0, 0, 255);
		imlib_image_draw_line(image_width_mb_0, 0, image_width_mb_0, image_height, 0);
		imlib_image_draw_line(0, image_height_mb_0, image_width, image_height_mb_0, 0);
		//imlib_image_draw_line(0, imag, image_height, imag, 0);
		
		imlib_context_set_color(0, 255, 0, 255);
		imlib_image_draw_line(image_width_mid, 0, image_width_mid, image_height, 0);
		imlib_image_draw_line(0, image_height_mid, image_width, image_height_mid, 0);
		
		
		
		
		int image_size = imlib_get_cache_size();
		printf("size: %d\n", image_size);
		
		char file_name[100];
		sprintf(file_name, "pics/pic_r%dx%d_d%d-%d_x%.2f-%.2f_y%.2f-%.2f.png",
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

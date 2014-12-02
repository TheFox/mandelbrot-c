
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
	*/
	
	
	if(argc <= 8){
		printf("Usage: %s P_WIDTH P_HEIGHT depth_min depth_max MB_WIDTH_MIN MB_WIDTH_MAX MB_HEIGHT_MIN MB_HEIGHT_MAX\n", *argv);
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
	const float mb_width_min = atof(argv[5]);
	const float mb_width_max = atof(argv[6]);
	const float mb_height_min = atof(argv[7]);
	const float mb_height_max = atof(argv[8]);
	
	const float mb_width_step = (mb_width_max - mb_width_min) / image_width;
	const float mb_height_step = (mb_height_max - mb_width_min) / image_height;
	
	const int depth_diff = depth_max - depth_min;
	
	printf("PID: %d\n", getpid());
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
	
	printf("OMP: ");
#ifdef _OMP_H
	printf("yes");
#else
	printf("no");
#endif
	printf("\n");
	
	printf("SAVE_DEPTH_STEP: ");
#ifdef SAVE_DEPTH_STEP
	printf("yes");
#else
	printf("no");
#endif
	printf("\n");
	
	
	const size_t voidp_s = sizeof(voidp);
	const size_t voidpp_s = sizeof(voidpp);
	//const size_t ucharp_s = sizeof(unsigned char *);
	const size_t ucharp_s = sizeof(ucharp);
	const size_t uchar_s = sizeof(unsigned char);
	const size_t image_plain_s        = image_width * voidp_s;
	const size_t image_plain_width_s  = image_height * voidp_s;
	const size_t image_plain_height_s = 4 * uchar_s;
	//const size_t image_plain_item_s   = 4            * ucharp_s;
	const size_t image_plain_s_total = 
		image_width * voidp_s
		+ image_width * image_height * voidp_s
		+ image_width * image_height * 4;
	// 8192 * 8 + 8192 * 8192 * 8 + 8192 * 8192 * 4
	
	printf("size\n");
	printf("\t voidp_s:  %lu\n", voidp_s);
	printf("\t voidpp_s: %lu\n", voidpp_s);
	printf("\t ucharp_s: %lu\n", ucharp_s);
	printf("\t uchar **: %lu\n", sizeof(unsigned char **));
	
	printf("alloc image pixs: %.2f MB\n", (float)image_plain_s_total / (float)1024 / (float)1024);
	printf("\t image_plain_s:        %lu\n", image_plain_s);
	printf("\t image_plain_width_s:  %lu\n", image_plain_width_s);
	printf("\t image_plain_height_s: %lu\n", image_plain_height_s);
	//printf("\t item_s:   %lu\n", image_plain_item_s);
	
	
	voidpp image_plain = (voidpp)malloc(image_plain_width_s);
	memset(image_plain, 0, image_plain_width_s);
	
	image_plain[0] = (ucharp)malloc(image_plain_height_s);
	memset(image_plain[0], 0, image_plain_height_s);
	
	
	//strcpy(image_plain[0], "ABC");
	
	char str1[100];
	memset(str1, 0, 100);
	strcpy(str1, "hello");
	printf("'%s'\n", str1);
	
	/*
	int x = 0;
	int y = 0;
	for(x = 0; x < image_width; x++){
		(*image_plain)[x] = (ucharp)malloc(image_plain_height_s);
		memset((*image_plain)[x], 0, image_plain_height_s);
		
		//for(y = 0; y < image_height; y++){
		//	(*image_plain)[x][y] = (ucharp)malloc(image_plain_item_s);
		//	memset((*image_plain)[x][y], 0, image_plain_item_s);
		//}
		
	}
	*/
	
	printf("\t image pixs:     %p\n", image_plain);
	printf("\t image pixs: 0 = %p\n", image_plain[0]);
	printf("\t image pixs: 00 = %p\n", (*image_plain[0])[0]);
	printf("\t image pixs: 1 = %p\n", image_plain[1]);
	printf("\t image pixs: 2 = %p\n", image_plain[2]);
	printf("\t image pixs: 3 = %p\n", image_plain[3]);
	
	sleep(10);
	
	return 0;
	
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
		
		imlib_context_set_color(255, 255, 0, 255);
		imlib_image_draw_line(0, 0, 1, 1, 0);
		
		return 0;
		
		//for(int depth_i = depth_min; depth_i <= depth_max; depth_i++){
		for(int depth_i = 0; depth_i <= depth_diff; depth_i++){
			const int depth_base = depth_min + depth_i;
			//const float depth_percent = (float)((float)depth_i / (float)depth_max);
			const float depth_percent = (float)((float)depth_i / (float)depth_diff);
			const float depth_color = depth_percent * color_diff;
			const float blue = COLOR_MIN + depth_color;
			//imlib_context_set_color(0, 0, 255, blue);
			imlib_context_set_color(0, 0, blue, 255);
			
#ifdef DEBUG
			printf("depth_i: %.2f %3f (%d/%d %d %6.2f)\n", depth_percent, blue, depth_i, depth_max, color_diff, depth_color);
#endif
			
			for(int pos_x = 0; pos_x < image_width; pos_x++){
				const float val_x = mb_width_min + mb_width_step * pos_x;
				
				for(int pos_y = 0; pos_y < image_height; pos_y++){
					const float val_y = mb_height_min + mb_height_step * pos_y;
					
					//printf("\txy: %d %d (%.2f %.2f)\n", pos_x, pos_y, val_x, val_y);
					
					const int point_depth = point_iteration(val_x, val_y, depth_max);
					if(point_depth > depth_base)
						//printf("\t%d %d = %d\n", pos_x, pos_y, point_depth);
						//imlib_image_draw_line(pos_x, pos_y, pos_x, pos_y, 255);
						;
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

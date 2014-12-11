
#include "mandelbrot_test.hpp"

int main(int argc, char **argv){
	
	print_copyright();
	
#include "print_config.h"
	
	printf("getpid: %p\n", getpid);
	printf("strcpy: %p\n", strcpy);
	printf("sqrt: %p\n", sqrt);
	printf("time: %p\n", time);
	printf("localtime: %p\n", localtime);
	
	printf("imlib_create_image: %p\n", imlib_create_image);
	printf("imlib_context_set_image: %p\n", imlib_context_set_image);
	printf("imlib_image_set_has_alpha: %p\n", imlib_image_set_has_alpha);
	printf("imlib_context_set_color: %p\n", imlib_context_set_color);
	printf("imlib_image_draw_line: %p\n", imlib_image_draw_line);
	printf("imlib_save_image: %p\n", imlib_save_image);
	printf("imlib_add_path_to_font_path: %p\n", imlib_add_path_to_font_path);
	printf("imlib_load_font: %p\n", imlib_load_font);
	printf("imlib_context_set_font: %p\n", imlib_context_set_font);
	printf("imlib_text_draw: %p\n", imlib_text_draw);
	
#ifdef USE_OPENMP
	printf("omp_set_num_threads: %p\n", omp_set_num_threads);
	printf("omp_get_num_threads: %p\n", omp_get_num_threads);
	printf("omp_get_thread_num: %p\n", omp_get_thread_num);
	printf("omp_set_dynamic: %p\n", omp_set_dynamic);
	
	
	int iter_x;
	omp_set_num_threads(OPENMP_NUM_THREADS);
	omp_set_dynamic(0);
	
	#pragma omp parallel for
	for(iter_x = 0; iter_x < 8; iter_x++){
		const int id_x = omp_get_thread_num();
		if(id_x)
			printf("OMP i=%d ID=%d\n", iter_x, id_x);
	}
#else
	puts("OpenMP header not found");
#endif
	
	printf("print_copyright: %p\n", print_copyright);
	printf("print_config: %p\n", print_config);
	printf("point_iteration: %p\n", point_iteration);
	printf("data_file_name: %p\n", data_file_name);
	
	return EXIT_SUCCESS;
}

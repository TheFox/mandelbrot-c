
#include "../src/mandelbrot.h"

int main(int argc, char **argv){
	
	printf("%s %d.%d.%d (%s %s)\n", Mandelbrot_NAME,
		Mandelbrot_VERSION_MAJOR, Mandelbrot_VERSION_MINOR, Mandelbrot_VERSION_PATCH,
		__DATE__, __TIME__);
	printf("%s\n", Mandelbrot_COPYRIGHT);
	printf("\n");
	
	printf("sqrt: %p\n", sqrt);
	
	printf("imlib_create_image: %p\n", imlib_create_image);
	printf("imlib_context_set_image: %p\n", imlib_context_set_image);
	printf("imlib_image_set_has_alpha: %p\n", imlib_image_set_has_alpha);
	printf("imlib_context_set_color: %p\n", imlib_context_set_color);
	printf("imlib_image_draw_line: %p\n", imlib_image_draw_line);
	printf("imlib_save_image: %p\n", imlib_save_image);
	
	printf("g_hash_table_new: %p\n", g_hash_table_new);
	
	printf("omp_set_num_threads: %p\n", omp_set_num_threads);
	printf("omp_get_num_threads: %p\n", omp_get_num_threads);
	printf("omp_get_thread_num: %p\n", omp_get_thread_num);
	
	
	return EXIT_SUCCESS;
}

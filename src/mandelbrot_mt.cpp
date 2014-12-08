
/*
	Run multiple processes to generate data files.
	Compile all data files with mandelbrot_ci.
*/

#include "mandelbrot_mt.hpp"

using namespace std;

int main(int argc, char const *argv[]){
	print_copyright();
	
	if(argc <= 10){
		printf("Usage: %s P_WIDTH P_HEIGHT DEPTH_MIN DEPTH_MAX MB_WIDTH_MID MB_WIDTH_ZOOM MB_HEIGHT_MID MB_HEIGHT_MAX THREAD_ID THREAD_MAX\n", *argv);
		return 1;
	}
	
	const int image_width = atof(argv[1]);
	const int image_height = atof(argv[2]);
	const int depth_min = atof(argv[3]);
	const int depth_max = atof(argv[4]);
	const mbnum mb_width_mid = atof(argv[5]);
	const mbnum mb_width_zoom_org = atof(argv[6]);
	const mbnum mb_height_mid = atof(argv[7]);
	const mbnum mb_height_zoom_org = atof(argv[8]);
	const int thread_id = atof(argv[9]);
	const int thread_max = atof(argv[10]);
	
#include "init_default_vars.h"
	printf("thread: %d/%d\n", thread_id, thread_max);
	
	print_config(image_width, image_width_mid, image_height, image_height_mid, color_diff, depth_min, depth_max, depth_diff, depth_step, mb_width_mid, mb_width_zoom_org, mb_width_min, mb_width_max, mb_width_step, mb_height_mid, mb_height_zoom_org, mb_height_min, mb_height_max, mb_height_step);
	
//#include "init_image_plain.h"
#include "init_mb_xy_grid.h"
	
	const size_t file_name_s = 128;
	char *file_name = (char *)malloc(file_name_s);
	memset(file_name, 0, file_name_s);
	
	puts("start");
	
	float depth_percent = 0.0;
	int depth_base = depth_min;
	for(int depth_i = thread_id - 1; depth_i <= depth_diff; depth_i += thread_max){
		depth_base++;
		
#ifdef DEBUG
		printf("depth_i: %d/%d %f\n", depth_i, depth_diff, depth_percent);
		fflush(stdout);
#endif
		
		data_file_name(file_name, image_width, image_height, depth_min, depth_max, mb_width_mid, mb_width_zoom_org, mb_height_mid, mb_height_zoom_org, depth_i);
		
		ofstream data_file;
		data_file.open(file_name);
		if(data_file.is_open()){
			for(pos_x = 0; pos_x < image_width; pos_x++){
#ifdef USE_MB_XY_GRID
				const mbnum mb_x = mb_x_grid_r[pos_x];
#else
				const mbnum mb_x = mb_width_min + mb_width_step * pos_x;
#endif
				
				//printf("\t mb_x: %d %f\n", pos_x, mb_x);

#ifdef USE_OPENMP
	#pragma omp parallel for
#endif
				for(pos_y = 0; pos_y < image_height; pos_y++){
#ifdef USE_MB_XY_GRID
					const mbnum mb_y = mb_y_grid_r[pos_y];
#else
					const mbnum mb_y = mb_height_min + mb_height_step * (image_height - pos_y);
#endif
					
					//printf("\t\t mb_y %d %f\n", pos_y, mb_y);
					
					
					const int point_depth = point_iteration(mb_x, mb_y, depth_max);
					if(point_depth > depth_base)
						//image_plain[pos_x][pos_y] = depth_percent;
						//data_file << depth_percent;
						data_file << 'x';
					
					data_file << ',';
				}
				
				data_file.seekp(data_file.tellp() - (streamoff)1);
				data_file << endl;
			}
		}
		else{
			puts("file open failed");
			return EXIT_FAILURE;
		}
		
		depth_percent += depth_step;
	}
	puts("");
	
	puts("end");
	return EXIT_SUCCESS;
}


/*
	Read the data files and draw the image.
*/

#include "mandelbrot_ci.hpp"

using namespace std;

int main(int argc, char const *argv[]){
	
	print_copyright();
	
	if(argc <= 8){
		printf("Usage: %s P_WIDTH P_HEIGHT DEPTH_MIN DEPTH_MAX MB_WIDTH_MID MB_WIDTH_ZOOM MB_HEIGHT_MID MB_HEIGHT_MAX\n", *argv);
		return 1;
	}
	
	const int image_width = atof(argv[1]);
	const int image_height = atof(argv[2]);
	const int depth_min = atof(argv[3]);
	const int depth_max = atof(argv[4]);
	const mbnum_t mb_width_mid = atof(argv[5]);
	const mbnum_t mb_width_zoom_org = atof(argv[6]);
	const mbnum_t mb_height_mid = atof(argv[7]);
	const mbnum_t mb_height_zoom_org = atof(argv[8]);
	
#include "init_default_vars.h"
	
	char *start_time_text = (char *)malloc(128);
	memset(start_time_text, 0, 128);
	time_t start_time = time(NULL);
	struct tm *start_time_tm = localtime(&start_time);
	strftime(start_time_text, 128, "S time: %F %T %z %Z", start_time_tm);
	
	print_config(image_width, image_width_mid, image_height, image_height_mid, color_diff, depth_min, depth_max, depth_diff, depth_step, mb_width_mid, mb_width_zoom_org, mb_width_min, mb_width_max, mb_width_step, mb_height_mid, mb_height_zoom_org, mb_height_min, mb_height_max, mb_height_step);
	
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
	
#include "init_image_plain.h"
	
#ifdef USE_OPENMP
	puts("OpenMP is active");
	omp_set_num_threads(OPENMP_NUM_THREADS);
	omp_set_dynamic(0);
#endif
	
	const size_t file_name_s = 128;
	char *file_name = (char *)malloc(file_name_s);
	memset(file_name, 0, file_name_s);
	
	const size_t num_str_s = 128;
	char *num_str = (char *)malloc(num_str_s);
	memset(num_str, 0, num_str_s);
	
	puts("start");
	
	float depth_percent = 0.0;
	int depth_base = depth_min;
	for(int depth_i = 0; depth_i <= depth_diff; depth_i++){
		depth_base++;
		
		data_file_name(file_name, image_width, image_height, depth_min, depth_max, mb_width_mid, mb_width_zoom_org, mb_height_mid, mb_height_zoom_org, depth_i);
		
#ifdef DEBUG
		printf("depth_i: %d/%d %f %s\n", depth_i, depth_diff, depth_percent, file_name);
		fflush(stdout);
#endif
		
		ifstream data_file;
		data_file.open(file_name);
		if(data_file.is_open()){
			string line;
			for(pos_x = 0; getline(data_file, line); pos_x++){
				char *line_c = (char *)line.c_str();
				
				pos_y = 0;
				while(true){
					//printf("%d '%s'\n", pos_x, line_c);
					
					if(line_c[0] == 'x'){
						//printf("\t - found\n");
						image_plain[pos_x][pos_y] = depth_percent;
						line_c++;
					}
					
					if(line_c[0] == 0)
						break;
					
					line_c++;
					
					pos_y++;
					//sleep(1);
				}
				//break;
			}
			data_file.close();
		}
		else{
			puts("file open failed");
			return EXIT_FAILURE;
		}
		
		//break;
		depth_percent += depth_step;
	}
	puts("");
	
	/*
	for(pos_x = 0; pos_x < 2; pos_x++){
		for(pos_y = 0; pos_y < 2; pos_y++){
			printf("%d %d = %f\n", pos_x, pos_y, image_plain[pos_x][pos_y]);
		}
	}
	*/
	
#include "draw_image.h"
	
	puts("end");
	return EXIT_SUCCESS;
}

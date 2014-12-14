
#include "print_const.h"

printf("program_dirname: %s\n", program_dirname);
printf("program_cwd:     %s\n", program_cwd);

printf("image_width:  %d (%d)\n", image_width, image_width_mid);
printf("image_height: %d (%d)\n", image_height, image_height_mid);
printf("color_diff: %d\n", color_diff);
printf("\n");

printf("depth_min:  %d\n", depth_min);
printf("depth_max:  %d\n", depth_max);
printf("depth_diff: %d\n", depth_diff);
printf("depth_step: %f\n", depth_step);
printf("\n");

printf("mb_width mid:   %.32" MBNUM_FORMAT "\n", mb_width_mid);
printf("mb_width zoom:  %.32" MBNUM_FORMAT "\n", mb_width_zoom_org);
printf("mb_width min:   %.32" MBNUM_FORMAT "\n", mb_width_min);
printf("mb_width max:   %.32" MBNUM_FORMAT "\n", mb_width_max);
printf("mb_width step:  %.32" MBNUM_FORMAT "\n", mb_width_step);
printf("\n");

printf("mb_height mid:  %.32" MBNUM_FORMAT "\n", mb_height_mid);
printf("mb_height zoom: %.32" MBNUM_FORMAT "\n", mb_height_zoom_org);
printf("mb_height min:  %.32" MBNUM_FORMAT "\n", mb_height_min);
printf("mb_height max:  %.32" MBNUM_FORMAT "\n", mb_height_max);
printf("mb_height step: %.32" MBNUM_FORMAT "\n", mb_height_step);
printf("\n");

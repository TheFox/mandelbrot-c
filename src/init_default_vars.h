int pos_x = 0;
int pos_y = 0;

const int image_width_mid = image_width / 2;
const int image_height_mid = image_height / 2;
const int color_diff = COLOR_MAX - COLOR_MIN;

const mbnum mb_width_min = mb_width_mid - mb_width_zoom;
const mbnum mb_width_max = mb_width_mid + mb_width_zoom;
const mbnum mb_width_step = (mb_width_max - mb_width_min) / image_width;

const mbnum mb_height_min = mb_height_mid - mb_height_zoom;
const mbnum mb_height_max = mb_height_mid + mb_height_zoom;
const mbnum mb_height_step = (mb_height_max - mb_height_min) / image_height;

const int depth_diff = depth_max - depth_min;
const float depth_step = 1.0 / (float)depth_diff;

const size_t mbnum_s = sizeof(mbnum);
const size_t image_plain_s        = image_width  * mbnum_s;
const size_t image_plain_width_s  = image_height * mbnum_s;
const size_t image_plain_s_total = 
	image_width * mbnum_s
	+ image_width * image_height * mbnum_s
	+ image_width * image_height * 4;

#ifdef TEXT
#ifdef GRID
	const float grid_step_pixel_f = (float)GRID_STEP_PIXEL;
#endif
#endif

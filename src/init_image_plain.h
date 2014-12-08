
const size_t image_plain_s        = image_width  * mbnum_s;
const size_t image_plain_width_s  = image_height * mbnum_s;
const size_t image_plain_s_total = 
	image_width * mbnum_s
	+ image_width * image_height * mbnum_s
	+ image_width * image_height * 4;

printf("image plain: %.2f MB (%lu)\n", (float)image_plain_s_total / (float)1024 / (float)1024, image_plain_s_total);

size_t image_plain_size = 0;
printf("alloc image_plain\n");
mbnum **image_plain = (mbnum **)malloc(image_plain_s);
memset(image_plain, 0, image_plain_s);
image_plain_size += image_plain_s;
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
for(pos_x = 0; pos_x < image_width; pos_x++){
	image_plain[pos_x] = (mbnum *)malloc(image_plain_width_s);
	memset(image_plain[pos_x], 0, image_plain_width_s);
	image_plain_size += image_plain_width_s;
}
printf("alloc image_plain done: %lu\n", image_plain_size);


const size_t image_plain_s        = image_width  * sizeof(mbnum_p);
const size_t image_plain_width_s  = image_height * sizeof(mbnum);
const size_t image_plain_s_total = 
	image_plain_s
	+ image_width * image_height * sizeof(mbnum)
	//+ image_width * image_height * 4
	;

printf("sizeof(mbnum):   %lu\n", sizeof(mbnum));
printf("sizeof(mbnum*):  %lu\n", sizeof(mbnum_p));
printf("sizeof(mbnum**): %lu\n", sizeof(mbnum_p*));

printf("image_plain_s:       %lu\n", image_plain_s);
printf("image_plain_width_s: %lu\n", image_plain_width_s);

printf("image plain: %.2f MB (%lu)\n", (float)image_plain_s_total / (float)1024 / (float)1024, image_plain_s_total);

size_t image_plain_size = 0;

printf("alloc image_plain: %lu\n", image_plain_s);
mbnum_p *image_plain = (mbnum_p *)malloc(image_plain_s);

printf("memset image_plain\n");
memset(image_plain, 0, image_plain_s);
image_plain_size += image_plain_s;

printf("create image_plain\n");
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
for(pos_x = 0; pos_x < image_width; pos_x++){
	//printf("create image_plain: malloc\n");
	image_plain[pos_x] = (mbnum_p)malloc(image_plain_width_s);
	if(!image_plain[pos_x]){
		puts("malloc failed");
		exit(1);
	}
	//printf("create image_plain: %d/%d %p\n", pos_x, image_width, image_plain[pos_x]);
	
	//printf("create image_plain: memset\n");
	//printf("\t image_plain[%d][0]: %p\n", pos_x, image_plain[pos_x][0]);
	memset(image_plain[pos_x], 0, image_plain_width_s);
	//printf("\t image_plain[%d]: %p\n", pos_x, image_plain[pos_x]);
	//printf("\t image_plain[%d][0]: %p\n", pos_x, image_plain[pos_x][0]);
	
	image_plain_size += image_plain_width_s;
	
	//sleep(1);
}
printf("alloc image_plain done: %lu\n", image_plain_size);


Imlib_Image image;
image = imlib_create_image(image_width, image_height);
if(image){
	puts("create image");
	
	imlib_context_set_image(image);
	imlib_image_set_has_alpha(1);
	
	char red = 0;
	char green = 0;
	char blue = 0;
	
	for(pos_x = 0; pos_x < image_width; pos_x++)
		for(pos_y = 0; pos_y < image_height; pos_y++){
			red   = (int)(image_plain[pos_x][pos_y] * 255.0);
			green = (int)(image_plain[pos_x][pos_y] * 255.0);
			blue  = (int)(image_plain[pos_x][pos_y] * 255.0);
			
			imlib_context_set_color(red, green, blue, 255);
			imlib_image_draw_line(pos_x, pos_y, pos_x, pos_y, 0);
		}
	
	
#ifdef GRID
	imlib_context_set_color(255, 0, 0, 255);
	if(image_width_mb_0 > 0 && image_width_mb_0 < image_width)
		imlib_image_draw_line(image_width_mb_0, 0, image_width_mb_0, image_height, 0);
	if(image_height_mb_0 > 0 && image_height_mb_0 < image_height)
		imlib_image_draw_line(0, image_height_mb_0, image_width, image_height_mb_0, 0);
	
	imlib_context_set_color(0, 255, 0, 255);
	imlib_image_draw_line(image_width_mid, 0, image_width_mid, image_height, 0);
	imlib_image_draw_line(0, image_height_mid, image_width, image_height_mid, 0);
	
#ifdef USE_OPENMP
	#pragma omp parallel for
#endif
	for(pos_x = image_width_mid - GRID_STEP_PIXEL; pos_x > 0; pos_x -= GRID_STEP_PIXEL)
		imlib_image_draw_line(pos_x, image_height_mid - GRID_STEP_SIZE, pos_x, image_height_mid + GRID_STEP_SIZE, 0);
	
#ifdef USE_OPENMP
	#pragma omp parallel for
#endif
	for(pos_x = image_width_mid + GRID_STEP_PIXEL; pos_x < image_width; pos_x += GRID_STEP_PIXEL)
		imlib_image_draw_line(pos_x, image_height_mid - GRID_STEP_SIZE, pos_x, image_height_mid + GRID_STEP_SIZE, 0);

#ifdef USE_OPENMP
	#pragma omp parallel for
#endif
	for(pos_y = image_height_mid - GRID_STEP_PIXEL; pos_y > 0; pos_y -= GRID_STEP_PIXEL)
		imlib_image_draw_line(image_width_mid - GRID_STEP_SIZE, pos_y, image_width_mid + GRID_STEP_SIZE, pos_y, 0);

#ifdef USE_OPENMP
	#pragma omp parallel for
#endif
	for(pos_y = image_height_mid + GRID_STEP_PIXEL; pos_y < image_height; pos_y += GRID_STEP_PIXEL)
		imlib_image_draw_line(image_width_mid - GRID_STEP_SIZE, pos_y, image_width_mid + GRID_STEP_SIZE, pos_y, 0);
#endif
	
#ifdef TEXT
	imlib_add_path_to_font_path("/usr/share/fonts");
	imlib_add_path_to_font_path("/usr/local/share/fonts");
	imlib_add_path_to_font_path("/Library/Fonts");
	
	Imlib_Font font = imlib_load_font("Courier New/10");
	//Imlib_Font font = imlib_load_font("notepad/30");
	if(font){
		imlib_context_set_color(255, 0, 0, 255);
		
		printf("font ok\n");
		imlib_context_set_font(font);
		
		
		time_t picture_time = time(NULL);
		struct tm *picture_time_tm = localtime(&picture_time);
		
		
		char *text = (char *)malloc(128);
		memset(text, 0, 128);
		
		strcpy(text, "TEST TEXT");
		
		int text_w, text_h;
		imlib_get_text_size(text, &text_w, &text_h);
		printf("font: '%d' '%d'\n", text_w, text_h);
		//imlib_text_draw(320 - (text_w / 2) - up_x, 240 - (text_h / 2) - up_y, text);
		//imlib_text_draw(TEXT_OFFSET_X, TEXT_OFFSET_Y, text);
		
		int text_offset_y = TEXT_OFFSET_Y;
		strcpy(text, PROJECT_COPYRIGHT);
		imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
		
		
		text_offset_y += text_h;
		imlib_text_draw(TEXT_OFFSET_X, text_offset_y, start_time_text);
		
		text_offset_y += text_h;
		strftime(text, 128, "P time: %F %T %z %Z", picture_time_tm);
		imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
		
		text_offset_y += text_h;
		sprintf(text, "img     w h:  %d %d px", image_width, image_height);
		imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
		
#ifdef GRID
		text_offset_y += text_h;
		sprintf(text, "grid step x: % .16" MBNUM_FORMAT, mb_width_step * grid_step_pixel_f);
		imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
		
		text_offset_y += text_h;
		sprintf(text, "grid step y: % .16" MBNUM_FORMAT, mb_height_step * grid_step_pixel_f);
		imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
#endif
		
		
		text_offset_y += text_h;
		sprintf(text, "cr      min: % .16" MBNUM_FORMAT, mb_width_min);
		imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
		
		text_offset_y += text_h;
		sprintf(text, "        max: % .16" MBNUM_FORMAT, mb_width_max);
		imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
		
		text_offset_y += text_h;
		sprintf(text, "        mid: % .16" MBNUM_FORMAT, mb_width_mid);
		imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
		
		text_offset_y += text_h;
		sprintf(text, "       zoom: % .16" MBNUM_FORMAT, mb_width_zoom_org);
		imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
		
		text_offset_y += text_h;
		sprintf(text, "       step: % .16" MBNUM_FORMAT, mb_width_step);
		imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
		
		
		text_offset_y += text_h;
		sprintf(text, "ci      min: % .16" MBNUM_FORMAT, mb_height_min);
		imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
		
		text_offset_y += text_h;
		sprintf(text, "        max: % .16" MBNUM_FORMAT, mb_height_max);
		imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
		
		text_offset_y += text_h;
		sprintf(text, "        mid: % .16" MBNUM_FORMAT, mb_height_mid);
		imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
		
		text_offset_y += text_h;
		sprintf(text, "       zoom: % .16" MBNUM_FORMAT, mb_height_zoom_org);
		imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
		
		text_offset_y += text_h;
		sprintf(text, "       step: % .16" MBNUM_FORMAT, mb_height_step);
		imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
		
		
		text_offset_y += text_h;
		sprintf(text, "depth   B E:  %d %d", depth_min, depth_max);
		imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
		
		text_offset_y += text_h;
		sprintf(text, "       step:  %.16f", depth_step);
		imlib_text_draw(TEXT_OFFSET_X, text_offset_y, text);
		
		imlib_free_font();
	}
	else
		printf("font failed\n");
#endif
	
	int image_size = imlib_get_cache_size();
	printf("size: %d\n", image_size);
	
	char *file_name = (char *)malloc(128);
	memset(file_name, 0, 128);
	sprintf(file_name, "pics/mbs_r%dx%d_d%d-%d_x%.2" MBNUM_FORMAT "-%.2" MBNUM_FORMAT "_y%.2" MBNUM_FORMAT "-%.2" MBNUM_FORMAT ".png",
		image_width, image_height,
		depth_min, depth_max,
		mb_width_mid, mb_width_zoom_org,
		mb_height_mid, mb_height_zoom_org);
	
	
	puts("save image: pic.png");
	imlib_save_image("pic.png");
	
	printf("save image: %s\n", file_name);
	imlib_save_image(file_name);
}

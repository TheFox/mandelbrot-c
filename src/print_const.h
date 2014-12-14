
printf("PID: %d\n", getpid());
	
	printf("DEBUG: ");
#ifdef DEBUG
	printf("yes");
#else
	printf("no");
#endif
	printf("\n");
	
	printf("USE_OPENMP: ");
#ifdef USE_OPENMP
	printf("yes");
#else
	printf("no");
#endif
	printf("\n");
	
	printf("OPENMP_NUM_THREADS: ");
#ifdef OPENMP_NUM_THREADS
	printf("%d", OPENMP_NUM_THREADS);
#else
	printf("N/A");
#endif
	printf("\n");
	
	printf("USE_OPENCL: ");
#ifdef USE_OPENCL
	printf("yes");
#else
	printf("no");
#endif
	printf("\n");
	
	printf("USE_MB_XY_GRID: ");
#ifdef USE_MB_XY_GRID
	printf("yes");
#else
	printf("no");
#endif
	printf("\n");
	
	printf("USE_LOG: ");
#ifdef USE_LOG
	printf("yes");
#else
	printf("no");
#endif
	printf("\n");
	
	printf("GRID: ");
#ifdef GRID
	printf("yes");
#else
	printf("no");
#endif
	printf("\n");
	printf("GRID_STEP_PIXEL: %d\n", GRID_STEP_PIXEL);
	printf("GRID_STEP_SIZE: %d\n", GRID_STEP_SIZE);
	
	printf("TEXT: ");
#ifdef TEXT
	printf("yes");
#else
	printf("no");
#endif
	printf("\n");
	printf("TEXT_OFFSET_X: %d\n", TEXT_OFFSET_X);
	printf("TEXT_OFFSET_Y: %d\n", TEXT_OFFSET_Y);

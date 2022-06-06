	n2d_rectangle_t rectangle, src_rect;
	n2d_buffer_t  	destnation;
	n2d_buffer_t  	source;
	int screen_width, screen_height;
	int status;
	int r, g, b, rgba;
	
	IDirectFBSurface	   *img = NULL;
	IDirectFBImageProvider *provider;
	DFBSurfaceDescription img_dsc;
	
	screen_width = 1024;
	screen_height = 600;
	buff_size = screen_width * screen_height * 4;
	
	/* Img Surface */
	img_dsc.flags = DSDESC_CAPS | DSDESC_PIXELFORMAT | DSDESC_WIDTH | DSDESC_HEIGHT;
	img_dsc.caps = DSCAPS_VIDEOONLY;
	img_dsc.height = IMG_HEIGHT;
	img_dsc.width = IMG_WIDTH;
	img_dsc.pixelformat = DSPF_ARGB;
	dfb->CreateSurface(dfb, &img_dsc, &img);
	char *ptr = NULL;
	int pitch = IMG_WIDTH * 4;
	img->Lock( img, DSLF_READ, &ptr, &pitch );
	img->Unlock( img );
	printf("img physical addr = 0x%0x\n", ptr);
	
	/* Img Provider */
	dfb->CreateImageProvider(dfb, filename, &provider);
	provider->GetSurfaceDescription(provider, &img_dsc);
	provider->RenderTo(provider, img, NULL);
	provider->Release(provider);
	
	/* Dest Buffer */
	memset(&destnation, 0 , sizeof(n2d_buffer_t));
	destnation.format = N2D_BGRA8888;
	destnation.width = screen_width;
	destnation.height = screen_height+8; //Alignment: 16 pixels
	//destnation.alignedw = screen_width;
	//destnation.alignedh = screen_height+8;
	destnation.stride = screen_width*4;
	destnation.orientation = N2D_0;
	destnation.gpu = (unsigned int*)g_osd_buf;
	
	/* Src Buffer */
	memset(&source, 0 , sizeof(n2d_buffer_t));
	source.format = N2D_BGRA8888;
	source.width = IMG_WIDTH+4; //Alignment
	source.height = IMG_HEIGHT;
	//source.alignedw = IMG_WIDTH+4;
	//source.alignedh = IMG_HEIGHT;
	source.stride = (IMG_HEIGHT+4)*4;
	source.orientation = N2D_0;
	source.gpu = (unsigned int*)(ptr);
	
	/* Dest Rectangle */
	memset(&rectangle, 0 , sizeof(n2d_rectangle_t));
	rectangle.width = IMG_WIDTH;
	rectangle.height = IMG_HEIGHT;
	
	/* Src Rectangle */
	memset(&src_rect, 0 , sizeof(n2d_rectangle_t));
	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.width = IMG_WIDTH;
	src_rect.height = IMG_HEIGHT;
	
	srand(time(NULL));
	printf(">>>>>%s start to drawing\n", __FUNCTION__);
	/* N2D Blit */
	unsigned long long pixels = IMG_HEIGHT * IMG_WIDTH * 5000;
	long t, dt;
	//n2d_fill(&destnation, NULL, 0x000000ff, N2D_BLEND_NONE);
	t = myclock();
	for (times = 0 ; times < runs ; times++){
		rectangle.x = rand() % (screen_width - IMG_WIDTH);
		rectangle.y = rand() % (screen_height - IMG_HEIGHT);
		status = n2d_blit(&destnation, &rectangle, &source, &src_rect, N2D_BLEND_NONE);
		if(status != N2D_SUCCESS)
			printf("status = %d\n", status);
	}
	dt = myclock() - t;
	printf("[%s] pixels = %llu\n", __FUNCTION__, pixels);
	pixels = pixels / (unsigned long long)dt;
	printf("[%s][N2D_Blit] %4llu.%.3llu MPixel/s\n", __FUNCTION__, pixels / 1000, pixels % 1000);
	
	/* N2D Alpha Blit */
	//n2d_fill(&destnation, NULL, 0x000000ff, N2D_BLEND_NONE);
	pixels = IMG_HEIGHT * IMG_WIDTH * 5000;
	t = myclock();
	for (times = 0 ; times < runs ; times++){
		rectangle.x = rand() % (screen_width - IMG_WIDTH);
		rectangle.y = rand() % (screen_height - IMG_HEIGHT);
		status = n2d_blit(&destnation, &rectangle, &source, &src_rect, N2D_BLEND_SRC_OVER);
	}
	dt = myclock() - t;
	printf("[%s] pixels = %llu\n", __FUNCTION__, pixels);
	pixels = pixels / (unsigned long long)dt;
	printf("[%s][N2D_Alpha_Blit] %4llu.%.3llu MPixel/s\n", __FUNCTION__, pixels / 1000, pixels % 1000);
	
	/* N2D FillRect */
	//n2d_fill(&destnation, NULL, 0x000000ff, N2D_BLEND_NONE);
	pixels = IMG_HEIGHT * IMG_WIDTH * 5000;
	t = myclock();
	for (times = 0 ; times < runs ; times++){
		rectangle.x = rand() % (screen_width - IMG_WIDTH);
		rectangle.y = rand() % (screen_height - IMG_HEIGHT);
		r = rand() % 255;
		g = rand() % 255;
		b = rand() % 255;
		rgba = (r << 24) + (g << 16) + (b << 8) + 255;
		status = n2d_fill(&destnation, &rectangle, rgba, N2D_BLEND_NONE);
	}
	dt = myclock() - t;
	printf("[%s] pixels = %llu\n", __FUNCTION__, pixels);
	pixels = pixels / (unsigned long long)dt;
	printf("[%s][N2D_Fill] %4llu.%.3llu MPixel/s\n", __FUNCTION__, pixels / 1000, pixels % 1000);
	
	/* N2D DrawLine */
	n2d_point_t start, end;
	//n2d_fill(&destnation, NULL, 0x000000ff, N2D_BLEND_NONE);
	long long line = runs;
	t = myclock();
	for (times = 0 ; times < runs ; times++){
		start.x = rand() % (screen_width/2);
		start.y = rand() % screen_height;
		end.x = rand() % (screen_width/2) + (screen_width/2);
		end.y = rand() % screen_height;
		r = rand() % 255;
		g = rand() % 255;
		b = rand() % 255;
		rgba = (r << 24) + (g << 16) + (b << 8) + 255;
		status = n2d_line(&destnation, start, end, NULL, rgba, N2D_BLEND_NONE);
	}
	dt = myclock() - t;
	printf("[%s] lines = %llu\n", __FUNCTION__, line);
	line = line * 1000 / (unsigned long long)dt;
	printf("[%s][N2D_DrawLine] %4llu.%.3llu KLine/s\n", __FUNCTION__, line / 1000, line % 1000);
	
	/* N2D DrawRect */
	//n2d_fill(&destnation, NULL, 0x000000ff, N2D_BLEND_NONE);
	n2d_point_t starts[4],ends[4];
	long long rect = runs / 2;
	t = myclock();
	for (times = 0 ; times < (runs / 2) ; times++){
		
		/* Top */
		starts[0].x = rand() % (screen_width - IMG_WIDTH);
		starts[0].y = rand() % (screen_height - IMG_HEIGHT);
		ends[0].x = starts[0].x + IMG_WIDTH;
		ends[0].y = starts[0].y;
		
		/* Left */
		starts[1].x = starts[0].x;
		starts[1].y = starts[0].y;
		ends[1].x = starts[0].x;
		ends[1].y = starts[0].y + IMG_HEIGHT;
		
		/* Bottom */
		starts[2].x = starts[0].x;
		starts[2].y = starts[0].y + IMG_HEIGHT;
		ends[2].x = starts[0].x + IMG_WIDTH;
		ends[2].y = starts[0].y + IMG_HEIGHT;
		
		/* Right */
		starts[3].x = starts[0].x + IMG_WIDTH;
		starts[3].y = starts[0].y;
		ends[3].x = starts[0].x + IMG_WIDTH;
		ends[3].y = starts[0].y + IMG_HEIGHT;
		
		r = rand() % 255;
		g = rand() % 255;
		b = rand() % 255;
		rgba = (r << 24) + (g << 16) + (b << 8) + 255;
		int pen;
		for (pen = 0 ; pen < 4 ; pen++){
			status = n2d_line(&destnation, starts[pen], ends[pen], NULL, rgba, N2D_BLEND_NONE);
		}
	}
	dt = myclock() - t;
	printf("[%s] rects = %llu\n", __FUNCTION__, rect);
	rect = rect * 1000 / (unsigned long long)dt;
	printf("[%s][N2D_DrawRect] %4llu.%.3llu KRect/s\n", __FUNCTION__, rect / 1000, rect % 1000);
	
	img->Release(img);


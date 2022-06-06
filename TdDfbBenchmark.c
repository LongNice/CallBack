#include <TdDfbBenchmark.h>

static void signal_handler(int signo)
{
    signal(signo, signal_handler);
    switch (signo)
    {
    	case SIGTERM:
        	printf("Process recieve SIGTERM  car_info exit\n");    

        /*程序自己退出，或shell里调用kill缺省该进程。该信号可以被阻塞，或被处理
        可以在这里做一些程序退出前的最后处理工作*/
        exit(0);
        break;
    }

}

inline void TdDfbInit( int w, int h, char *p, char *ap)
{	
	root = TwAppInit();
    signal(SIGTERM, &signal_handler);
	
    /*读取配置文件config.ini*/
    TwConfigParseFile(TGetExecutePath("../etc/config.ini"));

	int iRet = T_SUCCESS, rootW, rootH;
	main_win = TObjectGetFromName("main_win");
	TwGetSize(root, &rootW, &rootH);
	TwResize(main_win, rootW, rootH);
	TwDialSetBackend(BACKEND_TYPE_DEFAULT, w, h);

	/* DFB Init */
	DFBSurfaceDescription dsc;
	DFBCHECK(DirectFBInit(NULL, NULL));
	DFBCHECK(DirectFBCreate(&dfb));
	DFBCHECK(dfb->SetCooperativeLevel(dfb, DFSCL_FULLSCREEN));

	dsc.flags = DSDESC_CAPS | DSDESC_PIXELFORMAT;
    dsc.caps = DSCAPS_PRIMARY | DSCAPS_FLIPPING | DSCAPS_PREMULTIPLIED | DSCAPS_VIDEOONLY;
    dsc.pixelformat = DSPF_ARGB;

	DFBCHECK(dfb->CreateSurface(dfb, &dsc, &primary_surface));
	primary_surface->SetPorterDuff(primary_surface, DSPD_SRC_OVER);
    primary_surface->SetBlittingFlags(primary_surface, DSBLIT_BLEND_ALPHACHANNEL | DSBLIT_SRC_PREMULTCOLOR);
    primary_surface->SetDrawingFlags(primary_surface, DSDRAW_BLEND | DSDRAW_SRC_PREMULTIPLY);

	/* TD-cairo Init */
	DFBRectangle img_region = {0,0,w,h};
	DFBSurfaceRenderOptions options = DSRO_NONE;
	img_surface = cairo_directfb_surface_create_from_image(dfb, primary_surface, 
                                                             options, 
                                                             TGetExecutePath(p),
                                                             &img_region);
	csurface = cairo_directfb_surface_create(dfb, primary_surface);
    if (CAIRO_STATUS_SUCCESS != cairo_surface_status(csurface)) {
        printf("cairo_directfb_surface_create failed!\n");
        return;
    }

    cairo_cr = cairo_create(csurface);
	if (CAIRO_STATUS_SUCCESS != cairo_status(cairo_cr)) {
		printf("cairo_cr failed!\n");
		return;
	}
	
	TwUpdateShow();
}

inline void TdDfbFinish(){
	primary_surface->Release(primary_surface);
	img_surface->Release(img_surface);
	dfb->Release(dfb);
	cairo_surface_destroy(csurface);
	cairo_destroy(cairo_cr);
	TwDialCleanBackend();
}

unsigned long long TdDfbBlit( int w, int h, long t, long *frame)
{
    long i;

	for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++){
		cairo_save(cairo_cr);
    	cairo_translate(cairo_cr, (double)(myRand() % (SCREEN_WIDTH - w)), (double)(myRand() % (SCREEN_HEIGHT - h)));
    	cairo_set_source_surface(cairo_cr, img_surface, 0, 0);
    	cairo_paint(cairo_cr);
    	cairo_restore(cairo_cr);
	}

	*frame=i;
	
    return (unsigned long long) w * h * i;
}

unsigned long long TdDfbAlphaBlit( int w, int h, long t, long *frame)
{
    long i;

	for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++){
		cairo_save(cairo_cr);
		cairo_set_source_rgba(cairo_cr, 1.0, 1.0, 1.0, (float)(myRand()%255)/255);
		cairo_translate(cairo_cr, (double)(myRand() % (SCREEN_WIDTH - w)), (double)(myRand() % (SCREEN_HEIGHT - h)));
    	cairo_set_source_surface(cairo_cr, img_surface, 0, 0);
    	cairo_paint(cairo_cr);
    	cairo_restore(cairo_cr);
	}

	*frame=i;
	
    return (unsigned long long) w * h * i;
}

unsigned long long TdDfbStretchBlit( int w, int h, long t, long *frame)
{
    long i;

	*frame=i;

    return (unsigned long long) w* h * i;
}

unsigned long long TdDfbStretchAlphaBlit( int w, int h, long t, long *frame)
{
    long i;

	*frame=i;

    return (unsigned long long) w * h * i;
}

unsigned long long TdDfbFill( int w, int h, long t, long *frame)
{
    long i;

	for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++){
		cairo_rectangle(cairo_cr, (double)(myRand()%(SCREEN_WIDTH - w)), (double)(myRand()%(SCREEN_HEIGHT - h)), (double)w, (double)h);
		cairo_set_source_rgba(cairo_cr, (float)(myRand()%255)/255, (float)(myRand()%255)/255, (float)(myRand()%255)/255, 1.0);
    	cairo_fill(cairo_cr);
	}
	
	*frame=i;

    return (unsigned long long) w * h * i;
}

unsigned long long TdDfbAlphaFill( int w, int h, long t, long *frame)
{
    long i;

	for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++){
		cairo_rectangle(cairo_cr, (double)(myRand()%(SCREEN_WIDTH - w)), (double)(myRand()%(SCREEN_HEIGHT - h)), (double)w, (double)h);
		cairo_set_source_rgba(cairo_cr, (float)(myRand()%255)/255, (float)(myRand()%255)/255, (float)(myRand()%255)/255, (float)(myRand()%255)/255);
    	cairo_fill(cairo_cr);
	}
	
	*frame=i;

    return (unsigned long long) w * h * i;
}


unsigned long long TdDfbLine( int w, int h, long t, long *frame)
{
    long i;

	cairo_rectangle(cairo_cr, 0.0, 0.0, (double)SCREEN_WIDTH, (double)SCREEN_HEIGHT);
	cairo_set_source_rgba(cairo_cr, 0.0, 0.0, 0.0, 1.0);
    cairo_fill(cairo_cr);
	cairo_set_line_width(cairo_cr, 1.0);
	for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++){
		cairo_set_source_rgba(cairo_cr, (float)(myRand()%255)/255, (float)(myRand()%255)/255, (float)(myRand()%255)/255, 1.0);
		cairo_move_to(cairo_cr, (double)(myRand() % (SCREEN_WIDTH/2)), (double)(myRand() % SCREEN_HEIGHT));
    	cairo_line_to(cairo_cr, (double)(myRand() % (SCREEN_WIDTH/2) + (SCREEN_WIDTH/2)), (double)(myRand() % SCREEN_HEIGHT));
		cairo_stroke(cairo_cr);
	}

	*frame=i;

    return (unsigned long long) 1000 * i;
}

unsigned long long TdDfbAlphaLine( int w, int h, long t, long *frame)
{
    long i;

	cairo_rectangle(cairo_cr, 0.0, 0.0, (double)SCREEN_WIDTH, (double)SCREEN_HEIGHT);
	cairo_set_source_rgba(cairo_cr, 0.0, 0.0, 0.0, 1.0);
    cairo_fill(cairo_cr);
	cairo_set_line_width(cairo_cr, 1.0);
	for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++){
		cairo_set_source_rgba(cairo_cr, (float)(myRand()%255)/255, (float)(myRand()%255)/255, (float)(myRand()%255)/255, (float)(myRand()%255)/255);
		cairo_move_to(cairo_cr, (double)(myRand() % (SCREEN_WIDTH/2)), (double)(myRand() % SCREEN_HEIGHT));
    	cairo_line_to(cairo_cr, (double)(myRand() % (SCREEN_WIDTH/2) + (SCREEN_WIDTH/2)), (double)(myRand() % SCREEN_HEIGHT));
		cairo_stroke(cairo_cr);
	}

	*frame=i;

    return (unsigned long long) i * 1000;
}


unsigned long long TdDfbRect( int w, int h, long t, long *frame)
{
    long i;
	double x, y;

	cairo_rectangle(cairo_cr, 0.0, 0.0, (double)SCREEN_WIDTH, (double)SCREEN_HEIGHT);
	cairo_set_source_rgba(cairo_cr, 0.0, 0.0, 0.0, 1.0);
    cairo_fill(cairo_cr);
	cairo_set_line_width(cairo_cr, 1.0);
	for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++){
		x = (double)(myRand()%(SCREEN_WIDTH - w));
		y = (double)(myRand()%(SCREEN_HEIGHT - h));

		/* Top */
		cairo_set_source_rgba(cairo_cr, (float)(myRand()%255)/255, (float)(myRand()%255)/255, (float)(myRand()%255)/255, 1.0);
		cairo_move_to(cairo_cr, x, y);
    	cairo_line_to(cairo_cr, x + w, y);

		/* Left */
		cairo_set_source_rgba(cairo_cr, (float)(myRand()%255)/255, (float)(myRand()%255)/255, (float)(myRand()%255)/255, 1.0);
		cairo_move_to(cairo_cr, x, y);
    	cairo_line_to(cairo_cr, x, y + h);

		/* Bottom */
		cairo_set_source_rgba(cairo_cr, (float)(myRand()%255)/255, (float)(myRand()%255)/255, (float)(myRand()%255)/255, 1.0);
		cairo_move_to(cairo_cr, x, y + h);
    	cairo_line_to(cairo_cr, x + w, y + h);

		/* Right */
		cairo_set_source_rgba(cairo_cr, (float)(myRand()%255)/255, (float)(myRand()%255)/255, (float)(myRand()%255)/255, 1.0);
		cairo_move_to(cairo_cr, x + w, y);
    	cairo_line_to(cairo_cr, x + w, y + h);
		
		cairo_stroke(cairo_cr);
	}

	*frame=i;

    return (unsigned long long) i * 1000;
}

unsigned long long TdDfbAlphaRect( int w, int h, long t, long *frame)
{
    long i;
	double x, y;

	cairo_rectangle(cairo_cr, 0.0, 0.0, (double)SCREEN_WIDTH, (double)SCREEN_HEIGHT);
	cairo_set_source_rgba(cairo_cr, 0.0, 0.0, 0.0, 1.0);
    cairo_fill(cairo_cr);
	cairo_set_line_width(cairo_cr, 1.0);
	for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++){
		x = (double)(myRand()%(SCREEN_WIDTH - w));
		y = (double)(myRand()%(SCREEN_HEIGHT - h));

		/* Top */
		cairo_set_source_rgba(cairo_cr, (float)(myRand()%255)/255, (float)(myRand()%255)/255, (float)(myRand()%255)/255, (float)(myRand()%255)/255);
		cairo_move_to(cairo_cr, x, y);
    	cairo_line_to(cairo_cr, x + w, y);

		/* Left */
		cairo_set_source_rgba(cairo_cr, (float)(myRand()%255)/255, (float)(myRand()%255)/255, (float)(myRand()%255)/255, (float)(myRand()%255)/255);
		cairo_move_to(cairo_cr, x, y);
    	cairo_line_to(cairo_cr, x, y + h);

		/* Bottom */
		cairo_set_source_rgba(cairo_cr, (float)(myRand()%255)/255, (float)(myRand()%255)/255, (float)(myRand()%255)/255, (float)(myRand()%255)/255);
		cairo_move_to(cairo_cr, x, y + h);
    	cairo_line_to(cairo_cr, x + w, y + h);

		/* Right */
		cairo_set_source_rgba(cairo_cr, (float)(myRand()%255)/255, (float)(myRand()%255)/255, (float)(myRand()%255)/255, (float)(myRand()%255)/255);
		cairo_move_to(cairo_cr, x + w, y);
    	cairo_line_to(cairo_cr, x + w, y + h);
		
		cairo_stroke(cairo_cr);
	}

	*frame=i;

    return (unsigned long long) i * 1000;
}


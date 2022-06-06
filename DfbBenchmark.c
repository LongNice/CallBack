#include <DfbBenchmark.h>

inline void DfbInit(int w, int h, char *p, char *ap){

	DFBSurfaceDescription dsc;
	DFBCHECK(DirectFBInit( NULL, NULL ));
	DFBCHECK(DirectFBCreate(&dfb));
	DFBCHECK(dfb->SetCooperativeLevel(dfb, DFSCL_FULLSCREEN));

	/* Primary Surface */
	dsc.flags = (DFBSurfaceDescriptionFlags)(DSDESC_HEIGHT | DSDESC_WIDTH | DSDESC_CAPS | DSDESC_PIXELFORMAT);
	dsc.caps = (DFBSurfaceCapabilities)(DSCAPS_PRIMARY | DSCAPS_FLIPPING);
	dsc.pixelformat = DSPF_ARGB;
	dsc.width = SCREEN_WIDTH;
	dsc.height = SCREEN_HEIGHT;
	DFBCHECK(dfb->CreateSurface( dfb, &dsc, &primary ));

	/* Image Surface */
	dsc.caps = (DFBSurfaceCapabilities)(DSCAPS_VIDEOONLY);
	dsc.width = w;
	dsc.height = h;
	DFBCHECK(dfb->CreateSurface( dfb, &dsc, &img ));
	DFBCHECK(dfb->CreateImageProvider(dfb, p, &provider));
	DFBCHECK(provider->GetSurfaceDescription(provider, &dsc));
	DFBCHECK(provider->RenderTo(provider, img, NULL));
	DFBCHECK(provider->Release(provider));

	/* Alpha Image Surface */
	DFBCHECK(dfb->CreateSurface( dfb, &dsc, &alphaImg ));
	DFBCHECK(dfb->CreateImageProvider(dfb, ap, &provider));
	DFBCHECK(provider->GetSurfaceDescription(provider, &dsc));
	DFBCHECK(provider->RenderTo(provider, alphaImg, NULL));
	DFBCHECK(provider->Release(provider));
	
}

inline void DfbFinish(){
	img->Release(img);
	alphaImg->Release(alphaImg);
	primary->Release(primary);
	dfb->Release(dfb);
}

/* DFB Blit */
unsigned long long DfbBlit(int w, int h, long t, long *frame){

	primary->SetColor(primary, 0, 0, 0, 255);
	primary->FillRectangle(primary, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	primary->SetBlittingFlags(primary, DSBLIT_NOFX);

	/* Get Acceleration Mask */
	//GetAccelerationMask(primary, img, DFXL_BLIT);
	
	long i;

	for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++){
		if(w == SCREEN_WIDTH && h == SCREEN_HEIGHT){
			DFBCHECK(primary->Blit(primary, img, NULL, 0, 0));
			}
		else{
			DFBCHECK(primary->Blit(primary, img, NULL, myRand() % (SCREEN_WIDTH - w), myRand() % (SCREEN_HEIGHT - h)));
			}
		if(flip)
			DFBCHECK(primary->Flip(primary, NULL, DSFLIP_WAITFORSYNC));
	}

	*frame=i;

	return (unsigned long long) w * h * i;
}

/* DFB Alpha Blit */
unsigned long long DfbAlphaBlit(int w, int h, long t, long *frame){

	primary->SetColor(primary, 0, 0, 0, 255);
	primary->FillRectangle(primary, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	primary->SetBlittingFlags(primary, DSBLIT_BLEND_ALPHACHANNEL | DSBLIT_SRC_PREMULTIPLY);
	primary->SetPorterDuff(primary, DSPD_SRC_OVER);
	
	/* Get Acceleration Mask */
	//showAccelerated( DFXL_BLIT, img );
	
	long i;

	for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++){
		if(w == SCREEN_WIDTH && h == SCREEN_HEIGHT){
			DFBCHECK(primary->Blit(primary, alphaImg, NULL, 0, 0));
			}
		else{
			DFBCHECK(primary->Blit(primary, alphaImg, NULL, myRand() % (SCREEN_WIDTH - w), myRand() % (SCREEN_HEIGHT - h)));
			}
		if(flip)
			DFBCHECK(primary->Flip(primary, NULL, DSFLIP_WAITFORSYNC));
	}

	*frame=i;

	return (unsigned long long) w * h * i;
}


/* DFB FillRect */
unsigned long long DfbFill(int w, int h, long t, long *frame){
	
	primary->SetColor(primary, 0, 0, 0, 255);
	primary->FillRectangle(primary, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	primary->SetDrawingFlags(primary, DSDRAW_NOFX);
	
	long i;

	for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++){
		DFBCHECK(primary->SetColor(primary, 
					myRand() % 0xFF, myRand() % 0xFF, myRand() % 0xFF, 0xFF));
		if(w == SCREEN_WIDTH && h == SCREEN_HEIGHT){
			DFBCHECK(primary->FillRectangle(primary, 0, 0, w, h));
			}
		else{
			DFBCHECK(primary->FillRectangle(primary, myRand() % (SCREEN_WIDTH - w), myRand() % (SCREEN_HEIGHT - h), w, h));
			}
		if(flip)
			DFBCHECK(primary->Flip(primary, NULL, DSFLIP_WAITFORSYNC));
	}

	*frame=i;

	return (unsigned long long) w * h * i;
}

/* DFB Alpha FillRect */
unsigned long long DfbAlphaFill(int w, int h, long t, long *frame){
	
	primary->SetColor(primary, 0, 0, 0, 255);
	primary->FillRectangle(primary, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	primary->SetDrawingFlags(primary, DSDRAW_BLEND);
	
	long i;

	for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++){
		DFBCHECK(primary->SetColor(primary, 
					myRand() % 0xFF, myRand() % 0xFF, myRand() % 0xFF, myRand() % 0xFF));
		if(w == SCREEN_WIDTH && h == SCREEN_HEIGHT){
			DFBCHECK(primary->FillRectangle(primary, 0, 0, w, h));
			}
		else{
			DFBCHECK(primary->FillRectangle(primary, myRand() % (SCREEN_WIDTH - w), myRand() % (SCREEN_HEIGHT - h), w, h));
			}
		if(flip)
			DFBCHECK(primary->Flip(primary, NULL, DSFLIP_WAITFORSYNC));
	}

	*frame=i;

	return (unsigned long long) w * h * i;
}


/* DFB DrawLine */
unsigned long long DfbLine(int w, int h, long t, long *frame){

	DFBRegion lines[10];
	int x, y, dx, dy;
	primary->SetColor(primary, 0, 0, 0, 255);
	primary->FillRectangle(primary, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	primary->SetDrawingFlags(primary, DSDRAW_NOFX);
	
	long i, l;

	for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++){
		for (l=0; l<10; l++) {
		   x  = myRand() % (SCREEN_WIDTH/2);
		   y  = myRand() % SCREEN_HEIGHT;
		   dx = myRand() % (SCREEN_WIDTH/2) + (SCREEN_WIDTH/2);
		   dy = myRand() % SCREEN_HEIGHT;

		   lines[l].x1 = x;
		   lines[l].y1 = y;
		   lines[l].x2 = dx;
		   lines[l].y2 = dy;
	  	}
	  
	  	DFBCHECK(primary->SetColor( primary,
					  myRand() % 0xFF, myRand() % 0xFF, myRand() % 0xFF, 0xFF ));
	  	DFBCHECK(primary->DrawLines( primary, lines, 10 ));	

		if(flip)
			DFBCHECK(primary->Flip(primary, NULL, DSFLIP_WAITFORSYNC));
	}
	
	*frame=i;

	return (unsigned long long) 10 * 1000 * i;

}

/* DFB DrawAlphaLine */
unsigned long long DfbAlphaLine(int w, int h, long t, long *frame){

	DFBRegion lines[10];
	int x, y, dx, dy;
	primary->SetColor(primary, 0, 0, 0, 255);
	primary->FillRectangle(primary, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	primary->SetDrawingFlags(primary, DSDRAW_BLEND);
	
	long i, l;

	for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++){
		for (l=0; l<10; l++) {
		   x  = myRand() % (SCREEN_WIDTH/2);
		   y  = myRand() % SCREEN_HEIGHT;
		   dx = myRand() % (SCREEN_WIDTH/2) + (SCREEN_WIDTH/2);
		   dy = myRand() % SCREEN_HEIGHT;

		   lines[l].x1 = x;
		   lines[l].y1 = y;
		   lines[l].x2 = dx;
		   lines[l].y2 = dy;
	  	}
	  
	  	DFBCHECK(primary->SetColor( primary,
					  myRand() % 0xFF, myRand() % 0xFF, myRand() % 0xFF, myRand() % 0xFF ));
	  	DFBCHECK(primary->DrawLines( primary, lines, 10 ));	

		if(flip)
			DFBCHECK(primary->Flip(primary, NULL, DSFLIP_WAITFORSYNC));
	}
	
	*frame=i;

	return (unsigned long long) 10 * 1000 * i;

}


/* DFB DrawRect */
unsigned long long DfbRect(int w, int h, long t, long *frame){

	primary->SetColor(primary, 0, 0, 0, 255);
	primary->FillRectangle(primary, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	primary->SetDrawingFlags(primary, DSDRAW_NOFX);
	
	long i;

	for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++){
	  DFBCHECK(primary->SetColor( primary,
				 myRand() % 0xFF, myRand() % 0xFF, myRand() % 0xFF, 0xFF ));
	  if(w == SCREEN_WIDTH && h == SCREEN_HEIGHT){
	  DFBCHECK(primary->DrawRectangle( primary,
				 0, 0, w, h ));
	  	}
	  else{
	  DFBCHECK(primary->DrawRectangle( primary,
				 myRand() % (SCREEN_WIDTH-w), myRand() % (SCREEN_HEIGHT-h), w, h ));
	  	}
	  if(flip)
		DFBCHECK(primary->Flip(primary, NULL, DSFLIP_WAITFORSYNC));
	}

	*frame=i;
	
	return (unsigned long long) i * 1000;
}

/* DFB DrawAlphaRect */
unsigned long long DfbAlphaRect(int w, int h, long t, long *frame){

	primary->SetColor(primary, 0, 0, 0, 255);
	primary->FillRectangle(primary, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	primary->SetDrawingFlags(primary, DSDRAW_BLEND);
	
	long i;

	for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++){
	  DFBCHECK(primary->SetColor( primary,
				 myRand() % 0xFF, myRand() % 0xFF, myRand() % 0xFF, myRand() % 0xFF ));
	  if(w == SCREEN_WIDTH && h == SCREEN_HEIGHT){
	  DFBCHECK(primary->DrawRectangle( primary,
				 0, 0, w, h ));
	  	}
	  else{
	  DFBCHECK(primary->DrawRectangle( primary,
				 myRand() % (SCREEN_WIDTH-w), myRand() % (SCREEN_HEIGHT-h), w, h ));
	  	}
	  if(flip)
		DFBCHECK(primary->Flip(primary, NULL, DSFLIP_WAITFORSYNC));
	}

	*frame=i;
	
	return (unsigned long long) i * 1000;
}


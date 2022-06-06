#include <TdBenchmark.h>

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


inline void TdInit(int w, int h)
{
    root = TwAppInit();
    signal(SIGTERM, &signal_handler);
	
    /*读取配置文件config.ini*/
    TwConfigParseFile(TGetExecutePath("../etc/config.ini"));
	
    btn = TObjectGetFromName("button1");
	TwResize(btn, w, h);
	TwUpdateShow();
}

inline void TdFinish()
{
	TwDestroy(root);
	TwDestroy(btn);
}

unsigned long long TdBlit(int w, int h, long t, long *frame)
{
    long i, start;
	
    for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++)
    {
    	if(forced_fps_on)
			start = myTime();

		TwHide(btn);
        TwMove(btn, myRand() % (SCREEN_WIDTH - w), myRand() % (SCREEN_HEIGHT - h));
        TwShow(btn);
        TwUpdateShow();

		if(forced_fps_on)
		{
			if((myTime() - start) < 1000000/forced_fps)
				usleep(1000000/forced_fps - (myTime() - start));
		}
    }

	*frame=i;

    return (unsigned long long) w * h * i;
}

unsigned long long TdAlphaBlit(int w, int h, long t, long *frame)
{
    long i, start;

   	for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++)
    {
    	if(forced_fps_on)
    		start = myTime();

		TwHide(btn);
		TwButtonSetAlpha(btn, myRand() % 0xff);
        TwMove(btn, myRand() % (SCREEN_WIDTH - w), myRand() % (SCREEN_HEIGHT - h));
        TwShow(btn);
        TwUpdateShow();

		if(forced_fps_on)
		{
			if((myTime() - start) < 1000000/forced_fps)
				usleep(1000000/forced_fps - (myTime() - start));
		}
    }

	*frame=i;

    return (unsigned long long) w * h * i;
}

unsigned long long TdStretchBlit(int w, int h, long t, long *frame)
{
    long i, start;
	int grow = 0;
	unsigned long long pixels = 0;
	TwButtonSetAlpha(btn, 255);
    TwMove(btn, (SCREEN_WIDTH - w) / 2, 0); // Move to central position

    for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++)
    {
		if(forced_fps_on)
    		start = myTime();

		TwHide(btn);
        TwResize(btn, w + grow, h + grow);
        TwShow(btn);
        TwUpdateShow();

		if(forced_fps_on)
		{
			if((myTime() - start) < 1000000/forced_fps)
				usleep(1000000/forced_fps - (myTime() - start));
		}

		grow++;
		pixels += (w + grow) * (h + grow);
		
		if((w + grow) == SCREEN_WIDTH || (h + grow) == SCREEN_HEIGHT)
			grow = 0;

    }

	*frame=i;

    return pixels;
}

unsigned long long TdStretchAlphaBlit(int w, int h, long t, long *frame)
{
    long i, start;
	int grow = 0;
	unsigned long long pixels = 0;
	TwButtonSetAlpha(btn, myRand() % 0xff);
    TwMove(btn, (SCREEN_WIDTH - w) / 2, 0); // Move to central position

    for(i = 0 ; i%100 || myTime() < (t + DEMOTIME) ; i++)
    {
    	if(forced_fps_on)
    		start = myTime();

		TwHide(btn);
		TwButtonSetAlpha(btn, myRand() % 0xff);
        TwResize(btn, w + grow, h + grow);
        TwShow(btn);
        TwUpdateShow();

		if(forced_fps_on)
		{
			if((myTime() - start) < 1000000/forced_fps)
				usleep(1000000/forced_fps - (myTime() - start));
		}

		grow++;
		pixels += (w + grow) * (h + grow);

		if((w + grow) == SCREEN_WIDTH || (h + grow) == SCREEN_HEIGHT)
			grow = 0;

    }

	*frame=i;

    return pixels;
}


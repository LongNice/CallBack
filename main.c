#include <main.h>

FrameworkSet AllFramework;

Framework Tddemos[TD_FUNC_NUM] = {
	{"TdBlit", "MPixel/sec", 0, TdBlit},
	{"TdAlphaBlit", "MPixel/sec", 0, TdAlphaBlit},
	{"TdStretchBlit", "MPixel/sec", 0, TdStretchBlit},
	{"TdStretchAlphaBlit", "MPixel/sec", 0, TdStretchAlphaBlit},
};

Framework TdDfbdemos[TDDFB_FUNC_NUM] = {
    {"TdDfbBlit", "MPixel/sec", 0, TdDfbBlit},
    {"TdDfbAlphaBlit", "MPixel/sec", 0, TdDfbAlphaBlit},
    //{"TdDfbStretchBlit", "MPixel/sec", 0, TdDfbStretchBlit},
    //{"TdDfbStretchAlphaBlit", "MPixel/sec", 0, TdDfbStretchAlphaBlit},
    {"TdDfbFill", "MPixel/sec", 0, TdDfbFill},
    {"TdDfbAlphaFill", "MPixel/sec", 0, TdDfbAlphaFill},
    {"TdDfbLine", "KLine/sec", 0, TdDfbLine},
    {"TdDfbAlphaLine", "KLine/sec", 0, TdDfbAlphaLine},
    {"TdDfbRect", "KRect/sec", 0, TdDfbRect},
    {"TdDfbAlphaRect", "KRect/sec", 0, TdDfbAlphaRect},
};

Framework Dfbdemos[DFB_FUNC_NUM] = {
    {"DfbBlit", "MPixel/sec", 0, DfbBlit},
    {"DfbAlphaBlit", "MPixel/sec", 0, DfbAlphaBlit},
    {"DfbFill", "MPixel/sec", 0, DfbFill},
    {"DfbAlphaFill", "MPixel/sec", 0, DfbAlphaFill},
    {"DfbLine", "KLine/sec", 0, DfbLine},
    {"DfbAlphaLine", "KLine/sec", 0, DfbAlphaLine},
    {"DfbRect", "KRect/sec", 0, DfbRect},
    {"DfbAlphaRect", "KRect/sec", 0, DfbAlphaRect}
};

void init()
{
	/* TD Framework */
	AllFramework.fw[0] = &Tddemos;
	AllFramework.init[0] = TdInit;
	AllFramework.finish[0] = TdFinish;
	AllFramework.func_num[0] = TD_FUNC_NUM;

	/* TD-DFB Framework */
	AllFramework.fw[1] = &TdDfbdemos;
	AllFramework.init[1] = TdDfbInit;
	AllFramework.finish[1] = TdDfbFinish;
	AllFramework.func_num[1] = TDDFB_FUNC_NUM;

	/* DFB Framework */
	AllFramework.fw[2] = &Dfbdemos;
	AllFramework.init[2] = DfbInit;
	AllFramework.finish[2] = DfbFinish;
	AllFramework.func_num[2] = DFB_FUNC_NUM;

	/* Qt Framework */

	/* GAL Framework */

	/* NANO2D Framework */
	
}

inline void parseArgv(int argc, char *argv[], InputArgs *args) {
	const char *optstring = "p:a:w:h:f:FTtdqgnH";
	int sel;
	struct option opts[] = {
		{"TD-framework", 0, NULL, 'T'},
		{"TD-DFB-framework", 0, NULL, 't'},
		{"DFB-framework", 0, NULL, 'd'},
		{"Qt-framework", 0, NULL, 'q'},
		{"GPU-Linux-framework", 0, NULL, 'g'},
		{"GPU-eCos-framework", 0, NULL, 'n'},
		{"Do Flip", 0, NULL, 'F'},
		{"Forced FPS", 0, NULL, 'f'},
		{"Image path", 1, NULL, 'p'},
		{"Alpha image path", 1, NULL, 'a'},
		{"Image width", 1, NULL, 'w'},
		{"Image height", 1, NULL, 'h'},
		{"help", 0, NULL, 'H'}
	};
		
	while ((sel = getopt_long(argc, argv, optstring, opts, NULL)) != -1) {
		switch (sel) {
		case 'T':
			printf("[%s] Framework: TD\n", __FUNCTION__);
			args->fw[0] = true;
			break;
		case 't':
			printf("[%s] Framework: TD-DFB\n", __FUNCTION__);
			args->fw[1] = true;
			break;
		case 'd':
			printf("[%s] Framework: DFB\n", __FUNCTION__);
			args->fw[2] = true;
			break;
		case 'q':
			printf("[%s] Framework: Qt\n", __FUNCTION__);
			args->fw[3] = true;
			break;
		case 'g':
			printf("[%s] Framework: Gal\n", __FUNCTION__);
			args->fw[4] = true;
			break;
		case 'n':
			printf("[%s] Framework: N2D\n", __FUNCTION__);
			args->fw[5] = true;
			break;
		case 'F':
			printf("[%s] do Flip\n", __FUNCTION__);
			flip = true;
			break;
		case 'f':
			printf("[%s] Forced FPS: %d\n", __FUNCTION__, atoi(optarg));
			if (optarg == NULL){
				printf("Invalid Args (Forced FPS)\n");
				exit(0);
			}
			forced_fps = atoi(optarg);
			forced_fps_on = true;
			break;
		case 'p':
			printf("[%s] Image Path: %s\n", __FUNCTION__, optarg);
			if (optarg == NULL){
				printf("Invalid Args (Image Path)\n");
				exit(0);
			}
			args->img_path = optarg;
			break;
		case 'a':
			printf("[%s] Alpha Image Path: %s\n", __FUNCTION__, optarg);
			if (optarg == NULL){
				printf("Invalid Args (Alpha Image Path)\n");
				exit(0);
			}
			args->alphaImg_path = optarg;
			break;
		case 'w':
			printf("[%s] Image Width: %d\n", __FUNCTION__, atoi(optarg));
			args->img_width = atoi(optarg);
			break;
		case 'h':
			printf("[%s] Image Height: %d\n", __FUNCTION__, atoi(optarg));
			args->img_height = atoi(optarg);
			break;
		case 'H':
			printf("[%s] This is help: \n", __FUNCTION__);
			helpList();
			break;
		default:
			printf("[%s] This is help: \n", __FUNCTION__);
			helpList();
		}
	}

	if(args->img_width == 0)
		args->img_width = IMAGE_WIDTH;
	if(args->img_height == 0)
		args->img_height = IMAGE_HEIGHT;
	if(args->img_path == NULL)
		args->img_path = "./../style/pnggrad8rgb_256x256.jpg";
	if(args->alphaImg_path == NULL)
		args->alphaImg_path = "./../style/pnggrad8rgb_256x256_alpha.png";
	
	printf("[%s] Image width: %d Image height: %d\n",
	__FUNCTION__, args->img_width, args->img_height);	
}

int main(int argc, const char **argv)
{  
	InputArgs iArgs = {.fw[0]=false, .fw[1]=false, .fw[2]=false, .fw[3]=false, .fw[4]=false, .fw[5]=false,
						.img_path=NULL, .alphaImg_path=NULL, .img_width=0, .img_height=0};
	int width, height;
	char *p, *ap;

	/* Process Initialization */
	parseArgv(argc, (char **)argv, &iArgs);
	width = iArgs.img_width;
	height = iArgs.img_height;
	p = iArgs.img_path;
	ap = iArgs.alphaImg_path;
	init();

    /* Main loop */
    int no, func;
    printf("Benchmark start!\n");
    for(no = 0 ; no < FW_NUM ; no++)
    {
    	if(iArgs.fw[no])
        {	
			/* Framework Initialization */
			AllFramework.init[no](width, height, p, ap);
			sync();
			
			for(func = 0 ; func < AllFramework.func_num[no] ; func++)
			{
				long ticks, start, dt, frame = 0;
				unsigned long long pixels = 0;
				struct tms stms, etms;
				int load;

				/* Get Ready*/
				if(dfb!=NULL)
				dfb->WaitIdle( dfb );
				sync();

				/* Get start time */
        		times(&stms);
				start = myTime();

				/* Forced FPS ?*/
				if(forced_fps_on)
					usleep(1000); // Avoid to arithmetic exception. 

        		/* Run! */
        		pixels = (*AllFramework.fw[no][func].func)(width, height, start, &frame);

				/* Waitting for GPU idle */
				if(dfb!=NULL)
				dfb->WaitIdle( dfb );

        		/* Get ending time */
				dt = myTime() - start;
				times(&etms);

        		/* Caculate CPU load */
        		ticks = etms.tms_utime - stms.tms_utime + etms.tms_stime - stms.tms_stime;
				load = ticks * 1000 / (sysconf(_SC_CLK_TCK) * dt / 1000);

        		/* Caculate pixel rate */
       			AllFramework.fw[no][func].result = pixels / (unsigned long long)dt;

        		/* Print results */
        		printf("[%s] FPS %ld (CPU: %3d.%d%%) %4llu.%.3llu %s\n", 
        		AllFramework.fw[no][func].desc, frame * 1000 / dt, load / 10, load % 10, AllFramework.fw[no][func].result / 1000, 
        		AllFramework.fw[no][func].result % 1000, AllFramework.fw[no][func].unit);

			}

			/* Release */
			AllFramework.finish[no]();
			
    	}
    }

    return 0;
}


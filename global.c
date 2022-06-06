#include <global.h>

/* Global */
bool flip = false;
bool forced_fps_on = false;
long forced_fps = 0;


/* Td */
TWidget *root = NULL;
TWidget *btn = NULL;

/* TdDfb */
TWidget *main_win = NULL;
IDirectFBSurface *primary_surface = NULL; 
IDirectFBSurface *img_surface = NULL; 
cairo_surface_t *csurface = NULL;
cairo_t *cairo_cr = NULL;

/* Dfb */
IDirectFB *dfb = NULL;
IDirectFBSurface *primary = NULL;
IDirectFBSurface *img = NULL;
IDirectFBSurface *alphaImg = NULL;
IDirectFBImageProvider *provider = NULL;

inline void helpList()
{
	printf("Usage: GraphicsBenchmark [OPTION]\n"
	       "\n"
		   "...\n"
	       "\n"
	      );
	exit(0);
}

/* return: millisec */
inline long myTime()
{
    struct timeval tv;

    gettimeofday (&tv, NULL);

    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/* myRand */
static unsigned int rand_pool = 0x12345678;
static unsigned int rand_add  = 0x87654321;
inline unsigned int myRand()
{
     rand_pool ^= ((rand_pool << 7) | (rand_pool >> 25));
     rand_pool += rand_add;
     rand_add  += rand_pool;

     return rand_pool;
}


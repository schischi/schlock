#include "xutils.h"

void win_screen_res(int *w, int *h, Display *display)
{
    XRRScreenResources *s;
    XRRCrtcInfo *crtc_info;

    s = XRRGetScreenResources (display, DefaultRootWindow(display));
    crtc_info = XRRGetCrtcInfo (display, s, s->crtcs[0]);
    *w = crtc_info->width;
    *h = crtc_info->height;
}

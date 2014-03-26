/* This is hmmlock written by Adrien Schildknecht (c) 2013
 * Email: adrien+dev@schischi.me
 * Feel free to copy and redistribute in terms of the BSD license
 */
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cairo-xlib.h>
#include <X11/extensions/shape.h>
#include <librsvg-2.0/librsvg/rsvg.h>

#include "../common/xutils.h"
#include "config.h"

#define LOOP_SLEEP 20
#define fatal(x) \
    do{ if(errno) perror(x); else fprintf(stderr, "%s", x); exit(1); }while(0)
#define SCHLOCK_PATH "/home/schischi/dev/schlock/schlock"
#define SVG_PATH "hmm.svg"

static char* const schlock[] = {SCHLOCK_PATH, "schlock", NULL};

int main(int argc, char* argv[])
{
    Window w;
    pid_t pid;
    Display *dpy;
    Pixmap shape;
    cairo_surface_t *surface, *shapeSurface;
    cairo_t *cairo, *shapeCairo;
    RsvgHandle *rsvg_handle;
    int width, height, wWidth, wHeight;

    t_config *conf = config_load(argc, argv);
    if (conf == NULL)
        fatal("Config file not found\n");

    if(!(rsvg_handle = rsvg_handle_new_from_file(conf->svg, NULL)))
        fatal("Unable to load svg\n");

    if(!(dpy = XOpenDisplay(NULL)))
        fatal("Can't open display\n");

    win_screen_res(&wWidth, &wHeight, dpy);
    RsvgDimensionData dim;
    rsvg_handle_get_dimensions (rsvg_handle, &dim);
    width = dim.width;
    height = dim.height;

    w = XCreateSimpleWindow(dpy, RootWindow(dpy, DefaultScreen(dpy)),
            (wWidth - width) / 2 - conf->x, (wHeight - height) / 2 + conf->y,
            width, height, 0, 0, BlackPixel(dpy, 0));
    config_delete(conf);
    XSetWindowAttributes winattr;
    winattr.override_redirect = 1;
    XChangeWindowAttributes(dpy, w, CWOverrideRedirect, &winattr);

    XSelectInput(dpy, w, StructureNotifyMask | ExposureMask);
    XMapWindow(dpy, w);

    surface = cairo_xlib_surface_create(dpy, w, DefaultVisual(dpy, 0), width,
            height);
    cairo = cairo_create(surface);

    shape = XCreatePixmap(dpy, w, width, height, 1);
    shapeSurface = cairo_xlib_surface_create_for_bitmap(dpy, shape,
            DefaultScreenOfDisplay(dpy), width, height);
    shapeCairo = cairo_create(shapeSurface);

    do {
        pid = vfork();
    } while((pid == -1) && (errno == EAGAIN));

    if(!pid) {
        execv(schlock[0], schlock + 1);
        fatal("Schlock not found\n");
    }

    do {
        XEvent e;
        if(waitpid(-1, (int[]){0}, WNOHANG))
            break;
        if(XCheckWindowEvent(dpy, w, StructureNotifyMask | ExposureMask, &e)) {
            /* Draw SVG */
            cairo_set_source_rgba(cairo, 1, 1, 1, 0);
            rsvg_handle_render_cairo(rsvg_handle, cairo);
            cairo_paint(cairo);
            /* Clear Shape */
            cairo_set_operator(shapeCairo, CAIRO_OPERATOR_CLEAR);
            cairo_rectangle(shapeCairo, 0, 0, width, height);
            cairo_fill(shapeCairo);
            /* Shape */
            cairo_set_operator(shapeCairo, CAIRO_OPERATOR_OVER);
            rsvg_handle_render_cairo(rsvg_handle, shapeCairo);
            rsvg_handle_close(rsvg_handle, NULL);
            cairo_fill(shapeCairo);

            XShapeCombineMask(dpy, w, ShapeBounding, 0, 0,
                    cairo_xlib_surface_get_drawable(shapeSurface), ShapeSet);
        }
        usleep(1000 * LOOP_SLEEP);
    } while(1);
    XFreePixmap(dpy, shape);
    cairo_destroy(shapeCairo);
    cairo_surface_destroy(surface);
    cairo_surface_destroy(shapeSurface);
    rsvg_handle_close(rsvg_handle, NULL);
    XDestroyWindow(dpy, w);
    XCloseDisplay(dpy);
    return EXIT_SUCCESS;
}

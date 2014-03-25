#ifndef XUTILS_H
# define XUTILS_H

# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include <X11/keysym.h>
# include <X11/extensions/Xrandr.h>

void win_screen_res(int *w, int *h, Display *display);

#endif /* !XUTILS_H */

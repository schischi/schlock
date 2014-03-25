#ifndef LOCK_H
# define LOCK_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/Xrandr.h>

#include "buf.h"

struct s_lock {
    Display *display;
    Window win;
    GC gc;
};
struct s_lock typedef t_lock;

t_lock win_create();
char win_getc(t_lock lock, t_buf *buf);
void win_str(t_lock lock, t_buf *buf);


#endif /* !LOCK_H */

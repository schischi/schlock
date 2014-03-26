#ifndef LOCK_H
# define LOCK_H

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/Xrandr.h>

#include "buf.h"

struct s_lock {
    Display *display;
    Window win;
    GC gc;
    char *msg;
    size_t msg_len;
    struct timeval started;
};
struct s_lock typedef t_lock;

t_lock win_create(char *msg);
char win_getc(t_lock lock, t_buf *buf);
void win_str(t_lock lock, t_buf *buf);


#endif /* !LOCK_H */

#include "lock.h"
#include "../common/xutils.h"

const int win_width = 400;
const int win_height = 100;

t_lock win_create()
{
    t_lock lock;
    Window root_win;
    int screen;
    XEvent evt;
    int white, black;
    int screen_width, screen_height;

    lock.display = XOpenDisplay(NULL);
    screen = DefaultScreen(lock.display);
    win_screen_res(&screen_width, &screen_height, lock.display);

    root_win = RootWindow(lock.display, screen);
    white = WhitePixel(lock.display, screen);
    black = BlackPixel(lock.display, screen);
    lock.win = XCreateSimpleWindow(lock.display, root_win,
            (screen_width - win_width) / 2,
            (screen_height - win_height) / 2,
            win_width, win_height, 0, black, black);
    XSelectInput(lock.display, lock.win, StructureNotifyMask);
    XMapWindow(lock.display, lock.win);
    XStoreName(lock.display, lock.win, "schlock");
    lock.gc = XCreateGC(lock.display, lock.win, 0, NULL);
    XSetForeground(lock.display, lock.gc, white);
    while (evt.type != MapNotify)
        XNextEvent(lock.display, &evt);
    XFlush(lock.display);

    /* Grab input */
    XGrabPointer(lock.display, lock.win, 0, ButtonPressMask | ButtonReleaseMask
            | PointerMotionMask, GrabModeSync, GrabModeAsync, lock.win, None,
            CurrentTime);
    XGrabKeyboard(lock.display, lock.win, True, GrabModeAsync, GrabModeAsync, CurrentTime);
    return lock;
}

char win_getc(t_lock lock, t_buf *buf)
{
    KeySym key;
    char c;
    XEvent evt;

    XSelectInput(lock.display, lock.win, KeyPressMask);
    XNextEvent(lock.display, &evt);
    if (evt.type == KeyPress) {
        XLookupString(&evt.xkey, &c, 1, &key, NULL);
        if (key == XK_Return)
            return 0;
        else if (IsFunctionKey(key)
                || IsKeypadKey(key)
                || IsMiscFunctionKey(key)
                || IsPFKey(key)
                ||IsPrivateKeypadKey(key))
            return 1;
        else if (key == XK_BackSpace) {
            if (buf->i)
                buf->buf[--(buf->i)] = 0;
        }
        else if (buf->i < buf->max - 1) {
            buf->buf[buf->i] = c;
            buf->buf[++(buf->i)] = 0;
        }
    }
    return 1;
}

void win_str(t_lock lock, t_buf *buf)
{
    t_buf *h = buf_create(buf->max);

    for (int i = 0; i < buf->i; ++i)
        h->buf[(h->i)++] = '#';
    XClearWindow(lock.display, lock.win);
    XDrawString(lock.display, lock.win, lock.gc, 10, 10, h->buf, h->i);
    XFlush(lock.display);
    buf_delete(h);
}


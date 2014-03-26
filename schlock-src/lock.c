#include "lock.h"
#include "../common/xutils.h"

const int win_width = 400;
const int win_height = 100;

t_lock win_create(char *msg)
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

    XColor color;
    Colormap colormap;
    char green[] = "#9FB6CD";

    colormap = DefaultColormap(lock.display, 0);
    XParseColor(lock.display, colormap, green, &color);
    XAllocColor(lock.display, colormap, &color);

    lock.win = XCreateSimpleWindow(lock.display, root_win,
            (screen_width - win_width) / 2,
            (screen_height - win_height) / 2,
            win_width, win_height, 0, black, black);
    XSelectInput(lock.display, lock.win, StructureNotifyMask);
    XMapWindow(lock.display, lock.win);
    XStoreName(lock.display, lock.win, "schlock");
    lock.gc = XCreateGC(lock.display, lock.win, 0, NULL);
    //Font font = XLoadFont(lock.display, "10x14");
    //XSetFont(lock.display, lock.gc, font);
    XSetForeground(lock.display, lock.gc, white);
    while (evt.type != MapNotify)
        XNextEvent(lock.display, &evt);
    XFlush(lock.display);

    /* Grab input */
    XGrabPointer(lock.display, lock.win, 0, ButtonPressMask | ButtonReleaseMask
            | PointerMotionMask, GrabModeSync, GrabModeAsync, lock.win, None,
            CurrentTime);
    XGrabKeyboard(lock.display, lock.win, True, GrabModeAsync, GrabModeAsync, CurrentTime);

    lock.msg = msg;
    lock.msg_len = strlen(msg);
    gettimeofday(&lock.started, NULL);
    return lock;
}

char win_getc(t_lock lock, t_buf *buf)
{
    KeySym key;
    char c;
    XEvent evt;
    static int frame = 0;

    if (XCheckWindowEvent(lock.display, lock.win,
                StructureNotifyMask | ExposureMask | KeyPressMask, &evt)
            && evt.type == KeyPress) {
        XLookupString(&evt.xkey, &c, 1, &key, NULL);
        if (key == XK_Return)
            return 0;
        else if (IsFunctionKey(key)
                || IsKeypadKey(key)
                || IsMiscFunctionKey(key)
                || IsPFKey(key)
                || IsPrivateKeypadKey(key))
            return 1;
        else if (key == XK_BackSpace) {
            if (buf->i)
                buf->buf[--(buf->i)] = 0;
        }
        else if (buf->i < buf->max - 1) {
            buf->buf[buf->i] = c;
            buf->buf[++(buf->i)] = 0;
        }
        return 1;
    }
    usleep(1000 * 10);
    if (++frame == 100) {
        frame = 0;
        return 1;
    }
    return 2;
}

static char rand_char()
{
    return rand() % 92 + 33;
}

void win_str(t_lock lock, t_buf *buf)
{
    static t_buf *h = NULL;
    char timer[255];
    struct timeval current;
    struct timeval res;
    gettimeofday(&current, NULL);
    timersub(&current, &lock.started, &res);

    snprintf(timer, 255, "      Started %.2dh%.2dm%.2ds ago",
            res.tv_sec / 3600,
            (res.tv_sec % 3600) / 60,
            (res.tv_sec % 60));
    size_t len = strlen(timer);

    if (h == NULL)
        h = buf_create(buf->max);

    if (buf->i == 0)
        h->i = 0;
    if (buf->i - h->i == 1)
        h->buf[h->i] = '#'; //rand_char();
    h->i = buf->i;
    XClearWindow(lock.display, lock.win);
    XDrawString(lock.display, lock.win, lock.gc, 10, 20, lock.msg, lock.msg_len);
    XDrawString(lock.display, lock.win, lock.gc, 10, 40, timer, len);
    XDrawString(lock.display, lock.win, lock.gc, 10, 60, "Password:", 9);
    XDrawString(lock.display, lock.win, lock.gc, 100, 60, h->buf, h->i);
    XFlush(lock.display);
}


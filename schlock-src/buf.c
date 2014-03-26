#include "buf.h"

t_buf *buf_create(size_t max)
{
    t_buf *buf = malloc(sizeof (t_buf));
    buf->i = 0;
    buf->max = max;
    buf->buf = malloc(sizeof (char) * max);
    buf->buf[0] = 0;
    return buf;
}

void buf_delete(t_buf *buf)
{
    free(buf->buf);
    free(buf);
}

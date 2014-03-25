#ifndef BUF_H
# define BUF_H

# include <stdlib.h>

struct s_buf {
    size_t max;
    size_t i;
    char *buf;
};
struct s_buf typedef t_buf;

t_buf *buf_create(size_t max);
void buf_delete(t_buf *buf);

#endif /* !BUF_H */

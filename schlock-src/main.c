#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "cred.h"
#include "buf.h"
#include "lock.h"

t_buf *buf = NULL;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    if (cred_init())
        return EXIT_FAILURE;
    t_lock *lock = win_create(NULL);
    buf = buf_create(255);
    int ret = 1;

    while (1) {
        do {
            if (ret == 1)
                win_str(lock, buf);
        } while ((ret = win_getc(lock, buf)));
        if (!cred_check(buf->buf, buf->i))
            break;
        win_fail(lock);
        buf->i = 0;
    }

    buf_delete(buf);
    return EXIT_SUCCESS;
}

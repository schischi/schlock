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
    t_lock lock = win_create("This screen is locked by schischi");
    buf = buf_create(255);
    while (win_getc(lock, buf)) {
        win_str(lock, buf);
        continue;
    }
    if (cred_check(buf->buf, buf->i)) {
        printf("Nop\n");
    }

    buf_delete(buf);
    return EXIT_SUCCESS;
}

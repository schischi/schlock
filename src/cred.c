#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <sys/types.h>
#include <pwd.h>
#include <security/pam_appl.h>

#include "cred.h"
#include "buf.h"

static pam_handle_t *pamh;
extern t_buf *buf;

static int misc_conv(int num_msg, const struct pam_message **msg,
        struct pam_response **resp, void *appdata_ptr)
{
    if (num_msg == 0)
        return 1;

    *resp = calloc(num_msg, sizeof (struct pam_response));
    for (int i = 0; i < num_msg; ++i) {
        if (msg[i]->msg_style != PAM_PROMPT_ECHO_OFF
                && msg[i]->msg_style != PAM_PROMPT_ECHO_ON)
            continue;
        resp[i]->resp_retcode = 0;
        resp[i]->resp = strndup(buf->buf, buf->max);
    }
    return 0;
}

int cred_init()
{
    int c;
    struct passwd *pw;

    pw = getpwuid(geteuid());
    struct pam_conv conv = { misc_conv, NULL };
    int err;
    return pam_start("schischi_lock", pw->pw_name, &conv, &pamh) != PAM_SUCCESS;
}

int cred_check()
{
    return pam_authenticate(pamh, 0) != PAM_SUCCESS;
}

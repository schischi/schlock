#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <libconfig.h>

#include "config.h"

t_config *config_load(int argc, char *argv[])
{
    t_config *conf = malloc(sizeof (t_config));
    config_t cfg;
    config_setting_t *setting;
    const char *svg;
    char *shape = "hmm";

    int opt;
    while ((opt = getopt(argc, argv, "s:")) != -1) {
        switch (opt) {
            case 's':
                shape = optarg;
                break;
            default:
                return NULL;
        }
    }

    config_init(&cfg);
    if (!config_read_file(&cfg, "res/config")) {
        return NULL;
    }

    setting = config_lookup(&cfg, shape);
    if (!setting)
        setting = config_lookup(&cfg, "hmm");
    if (!config_setting_lookup_string(setting, "svg", &svg))
        return NULL;
    if (!config_setting_lookup_int(setting, "x", &conf->x))
        return NULL;
    if (!config_setting_lookup_int(setting, "y", &conf->y))
        return NULL;
    conf->svg = strdup(svg);
    config_destroy(&cfg);
#if 1
    printf("svg: %s (%d - %d)\n", conf->svg, conf->x, conf->y);
#endif

    return conf;
}

void config_delete(t_config *conf)
{
    free(conf->svg);
    free(conf);
}

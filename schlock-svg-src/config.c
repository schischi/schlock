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
    char *path = "/usr/share/schlock/";

    int opt;
    while ((opt = getopt(argc, argv, "s:")) != -1) {
        switch (opt) {
            case 's':
                shape = optarg;
                break;
            default:
                ; //return NULL;
        }
    }

    config_init(&cfg);
    if (!config_read_file(&cfg, "/usr/share/schlock/config")) {
        if (!config_read_file(&cfg, "config"))
            if (!config_read_file(&cfg, "res/config"))
                return NULL;
        path = "";
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
    asprintf(&(conf->svg), "%s%s", path, svg);
    config_destroy(&cfg);
    printf("Svg is %s\n", conf->svg);
    return conf;
}

void config_delete(t_config *conf)
{
    free(conf->svg);
    free(conf);
}

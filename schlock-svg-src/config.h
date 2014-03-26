#ifndef CONFIG_H
# define CONFIG_H

struct s_config {
    char *svg;
    int x;
    int y;
    int random;
};
struct s_config typedef t_config;

t_config *config_load(int argc, char *argv[]);
void config_delete(t_config *conf);

#endif /* !CONFIG_H */

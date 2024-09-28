#ifndef FT_LS_H
#define FT_LS_H

#ifndef DEBUG
# define DEBUG 0
#endif

#ifndef EXA
# define EXA 0
#endif

#ifdef __APPLE__
    #define MAX_PATH_LENGTH __DARWIN_MAXPATHLEN
#else
    #define MAX_PATH_LENGTH 1024
#endif


#include <stdbool.h>
#include "libft.h"
#include <dirent.h> // opendir readdir closedir
#include <sys/stat.h> // stat lstat
#include <pwd.h> // getpwuid
#include <grp.h> // getgrgid
#include <time.h> // ctime time
#include <unistd.h> // readlink
#include <stdlib.h> // exit
#include <stdio.h> // perror
#include <string.h> // strerror
#include <sys/xattr.h>

#define OPTION_l 1 << 0
#define OPTION_R 1 << 1
#define OPTION_a 1 << 2
#define OPTION_r 1 << 3
#define OPTION_t 1 << 4
#define OPTION_G 1 << 5
#define OPTION_L 1 << 6

#define IS_LONG(option) (option & OPTION_l)
#define IS_RECURSIVE(option) (option & OPTION_R)
#define IS_ALL(option) (option & OPTION_a)
#define IS_REVERSE(option) (option & OPTION_r)
#define IS_TIME_SORT(option) (option & OPTION_t)
#define IS_COLOR(option) (option & OPTION_G)
#define IS_LINK(option) (option & OPTION_L)

#define COLOR_RED		"\033[0;31m"
#define COLOR_GREY		"\033[0;37m"
#define COLOR_GREEN		"\033[0;32m"
#define COLOR_BLUE		"\033[0;34m"
#define COLOR_YELLOW	"\033[0;33m"
#define COLOR_RESET		"\033[0m"

#define ERROR_MSG_LEN 256
#define MAX_LINE_LEN 1024

typedef int32_t t_options;

typedef struct s_ls_ctrl {
	t_options		options;
	t_list 			*paths_args;
} t_ls_ctrl;

typedef struct s_dir_data {
	char			*name;
	char			perms[128];
	char			*owner;
	char			*group;
	int64_t			size;
	time_t			time;
	int 			nlink;
	int 			block_size;
	mode_t			st_mode;
	char			*path;
	bool			has_extattr;
} t_dir_data;


// ft_print.c
int	ft_ls(int argc, char *argv[]);
int	ft_ls_fd(int fd, int argc, char *argv[]);

// ft_directory.c
t_dir_data	*new_dir_data(struct dirent *entry, char *path);
void		clear_dir_data(t_dir_data *data);

// ft_print.c
void print_dir(int fd, char* path, t_list *dir_data, t_ls_ctrl *ctrl);
void print_headers(int fd);

// ft_format.c
void format_size(char *dest, int size);
void format_time(char *dest, char *time_str);
void format_perms(char *dest, char *perms);
void format_name(char *dest, t_dir_data *dir_data, t_options options);

// ft_args.c
t_ls_ctrl	parse_args(int argc, char *argv[]);
void		clear_args(t_ls_ctrl *ctrl);
void		print_args(const t_ls_ctrl ctrl);

// ft_comparators.c
int	dir_data_name_cmp(const void *a, const void *b);
int	dir_data_name_cmp_reverse(const void *a, const void *b);
int	dir_data_time_cmp(const void *a, const void *b);
int	dir_data_time_cmp_reverse(const void *a, const void *b);
int	comp_path(const char *queue_path_to_compare, const char *path_to_find);

// ft_utils.c
// char		get_dir_type(struct stat statbuf);
void		set_permissions(char *perms, mode_t mode);
const char* get_dirent_type_string(unsigned char d_type);
void		ft_error(const char *msg, const int exiting);

#endif

#include "ft_ls.h"
#include <errno.h>

void print_list(char dest[MAX_LINE_LEN], t_dir_data *dir_data, t_options options) {
	char			*time = ctime(&dir_data->time);
	char 			formatted_permissions[256] = {0},
					formatted_size[32] = {0},
					formatted_time[32] = {0},
					formatted_name[MAX_PATH_LENGTH] = {0};
	int				colored = options & OPTION_G;

	time[ft_strlen(time) - 1] = '\0';
	format_size(formatted_size, dir_data->size);
	format_time(formatted_time, time);
	if (colored)
		format_perms(formatted_permissions, dir_data->perms);
	ft_fprintf(
		dest,
		"%s %s%s %s%2d %s%s %s %s%s %s%s\n",
		colored ? formatted_permissions : dir_data->perms,
		colored ? COLOR_GREEN : "",
		formatted_size,
		colored ? COLOR_BLUE : "",
		dir_data->nlink,
		colored ? COLOR_YELLOW : "",
		dir_data->owner,
		dir_data->group,
		colored ? COLOR_BLUE : "",
		formatted_time,
		dir_data->name,
		colored ? COLOR_RESET : ""
	);
}

void print_simple(char dest[MAX_LINE_LEN], t_dir_data *dir_data) {
	ft_fprintf(dest, "%s\t", dir_data->name);
}

void print_dir(int fd, t_dir_data *dir_data, t_options options) {
	char buffer[MAX_LINE_LEN] = {0};
	char tmp[MAX_LINE_LEN] = {0};

	if (options & OPTION_L)
		print_list(buffer, dir_data, options);
	else
		print_simple(buffer, dir_data);

	ft_putstr_fd(buffer, fd);
}
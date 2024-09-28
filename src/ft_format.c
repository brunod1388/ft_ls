#include "ft_ls.h"

void format_size(char *dest, int size) {
#if EXA

	char	unit;
	int		result;
	int 	decimal;
	int		divider;

	if (size < 1024) {
		ft_fprintf(dest, "%4d", size);
		return;
	}
	if (size < 1024 * 1024) {
		divider = 1024;
		unit = 'k';
	} else if (size > 1024 * 1024 && size < 1024 * 1024 * 1024) {
		divider = 1024 * 1024;
		unit = 'm';
	} else if (size > 1024 * 1024 * 1024) {
		divider = 1024 * 1024 * 1024;
		unit = 'g';
	}

	result = (int) size / divider;
	decimal = (int) (((float)size / divider - (float) result) * 10);

	if (result < 10 && decimal > 0)
		ft_fprintf(dest, "%d,%d%c", result, decimal, unit);
	else
		ft_fprintf(dest, "%3d%c", (int) result, unit);
#else
	ft_fprintf(dest, "%d", size);
#endif
}

void format_time(char *dest, char *time_str) {
	char **split_time = ft_split(time_str, ' ');
	if (!split_time) {
		ft_error("malloc in format_time", 1);
	}
	split_time[3][5] = '\0';
#if EXA
	ft_fprintf(
		dest,
		"%s %s %s %s",
		split_time[2],
		split_time[1],
		split_time[4],
		split_time[3]
	);
#else
	ft_fprintf(
		dest,
		"%s %s %s",
		split_time[1],
		split_time[2],
		split_time[3]
	);
#endif
	ft_clearsplit(split_time);
}

void format_perms(char *dest, char *perms) {

	if (*perms == 'd')
		ft_strcat(dest, COLOR_BLUE);
	ft_strncat(dest, perms, 1);

	while (*++perms) {
		switch (*perms) {
		case 'r':
			ft_strcat(dest, COLOR_YELLOW);
			ft_strncat(dest, perms, 1);
			break;
		case 'w':
			ft_strcat(dest, COLOR_RED);
			ft_strncat(dest, perms, 1);
			break;
		case 'x':
			ft_strcat(dest, COLOR_GREEN);
			ft_strncat(dest, perms, 1);
			break;
		default:
			ft_strcat(dest, COLOR_GREY);
			ft_strncat(dest, perms, 1);
		}
	}
	ft_strcat(dest, COLOR_RESET);
}

void format_name(char *dest, t_dir_data *dir_data, t_options options) {
	mode_t st_mode = dir_data->st_mode;
	if (!IS_COLOR(options))
		ft_strcpy(dest, dir_data->name);
	else if (S_ISDIR(st_mode))
		ft_fprintf(dest, "%s%s%s", COLOR_BLUE, dir_data->name, COLOR_RESET);
	else if (S_ISLNK(st_mode))
		ft_fprintf(dest, "%s%s%s", COLOR_YELLOW, dir_data->name, COLOR_RESET);
	else
		ft_strcpy(dest, dir_data->name);
}
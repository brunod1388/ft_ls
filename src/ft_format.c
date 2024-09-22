#include "ft_ls.h"

void format_size(char *dest, int size) {
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
}

void format_time(char *dest, char *time_str) {
	char **split_time = ft_split(time_str, ' ');
	if (!split_time) {
		ft_error("malloc in format_time", 1);
	}
	split_time[3][5] = '\0';
	ft_fprintf(
		dest,
		"%s %s %s %s",
		split_time[2],
		split_time[1],
		split_time[4],
		split_time[3]
	);
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

void format_name(char *dest, char *name) {
	if (name[0] == '.') {
		ft_strcat(dest, COLOR_GREY);
	}
	ft_strcat(dest, name);
	ft_strcat(dest, COLOR_RESET);
}
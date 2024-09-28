#include "ft_ls.h"
#include <errno.h>

void print_long(char dest[MAX_LINE_LEN], t_dir_data *dir_data, t_options options) {
	char	*time = ctime(&dir_data->time);
	char 	formatted_permissions[256] = {0},
			formatted_size[32] = {0},
			formatted_time[32] = {0},
			formatted_name[MAX_PATH_LENGTH] = {0};
	int		colored = options & OPTION_G;

	time[ft_strlen(time) - 1] = '\0';
	format_size(formatted_size, dir_data->size);
	format_time(formatted_time, time);
	format_name(formatted_name, dir_data, options);
	if (colored) {
		format_perms(formatted_permissions, dir_data->perms);
	}

	int offset = 0;

#if EXA
	offset += ft_fprintf(dest + offset, "%s", formatted_permissions);
	if (S_ISDIR(dir_data->st_mode))
		offset += ft_fprintf(dest + offset, "%s     -", COLOR_GREY);
	else
		offset += ft_fprintf(dest + offset, "%s%6s", COLOR_GREEN, formatted_size);
	offset += ft_fprintf(dest + offset, " %s%s", COLOR_YELLOW, dir_data->owner);
	offset += ft_fprintf(dest + offset, " %s", dir_data->group);
	offset += ft_fprintf(dest + offset, " %s%s", COLOR_BLUE, formatted_time);
	offset += ft_fprintf(dest + offset, " %s%s\n", COLOR_RESET, formatted_name);
#else
	offset += ft_fprintf(dest + offset, "%s", colored ? formatted_permissions : dir_data->perms);
	offset += ft_fprintf(dest + offset, "%s", dir_data->has_extattr ? "@" : " ");
	offset += ft_fprintf(dest + offset, "%s%3d ", colored ? COLOR_BLUE : "", dir_data->nlink);
	offset += ft_fprintf(dest + offset, "%s%s ", colored ? COLOR_YELLOW : "", dir_data->owner);
	offset += ft_fprintf(dest + offset, " %s ", dir_data->group);
	offset += ft_fprintf(dest + offset, "%s%6s ", colored ? COLOR_BLUE : "", formatted_size);
	offset += ft_fprintf(dest + offset, "%s%s ", colored ? COLOR_BLUE : "", formatted_time);
	offset += ft_fprintf(dest + offset, "%s%s\n", COLOR_RESET, formatted_name);
#endif

#if DEBUG
	dest[ft_strlen(dest) - 1] = '\t';
	ft_strcat(dest, get_dirent_type_string(dir_data->st_mode));
	dest[ft_strlen(dest)] = '\n';
#endif
}

void print_simple(char dest[MAX_LINE_LEN], t_dir_data *dir_data) {
	char	formatted_name[MAX_PATH_LENGTH] = {0};

	format_name(formatted_name, dir_data, 0);
	ft_fprintf(dest, "%s\t", formatted_name);
}

void print_entry(int fd, t_dir_data *entry, t_options options) {
	char dest[MAX_LINE_LEN] = {0};
	if (options & OPTION_l) {
		print_long(dest, entry, options);
	} else {
		print_simple(dest, entry);
	}
	ft_putstr_fd(dest, fd);
}

void print_headers(int fd) {
	ft_printf_fd(fd, "%c[4mPermissions%c[0m",27,27);
	ft_printf_fd(fd, " %c[4mSize%c[0m",27,27);
	ft_printf_fd(fd, " %c[4mUser%c[0m",27,27);
	ft_printf_fd(fd, "      %c[4mGroup%c[0m",27,27);
	ft_printf_fd(fd, " %c[4mDate Modified%c[0m",27,27);
	ft_printf_fd(fd, "     %c[4mName\n%c[0m",27,27);
}

void print_total(int fd, t_list *entry_list) {
	int total = 0;

	while (entry_list) {
		total += ((t_dir_data*) entry_list->content)->block_size;
		entry_list = entry_list->next;
	}
	ft_printf_fd(fd, "total %d\n", total);
}

void print_dir(int fd, char* path, t_list *entry_list, t_ls_ctrl *ctrl) {
	t_options options = ctrl->options;

	if (ctrl->options & OPTION_R & ft_strcmp(path, "."))
		ft_printf_fd(fd, "%s:\n", path);

#if EXA
	if (options & OPTION_l)
		print_headers(fd);
#endif

	if (options & OPTION_l)
		print_total(fd, entry_list);

	do {
		t_dir_data *current_dir = (t_dir_data*) ft_lstpop_front(&entry_list);
		print_entry(fd, current_dir, options);
		clear_dir_data(current_dir);
	} while (entry_list);

	ft_putchar_fd('\n', fd);
	if (!(ctrl->options & OPTION_l))
		ft_putchar_fd('\n', fd);
}
#include "ft_ls.h"
#include <errno.h>

void print_list(char dest[MAX_LINE_LEN], t_dir_data *dir_data, t_options options) {
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
	if (EXA == 1 && dir_data->d_type != DT_DIR)
		offset += ft_fprintf(dest + offset, "%s%6s", COLOR_GREEN, formatted_size);
	else
		offset += ft_fprintf(dest + offset, "%s     -", COLOR_GREY);
	offset += ft_fprintf(dest + offset, " %s%s", COLOR_YELLOW, dir_data->owner);
	offset += ft_fprintf(dest + offset, " %s", dir_data->group);
	offset += ft_fprintf(dest + offset, " %s%s", COLOR_BLUE, formatted_time);
	offset += ft_fprintf(dest + offset, " %s%s\n", COLOR_RESET, formatted_name);
#else
	offset += ft_fprintf(dest + offset, "%s", colored ? formatted_permissions : dir_data->perms);
	offset += ft_fprintf(dest + offset, "%s%4d ", colored ? COLOR_BLUE : "", dir_data->nlink);
	offset += ft_fprintf(dest + offset, "%s%s ", colored ? COLOR_YELLOW : "", dir_data->owner);
	offset += ft_fprintf(dest + offset, " %s ", dir_data->group);
	offset += ft_fprintf(dest + offset, "%s%6s ", colored ? COLOR_BLUE : "", formatted_size);
	offset += ft_fprintf(dest + offset, "%s%s ", colored ? COLOR_BLUE : "", formatted_time);
	offset += ft_fprintf(dest + offset, "%s%s\n", colored ? COLOR_RESET : "", formatted_name);
#endif

#if DEBUG
	dest[ft_strlen(dest) - 1] = '\t';
	ft_strcat(dest, get_dirent_type_string(dir_data->d_type));
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
	if (options & OPTION_L) {
		print_list(dest, entry, options);
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

void print_total(int fd, t_list *dir_data_list) {
	int total = 0;

	while (dir_data_list) {
		total += ((t_dir_data*) dir_data_list->content)->block_size;
		dir_data_list = dir_data_list->next;
	}
	ft_printf_fd(fd, "total %d\n", total);
}

void print_dir(int fd, char* path, t_list *dir_data_list, t_ls_ctrl *ctrl, t_list **path_queue) {
	t_options options = ctrl->options;

	if (ctrl->options & OPTION_R)
		ft_printf_fd(fd, "%s:\n", path);

#if EXA
	if (options & OPTION_L)
		print_headers(fd);
#endif

	if (options & OPTION_L)
		print_total(fd, dir_data_list);
	do {
		t_dir_data *current_dir = (t_dir_data*) ft_lstpop_front(&dir_data_list);
		print_entry(fd, current_dir, options);
		if (options & OPTION_R && current_dir->d_type == DT_DIR && ft_strcmp(current_dir->name, ".") && ft_strcmp(current_dir->name, "..")) {
			char fullpath[MAX_PATH_LENGTH] = {0};
			ft_fprintf(fullpath, "%s/%s", path, current_dir->name);
			ft_lstadd_back(path_queue, ft_lstnew(ft_strdup(fullpath)));
		}
		clear_dir_data(current_dir);
	} while (dir_data_list);
	ft_putchar_fd('\n', fd);
	if (!(ctrl->options & OPTION_L))
		ft_putchar_fd('\n', fd);
}
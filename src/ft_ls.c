#include "ft_ls.h"

t_list *process_dir(DIR *dir, char *path, t_options options) {
	struct dirent *entry;
	t_list *dir_data = NULL;

	while ((entry = readdir(dir))) {
		if (entry->d_name[0] == '.' && !(options & OPTION_a))
			continue;
		struct s_dir_data *data = new_dir_data(entry, path);
		if (!data) {
			ft_lstclear(&dir_data,(void (*)(void *)) clear_dir_data);
			ft_error("malloc in process_dir", 0);
			return NULL;
		}
		ft_lstadd_back(&dir_data, ft_lstnew(data));
	}

	return dir_data;
}

void apply_sort_options(t_list *dir_data, t_options options) {
	if (options & OPTION_t && !(options & OPTION_r)) {
		ft_lstsort(&dir_data, (int (*)(void *, void *)) dir_data_time_cmp);
	}
	else if (options & OPTION_t && options & OPTION_r) {
		ft_lstsort(&dir_data, (int (*)(void *, void *)) dir_data_time_cmp_reverse);
	}
	else if (options & OPTION_r) {
		ft_lstsort(&dir_data, (int (*)(void *, void *)) dir_data_name_cmp_reverse);
	}
	else {
		ft_lstsort(&dir_data, (int (*)(void *, void *)) dir_data_name_cmp);
	}
}

int process_path(int fd, char *path, t_ls_ctrl *ctrl) {
	DIR *dir;
	t_list *dir_data;

	if (path == NULL) {
		ft_error("NULL path", 0);
		return 1;
	}
	dir = opendir(path);
	if (!dir) {
		ft_error(path, 0);
		return 1;
	}
	dir_data = process_dir(dir, path, ctrl->options);
	closedir(dir);
	if (!dir_data) {
		return 1;
	}

	apply_sort_options(dir_data, ctrl->options);

	print_dir(fd, dir_data, ctrl->options);

	return 0;
}

int ft_ls_fd(int fd, int argc, char *argv[]) {
	t_ls_ctrl	ctrl = parse_args(argc, argv);
	t_list		*current = ctrl.paths;
	int			show_header = ft_lstsize(ctrl.paths) > 1;

#if DEBUG
	print_args(ctrl);
#endif

	while (current) {
		if (show_header) {
			ft_printf_fd(fd, "%s:\n", (char *)current->content);
		}

		int ret = process_path(fd, (char *)current->content, &ctrl);

		if ( ret != 0) {
			ft_lstclear(&ctrl.paths, NULL);
			return ret;
		}
		current = current->next;
		if (current)
			ft_putchar_fd('\n', fd);
	}
	clear_args(&ctrl);
	return 0;
}

int ft_ls(int argc, char *argv[]) {
	return ft_ls_fd(STDOUT_FILENO, argc, argv);
}
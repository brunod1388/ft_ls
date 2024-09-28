#include "ft_ls.h"

t_list *process_dir(DIR *dir, char *path, t_options options) {
	struct dirent *entry;
	t_list *dir_data_list = NULL;

	while ((entry = readdir(dir))) {
		if (entry->d_name[0] == '.' && !(options & OPTION_a))
			continue;
		struct s_dir_data *data = new_dir_data(entry, path);
		if (!data) {
			ft_lstclear(&dir_data_list,(void (*)(void *)) clear_dir_data);
			ft_error("malloc in process_dir", 0);
			return NULL;
		}
		ft_lstadd_back(&dir_data_list, ft_lstnew(data));
	}

	return dir_data_list;
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

int comp_path(const char *queue_path_to_compare, const char *path_to_find) {
	char path_buffer[MAX_PATH_LENGTH] = {0};
	ft_strcat(path_buffer, path_to_find);
	ft_strcat(path_buffer, "/");
	return ft_strncmp(path_buffer, queue_path_to_compare, ft_strlen(path_buffer));
}

int process_path(int fd, char *path, t_ls_ctrl *ctrl, t_list *path_queue) {
	DIR *dir;
	t_list *dir_data_list;

	if (path == NULL) {
		ft_error("NULL path", 0);
		return 1;
	}
	dir = opendir(path);
	if (!dir) {
		ft_error(path, 0);
		return 1;
	}
	dir_data_list = process_dir(dir, path, ctrl->options);
	closedir(dir);
	if (!dir_data_list) {
		return 1;
	}

	apply_sort_options(dir_data_list, ctrl->options);

	print_dir(fd, path, dir_data_list, ctrl, &path_queue);

	if (ctrl->options & OPTION_R && path_queue) {
		char *next_path = (char*) ft_lstfind(&path_queue, path, (int (*)(void *, void *)) comp_path);

		if (!next_path)
			next_path = ft_lstpop_front(&path_queue);
		// char *next_path = ft_lstpop_front(&path_queue);

		if (next_path)
			process_path(fd, next_path, ctrl, path_queue);

		free(next_path);
	}
	return 0;
}

int ft_ls_fd(int fd, int argc, char *argv[]) {
	t_ls_ctrl	ctrl = parse_args(argc, argv);
	t_list		*path_arg_list = ctrl.paths_args;

#if DEBUG
	print_args(ctrl);
#endif

	while (path_arg_list) {
		int ret = process_path(fd, (char *)path_arg_list->content, &ctrl, NULL);

		if ( ret != 0) {
			ft_lstclear(&ctrl.paths_args, NULL);
			return ret;
		}
		path_arg_list = path_arg_list->next;
		if (path_arg_list)
			ft_putchar_fd('\n', fd);
	}
	clear_args(&ctrl);
	return 0;
}

int ft_ls(int argc, char *argv[]) {
	return ft_ls_fd(STDOUT_FILENO, argc, argv);
}
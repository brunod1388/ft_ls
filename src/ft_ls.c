#include "ft_ls.h"

#if DEBUG
#include "ft_debug.h"
#endif

int is_symlink(const char *path) {
    struct stat path_stat;
    if (lstat(path, &path_stat) != 0) {
        perror("lstat");
        return -1; // Error occurred
    }
    return S_ISLNK(path_stat.st_mode);
}

void apply_sort_options(t_list *dir_data_list, t_options options) {
	if (IS_TIME_SORT(options) && !IS_REVERSE(options)) {
		ft_lstsort(&dir_data_list, (int (*)(void *, void *)) dir_data_time_cmp);
	}
	else if (IS_TIME_SORT(OPTION_t) && IS_REVERSE(options)) {
		ft_lstsort(&dir_data_list, (int (*)(void *, void *)) dir_data_time_cmp_reverse);
	}
	else if (IS_REVERSE(options)) {
		ft_lstsort(&dir_data_list, (int (*)(void *, void *)) dir_data_name_cmp_reverse);
	}
	else {
		ft_lstsort(&dir_data_list, (int (*)(void *, void *)) dir_data_name_cmp);
	}
}

t_dir_data_list *process_dir(DIR *dir, char *path, t_options options, t_list **path_queue) {
	struct dirent *entry;
	t_list *dir_data_list = NULL;

	while ((entry = readdir(dir))) {
		if (entry->d_name[0] == '.' && !IS_ALL(options))
			continue;
		t_dir_data *data = new_dir_data(entry->d_name, path);
		if (!data) {
			ft_lstclear(&dir_data_list,(void (*)(void *)) clear_dir_data);
			ft_error("malloc in process_dir", 0);
			return NULL;
		}

		if (options & OPTION_R && S_ISDIR(data->st_mode) && ft_strcmp(data->name, ".") && ft_strcmp(data->name, "..")) {
			char fullpath[MAX_PATH_LENGTH] = {0};
			ft_fprintf(fullpath, "%s/%s", path, data->name);
			printf("test %s\n", fullpath);
			ft_lstadd_back(path_queue, ft_lstnew(ft_strdup(fullpath)));
		}
		ft_lstadd_back(&dir_data_list, ft_lstnew(data));
	}

	return dir_data_list;
}

int handle_symlink(int fd, char *path, t_options options) {
	t_dir_data *data = new_dir_data(path, NULL);
	char buffer[MAX_LINE_LEN] = {0};
	char link_path[MAX_PATH_LENGTH] = {0};

	if (!data) {
		ft_error("malloc in handle_symlink", 0);
		return 1;
	}
	print_long(buffer, data, options);
	buffer[ft_strlen(buffer) - 1] = '\0';
	readlink(path, link_path, MAX_PATH_LENGTH);
	// ft_printf("%s -> %s\n", buffer, link_path);
	ft_printf_fd(fd, "%s -> %s\n", buffer, link_path);

	clear_dir_data(data);
	return 0;
}

int process_path(int fd, char *path, t_ls_ctrl *ctrl, t_list *path_queue) {
	DIR *dir;
	t_dir_data_list *entry_list;

	if (path == NULL) {
		ft_error("NULL path", 0);
		return 1;
	}

	if (is_symlink(path) && IS_LONG(ctrl->options) && !IS_LINK(ctrl->options))
		return handle_symlink(fd, path, ctrl->options);

	dir = opendir(path);
	if (!dir) {
		ft_error(path, 0);
		return 1;
	}
	entry_list = process_dir(dir, path, ctrl->options, &path_queue);
#if DEBUG
	print_DIR(dir);
	closedir(dir);
	return 0;
#endif
	closedir(dir);
	if (!entry_list) {
		return 1;
	}

	apply_sort_options(entry_list, ctrl->options);

	print_dir(fd, path, entry_list, ctrl);

	if (IS_RECURSIVE(ctrl->options) && path_queue) {
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
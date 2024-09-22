#include "ft_ls.h"

int dirent_cmp(void *a, void *b) {
	return ft_strcmp_ci(((struct dirent *)a)->d_name, ((struct dirent *)b)->d_name);
}

int display_dir(int fd, char *path, t_ls_ctrl *ctrl) {
	if (path == NULL) {
		ft_error("NULL path", 0);
		return 1;
	}
	ctrl->current_dir = opendir(path);
	if (!ctrl->current_dir) {
		ft_error(path, 0);
		return 1;
	}
	struct dirent *entry;

	while ((entry = readdir(ctrl->current_dir))) {
		ft_lstadd_back(&ctrl->dirents, ft_lstnew(entry));
	}
	ft_lstsort(&ctrl->dirents, dirent_cmp);

	do {
		entry = ft_lstpop_front(&ctrl->dirents);
		// ft_printf_fd(fd, "%s type: %s\n", entry->d_name, get_dirent_type_string(entry->d_type));
		print_dir(fd, path, entry, ctrl->options);
	} while (ctrl->dirents);

	closedir(ctrl->current_dir);
	ctrl->current_dir = NULL;
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

		int ret = display_dir(fd, (char *)current->content, &ctrl);

		if ( ret != 0) {
			ft_lstclear(&ctrl.paths, NULL);
			return ret;
		}
		current = current->next;
		if (current)
			ft_putchar_fd('\n', fd);
	}
	return 0;
}

int ft_ls(int argc, char *argv[]) {
	return ft_ls_fd(STDOUT_FILENO, argc, argv);
}
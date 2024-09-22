# include "ft_ls.h"

static const char *G_DEFAULT_PATH = ".";

void add_path(t_ls_ctrl *ctrl, const char *path) {
	t_list *current_path = ft_lstnew((void *)path);

	if (!current_path) {
		clear_args(ctrl);
		ft_error("malloc in path", 1);
	}
	ft_lstadd_back(&ctrl->paths, current_path);
}

void add_option(t_ls_ctrl *ctrl, char *option_arg) {
	char error_msg[256] = {0};
	while (*(++option_arg)) {
		char option = *option_arg;

		switch (option) {
			case 'l':
				ctrl->options |= OPTION_L;
				break;
			case 'R':
				ctrl->options |= OPTION_R;
				break;
			case 'a':
				ctrl->options |= OPTION_a;
				break;
			case 'r':
				ctrl->options |= OPTION_r;
				break;
			case 't':
				ctrl->options |= OPTION_t;
				break;
			default:
				clear_args(ctrl);
				ft_fprintf(error_msg, "illegal option -- %c", option);
				ft_fprintf(error_msg, "%s\nusage: ft_ls [-lRart] [file ...]", error_msg);
				ft_error(error_msg, 1);
		}
	}
}

t_ls_ctrl parse_args(int argc, char *argv[]) {
	t_ls_ctrl ctrl = {.options = 0, .paths = NULL, .current_dir = NULL, .dirents = NULL};

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-')
			add_option(&ctrl, argv[i]);
		else
			add_path(&ctrl, argv[i]);
	}
	if (!ctrl.paths)
		add_path(&ctrl, G_DEFAULT_PATH);

	return ctrl;
}


void clear_args(t_ls_ctrl *ctrl) {
	if (ctrl->paths)
		ft_lstclear(&ctrl->paths, NULL);
	if (ctrl->dirents)
		ft_lstclear(&ctrl->dirents, NULL);
	if (ctrl->current_dir)
		closedir(ctrl->current_dir);
}

void print_args(const t_ls_ctrl ctrl) {

	char optionStr[32] = {0};
	ft_printf("---Arguments---\n");
	ft_printf("Options: %b", ctrl.options);
	for (int i = 0, j = 0; i < 6; i++) {
		if (ctrl.options & (1 << i)) 
			optionStr[j++] =  "lRart"[i];
	}
	if (!ctrl.options)
		ft_strcpy(optionStr, "None");
	ft_printf("%s\n", optionStr);
	ft_printf("Paths:\n");
	t_list *current = ctrl.paths;
	while (current) {
		ft_printf("\t%s\n", (char *)current->content);
		current = current->next;
	}
	ft_printf("---------------\n");
}
# include "ft_ls.h"

static const char *G_DEFAULT_PATH = ".";

void add_path(t_ls_ctrl *ctrl, const char *path) {
	t_list *current_path = ft_lstnew((void *)path);

	if (!current_path) {
		clear_args(ctrl);
		ft_error("malloc in path", 1);
	}
	ft_lstadd_back(&ctrl->paths_args, current_path);
}

void add_option(t_ls_ctrl *ctrl, char *option_arg) {
	char error_msg[256] = {0};
	while (*(++option_arg)) {
		char option = *option_arg;

		switch (option) {
			case 'l':
				ctrl->options |= OPTION_l;
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
			case 'G':
				ctrl->options |= OPTION_G;
				break;
			case 'L':
				ctrl->options |= OPTION_L;
				break;
			default:
				clear_args(ctrl);
				ft_fprintf(error_msg, "illegal option -- %c", option);
				ft_fprintf(error_msg, "%s\nusage: ft_ls [-lRart] [file ...]", error_msg);
				ft_error(error_msg, 1);
		}
	}
#if EXA
	ctrl->options |= OPTION_G;
#endif
}

t_ls_ctrl parse_args(int argc, char *argv[]) {
	t_ls_ctrl ctrl = {0};

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-')
			add_option(&ctrl, argv[i]);
		else
			add_path(&ctrl, argv[i]);
	}
	if (!ctrl.paths_args)
		add_path(&ctrl, G_DEFAULT_PATH);

	return ctrl;
}


void clear_args(t_ls_ctrl *ctrl) {
	if (ctrl->paths_args)
		ft_lstclear(&ctrl->paths_args, NULL);
}

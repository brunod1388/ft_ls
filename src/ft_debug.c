#include "ft_debug.h"
#include "ft_ls.h"

const char* get_dirent_type_string(unsigned char d_type) {
    switch (d_type) {
        case DT_UNKNOWN: return "Unknown";
        case DT_FIFO:    return "FIFO";
        case DT_CHR:     return "Character Device";
        case DT_DIR:     return "Directory";
        case DT_BLK:     return "Block Device";
        case DT_REG:     return "Regular File";
        case DT_LNK:     return "Symbolic Link";
        case DT_SOCK:    return "Socket";
        case DT_WHT:     return "Whiteout";
        default:         return "Invalid Type";
    }
}

void print_dirent(struct dirent *dirent) {
	if (!dirent) {
		printf("dirent is NULL\n");
		return;
	}
	printf("d_ino:    %llu\n", dirent->d_ino);
	printf("d_reclen: %hu\n", dirent->d_reclen);
	printf("d_type:   %hhu (%s)\n", dirent->d_type, get_dirent_type_string(dirent->d_type));
	printf("d_namlen: %hu\n", dirent->d_namlen);
	printf("d_name:   %s\n", dirent->d_name);
}

void print_stat(struct stat *statbuf) {
	if (!statbuf) {
		printf("statbuf is NULL\n");
		return;
	}
	printf("st_dev:     %d\n", statbuf->st_dev);
	printf("st_ino:     %llu\n", statbuf->st_ino);
	printf("st_mode:    %u\n", statbuf->st_mode);
	printf("st_nlink:   %u\n", statbuf->st_nlink);
	printf("st_uid:     %u\n", statbuf->st_uid);
	printf("st_gid:     %u\n", statbuf->st_gid);
	printf("st_rdev:    %d\n", statbuf->st_rdev);
	printf("st_size:    %lld\n", statbuf->st_size);
	printf("st_blksize: %u\n", statbuf->st_blksize);
	printf("st_blocks:  %lld\n", statbuf->st_blocks);
	printf("st_atime:   %ld\n", statbuf->st_atime);
	printf("st_mtime:   %ld\n", statbuf->st_mtime);
	printf("st_ctime:   %ld\n", statbuf->st_ctime);
}

void print_DIR(DIR *dir) {
	if (!dir) {
		printf("dir is NULL\n");
		return;
	}

	printf("dd_fd:    %d\n", dir->__dd_fd);
	printf("dd_loc:   %ld\n", dir->__dd_loc);
	printf("dd_size:  %ld\n", dir->__dd_size);
	printf("dd_buf:   %p\n", dir->__dd_buf);
	printf("dd_len:   %d\n", dir->__dd_len);
	printf("dd_seek:  %ld\n", dir->__dd_seek);
	printf("dd_flags: %d\n", dir->__dd_flags);
}

void print_args(const t_ls_ctrl ctrl) {

	char optionStr[32] = {0};
	ft_printf("---Arguments---\n");
	ft_printf("Options: %b", ctrl.options);
	for (int i = 0, j = 0; i < 6; i++) {
		if (ctrl.options & (1 << i)) 
			optionStr[j++] =  "lRartGL"[i];
	}
	if (!ctrl.options)
		ft_strcpy(optionStr, "None");
	ft_printf("%s\n", optionStr);
	ft_printf("Paths:\n");
	t_list *current = ctrl.paths_args;
	while (current) {
		ft_printf("\t%s\n", (char *)current->content);
		current = current->next;
	}
	ft_printf("---------------\n");
}

void print_options(t_options options) {
	printf("options: %d\n", options);
	printf("option long: %d\n", IS_LONG(options));
	printf("option recursive: %d\n", IS_RECURSIVE(options));
	printf("option reverse: %d\n", IS_REVERSE(options));
	printf("option time sort: %d\n", IS_TIME_SORT(options));
	printf("option all: %d\n", IS_ALL(options));
	printf("option color: %d\n", IS_COLOR(options));
	printf("option link: %d\n", IS_LINK(options));
}

void print_ctrl(t_ls_ctrl *ctrl) {
	if (!ctrl) {
		printf("ctrl is NULL\n");
		return;
	}
	print_options(ctrl->options);

	t_list *path_list = ctrl->paths_args;
	while (path_list) {
		printf("path: %s\n", (char *)path_list->content);
		path_list = path_list->next;
	}
}

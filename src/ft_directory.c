#include "ft_ls.h"

void parse_stat(char *path, t_dir_data *data) {
	struct stat statbuf;
	if (lstat(path, &statbuf) == -1) {
		ft_error("lstat in parse_stat", 0);
	}
	data->size = statbuf.st_size;
	data->time = statbuf.st_mtime;
	data->nlink = statbuf.st_nlink;
	data->block_size = statbuf.st_blocks;
	data->st_mode = statbuf.st_mode;
	data->owner = ft_strdup(getpwuid(statbuf.st_uid)->pw_name);
	data->group = ft_strdup(getgrgid(statbuf.st_gid)->gr_name);
	set_permissions(data->perms, data->st_mode);
	if (listxattr(path, NULL, 0, XATTR_NOFOLLOW))
		data->has_extattr = true;
	if (!data->owner || !data->group) {
		ft_error("malloc in parse_stat", 0);
	}
}

t_dir_data *new_dir_data(char* d_name, char *path) {
	t_dir_data *data = ft_calloc(1, sizeof(t_dir_data));
	if (!data) {
		ft_error("malloc in new_dir_data", 0);
		return NULL;
	}
	data->name = ft_strdup(d_name);
	if (!data->name) {
		clear_dir_data(data);
		ft_error("malloc in new_dir_data", 0);
		return NULL;
	}

	char full_path[MAX_PATH_LENGTH] = {0};
	if (path)
		ft_fprintf(full_path, "%s/", path);
	ft_strcat(full_path, d_name);

	parse_stat(full_path, data);

	if (listxattr(full_path, NULL, 0, XATTR_NOFOLLOW))
		data->has_extattr = true;
	if (!data->owner || !data->group) {
		clear_dir_data(data);
		ft_error("malloc in new_dir_data", 0);
		return NULL;
	}
	return data;
}

void clear_dir_data(t_dir_data *data) {
	if (!data)
		return;
	if (data->name)
		free(data->name);
	if (data->owner)
		free(data->owner);
	if (data->group)
		free(data->group);
	free(data);
}
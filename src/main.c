#include "ft_ls.h"

int main(int argc, char *argv[]) {
#if DEBUG
	ft_printf("==== DEBUG Mode ====\n");
#endif
	ft_ls(argc, argv);
	return 0;
}
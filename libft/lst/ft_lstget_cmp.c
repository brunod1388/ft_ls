#include "libft.h"

void*	ft_lstfind(t_list **lst, void *content, int (*cmp)(void *, void *)) {
	if (!lst || !*lst)
		return NULL;

	t_list	*current = *lst;
	t_list	*prev = NULL;

	while (current) {
		if (cmp(current->content, content) == 0) {
			void *result = current->content;
			if (prev)
				prev->next = current->next;
			else
				*lst = current->next;
			free(current);
			return result;
		}
		prev = current;
		current = current->next;
	}
	return NULL;
}

#include "libft.h"

void*	ft_lstpop_front(t_list **lst) {
	t_list	*tmp;
	void	*content;

	if (!lst || !*lst)
		return NULL;
	tmp = *lst;
	*lst = tmp->next;
	content = tmp->content;
	free(tmp);
	return content;
}
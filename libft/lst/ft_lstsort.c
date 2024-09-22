#include "libft.h"

void	ft_lstsort(t_list **lst, int (*cmp)(void *, void *)){
	t_list	*current;
	t_list	*next;
	void	*tmp;

	current = *lst;
	while (current)
	{
		next = current->next;
		while (next)
		{
			if (cmp(current->content, next->content) > 0)
			{
				tmp = current->content;
				current->content = next->content;
				next->content = tmp;
			}
			next = next->next;
		}
		current = current->next;
	}
}

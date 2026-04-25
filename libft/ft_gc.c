/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 02:02:42 by tdharmar          #+#    #+#             */
/*   Updated: 2025/11/20 02:05:08 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_list	**get_gc_list(void)
{
	static t_list	*gc_list = NULL;

	return (&gc_list);
}

static void	add_to_gc(void *ptr)
{
	t_list	*node;

	node = ft_lstnew(ptr);
	if (!node)
	{
		free(ptr);
		ft_gc_clear();
		exit(EXIT_FAILURE);
	}
	ft_lstadd_back(get_gc_list(), node);
}

void	*ft_gc_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		ft_gc_clear();
		exit(EXIT_FAILURE);
	}
	add_to_gc(ptr);
	return (ptr);
}

void	*ft_gc_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (size != 0 && count > ((size_t)-1) / size)
	{
		ft_gc_clear();
		exit(EXIT_FAILURE);
	}
	ptr = malloc(count * size);
	if (!ptr)
	{
		ft_gc_clear();
		exit(EXIT_FAILURE);
	}
	ft_bzero(ptr, count * size);
	add_to_gc(ptr);
	return (ptr);
}

void	ft_gc_clear(void)
{
	t_list	**gc_list;

	gc_list = get_gc_list();
	if (*gc_list)
	{
		ft_lstclear(gc_list, free);
		*gc_list = NULL;
	}
}

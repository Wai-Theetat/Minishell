/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 13:02:47 by tdharmar          #+#    #+#             */
/*   Updated: 2026/05/09 13:24:38 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*ft_token_new(t_token_type type, char *value, char quote)
{
	t_token	*token;

	token = ft_gc_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->quote = quote;
	token->fd = -1;
	return (token);
}

void	ft_token_add_back(t_token **lst, t_token *new)
{
	t_token	*last;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->next)
		last = last->next;
	last->next = new;
}

char	*ft_token_value_dup(const char *src)
{
	char	*dup;
	size_t	len;

	if (!src)
		return (NULL);
	len = ft_strlen(src);
	dup = ft_gc_malloc(len + 1);
	if (!dup)
		return (NULL);
	ft_strlcpy(dup, src, len + 1);
	return (dup);
}

int	ft_token_list_size(t_token *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

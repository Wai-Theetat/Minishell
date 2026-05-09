/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 13:41:16 by koonchevych       #+#    #+#             */
/*   Updated: 2026/05/09 13:51:54 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char **ft_env_to_array(t_env *env)
// {
//     char **arr;
//     size_t count;

// }

//test env_list

void ft_env_list(t_env **envp)
{
    if(!envp)
        return;
    
    t_env *current_node;
    current_node = *envp;
    while(current_node->next != NULL)
    {
        printf("%s:%s ->", current_node->key,current_node->value);
        current_node = current_node->next;
    }
    return;
}
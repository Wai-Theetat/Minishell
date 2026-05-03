/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 12:49:55 by tdharmar          #+#    #+#             */
/*   Updated: 2026/05/03 12:50:45 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

typedef enum e_token_type
{
	TOKEN_WORD,						//Regular txt/arg
	TOKEN_PIPE,						// |
	TOKEN_REDIRECT_IN,				// <
	TOKEN_REDIRECT_OUT,				// >
	TOKEN_REDIRECT_APPEND,			// >>
	TOKEN_HEREDOC,					// <<
	TOKEN_EOF						// End input
}	t_token_type;

#endif
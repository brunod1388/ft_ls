/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 17:33:36 by bgoncalv          #+#    #+#             */
/*   Updated: 2021/12/05 01:04:42 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_fprintf_format(char *dest, char *format, t_fdata *fdata)
{
	int	count;

	count = 0;
	while (*format)
	{
		if (*format == '%')
		{
			format = ft_eval_flags(format, fdata);
			if (ft_process_format(fdata) == -1)
				return (-1);
			if (fdata->type == 'c' && fdata->current[0] == 0)
				ft_strncat(dest, "\0", 1);
			ft_strncat(dest, fdata->current, fdata->clen);
			free(fdata->current);
			count += fdata->clen;
		}
		else
		{
			dest[count] = *format++;
			count++;
		}
	}
	return (count);
}

char* ft_fprintf(char *dest, const char *format, ...)
{
	t_fdata	*fdata;

	if (!format)
		return (0);
	fdata = malloc(sizeof(t_fdata));
	if (!fdata)
		return (NULL);
	va_start(fdata->ap, format);
	ft_fprintf_format(dest, (char *) format, fdata);
	va_end(fdata->ap);
	free(fdata);
	return (dest);
}
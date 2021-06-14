/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 13:17:02 by agirona           #+#    #+#             */
/*   Updated: 2021/06/14 20:56:27 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *str)
{
	int		i;

	i = 0;
	while (str[i])
		ft_putchar(str[i++]);
}

void	ft_putnbr(int nb)
{
	if (nb == -2147483648)
	{
		ft_putchar('-');
		ft_putnbr(214748364);
		ft_putchar(8 + '0');
	}
	else if (nb < 0)
	{
		ft_putchar('-');
		ft_putnbr(nb * -1);
	}
	else if (nb >= 10)
	{
		ft_putnbr(nb / 10);
		ft_putchar(nb % 10 + '0');
	}
	else
		ft_putchar(nb + '0');
}

int		ft_strlen(const char *str)
{
	int		i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int		nb_len(unsigned int	nb)
{
	int		count;

	count = 1;
	while (nb / 10)
	{
		nb /= 10;
		count++;
	}
	return (count);
}

char	*reverse(char *old, int i)
{
	char	*result;
	int		c;

	result = malloc(sizeof(char) * i + 1);
	if (result == NULL)
		return (NULL);
	c = 0;
	while (i > 0)
	{
		result[c] = old[i - 1];
		i--;
		c++;
	}
	result[c] = '\0';
	free(old);
	return (result);
}

char	*convert(unsigned int decimal)
{
	char			*result;
	int				tmp;
	int				i;

	result = malloc(sizeof(char) * (nb_len(decimal) + 1));
	if (result == NULL)
		return (NULL);
	if (decimal == 0)
	{
		result[0] = '0';
		result[1] = '\0';
		return (result);
	}
	i = 0;
	while (decimal > 0)
	{
		tmp = decimal % 16;
		if (tmp >= 10)
			result[i] = 97 + (tmp - 10);
		else
			result[i] = tmp % 10 + '0';
		decimal /= 16;
		i++;
	}
	result[i] = '\0';
	return (reverse(result, i));
}

void	string_prec(char *str, int len, int preclen)
{
	int		c;
	int		space;

	c = 0;
	if (ft_strlen(str) > preclen)
		space = len - preclen;
	else
		space = len - ft_strlen(str);
	while (c < space)
	{
		ft_putchar(' ');
		c++;
	}
	c = 0;
	while (str[c] && c < preclen)
		ft_putchar(str[c++]);
}

void	string(va_list arg, int prec, int len, int preclen)
{
	char	*str;
	int		i;
	(void)len;

	i = 0;
	str = va_arg(arg, char *);
	if (prec == 1)
		return (string_prec(str, len, preclen));
	while (i < len - ft_strlen(str))
	{
		ft_putchar(' ');
		i++;
	}
	ft_putstr(str);
}

void	number_prec(int nb, int len, int preclen)
{
	(void)len;
	(void)preclen;
	int		i;
	int		zero;

	i = 0;
	if (preclen < len)
	{
		zero = len - nb_len(nb) - (len - preclen);
		while (i < (len - preclen))
		{
			ft_putchar(' ');
			i++;
		}
	}
	else
		zero = preclen - nb_len(nb);
	i = 0;
	while (i < zero)
	{
		ft_putchar('0');
		i++;
	}
	ft_putnbr(nb);
}

void	number(va_list arg, int prec, int len, int preclen)
{
	int		nb;
	int		i;
	(void)prec;

	nb = va_arg(arg, int);
	if (prec == 1 && preclen != 0)
		return (number_prec(nb, len, preclen));
	i = 0;
	while (i < len - nb_len(nb))
	{
		ft_putchar(' ');
		i++;
	}
	if (prec != 1 && preclen != 0)
		ft_putnbr(nb);
	else if (len != 0)
		ft_putchar(' ');
}

void	hexa(va_list arg, int prec, int len)
{
	unsigned int		nb;
	char	*res;
	int		i;
	(void)prec;

	i = 0;
	nb = va_arg(arg, unsigned int);
	res = convert(nb);
	while (i < len - ft_strlen(res))
	{
		if (prec == 1)
			ft_putchar('0');
		else
			ft_putchar(' ');
		i++;
	}
	if (prec != 1 && nb != 0)
		ft_putstr(res);
}

void	get_flags(const char *str, va_list arg, int *i)
{
	int		prec;
	int		preclen;
	int		nb;

	prec = 0;
	preclen = 0;
	nb = 0;
	while (str[*i] && (str[*i] != 's' && str[*i] != 'd' && str[*i] != 'x'))
	{
		if (str[*i] == '.')
		{
			prec = 1;
			*i = *i + 1;
			while (str[*i] >= '0' && str[*i] <= '9')
			{
				preclen = preclen * 10 + str[*i] - '0';
				*i = *i + 1;
			}
		}
		else if (str[*i] >= '0' && str[*i] <= '9')
		{
			while (str[*i] >= '0' && str[*i] <= '9')
			{
				nb = nb * 10 + str[*i] - '0';
				*i = *i + 1;
			}
		}
		else
			*i = *i + 1;
	}
	ft_putstr("precision = ");
	ft_putnbr(preclen);
	ft_putchar('\n');
	ft_putstr("space = ");
	ft_putnbr(nb);
	ft_putchar('\n');
	ft_putchar('\n');
	if (str[*i] == 's' && ++*i)
		string(arg, prec, nb, preclen);
	if (str[*i] == 'd' && ++*i)
		number(arg, prec, nb, preclen);
	if (str[*i] == 'x' && ++*i)
		hexa(arg, prec, nb);
}

void	cut_str(const char *str, va_list arg)
{
	int		i;

	i = 0;
	while (i < ft_strlen(str))
	{
		if (str[i] == '%' && str[i + 1] == '%')
		{
			ft_putchar('%');
			i += 2;
		}
		else if (str[i] == '%')
			get_flags(str, arg, &i);
		else
			ft_putchar(str[i++]);
	}
}

int ft_printf(const char *str, ... )
{
	va_list		arg;

	va_start(arg, str);
	cut_str(str, arg);
	va_end(arg);
	return (0);
}

int		main(void)
{
	unsigned int	decimal;
	char			*result;

	decimal = -214748364;
	result = convert(decimal);
	if (result == NULL)
		return (0);
	ft_printf("%.d", 0);
	ft_putstr("\n\n");
	dprintf(1, "%.d", 0);
	return (0);
}

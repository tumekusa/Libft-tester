#include "libft.h"
#include <stdio.h>
#include <ctype.h>

int	main(void)
{
	int			c;
	int			ok;
	int			ko;
	int			pass;
	const char	*color;
	const char	*label;
	const char	*green;
	const char	*red;
	const char	*cyan;
	const char	*reset;

	green = "\033[32m";
	red   = "\033[31m";
	cyan  = "\033[36m";
	reset = "\033[0m";

	ok = 0;
	ko = 0;
	printf("%s========= test_tolower =========%s\n", cyan, reset);

	c = 'A';
	pass = (ft_tolower(c) == tolower(c));
	if (pass)
	{
		color = green;
		label = "OK";
		ok++;
	}
	else
	{
		color = red;
		label = "KO";
		ko++;
	}
	printf("[%d] 'A' (大文字)  expect='a'  ft=%d libc=%d  %s%s%s\n",
		1, ft_tolower(c), tolower(c), color, label, reset);

	c = 'Z';
	pass = (ft_tolower(c) == tolower(c));
	if (pass)
	{
		color = green;
		label = "OK";
		ok++;
	}
	else
	{
		color = red;
		label = "KO";
		ko++;
	}
	printf("[%d] 'Z' (大文字)  expect='z'  ft=%d libc=%d  %s%s%s\n",
		2, ft_tolower(c), tolower(c), color, label, reset);

	c = 'a';
	pass = (ft_tolower(c) == tolower(c));
	if (pass)
	{
		color = green;
		label = "OK";
		ok++;
	}
	else
	{
		color = red;
		label = "KO";
		ko++;
	}
	printf("[%d] 'a' (既に小文字、そのまま)  expect='a'  ft=%d libc=%d  %s%s%s\n",
		3, ft_tolower(c), tolower(c), color, label, reset);

	c = '5';
	pass = (ft_tolower(c) == tolower(c));
	if (pass)
	{
		color = green;
		label = "OK";
		ok++;
	}
	else
	{
		color = red;
		label = "KO";
		ko++;
	}
	printf("[%d] '5' (アルファベット外)  expect='5'  ft=%d libc=%d  %s%s%s\n",
		4, ft_tolower(c), tolower(c), color, label, reset);

	c = '@';
	pass = (ft_tolower(c) == tolower(c));
	if (pass)
	{
		color = green;
		label = "OK";
		ok++;
	}
	else
	{
		color = red;
		label = "KO";
		ko++;
	}
	printf("[%d] '@' (記号)  expect='@'  ft=%d libc=%d  %s%s%s\n",
		5, ft_tolower(c), tolower(c), color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

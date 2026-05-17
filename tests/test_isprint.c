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
	printf("%s========= test_isprint =========%s\n", cyan, reset);

	c = ' ';
	pass = (!!ft_isprint(c) == !!isprint(c));
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
	printf("[%d] ' ' (印字可能の下限)  expect=非0  ft=%d libc=%d  %s%s%s\n",
		1, !!ft_isprint(c), !!isprint(c), color, label, reset);

	c = '~';
	pass = (!!ft_isprint(c) == !!isprint(c));
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
	printf("[%d] '~' (印字可能の上限)  expect=非0  ft=%d libc=%d  %s%s%s\n",
		2, !!ft_isprint(c), !!isprint(c), color, label, reset);

	c = 'A';
	pass = (!!ft_isprint(c) == !!isprint(c));
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
	printf("[%d] 'A'  expect=非0  ft=%d libc=%d  %s%s%s\n",
		3, !!ft_isprint(c), !!isprint(c), color, label, reset);

	c = '\t';
	pass = (!!ft_isprint(c) == !!isprint(c));
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
	printf("[%d] '\\t' (制御文字)  expect=0  ft=%d libc=%d  %s%s%s\n",
		4, !!ft_isprint(c), !!isprint(c), color, label, reset);

	c = 127;
	pass = (!!ft_isprint(c) == !!isprint(c));
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
	printf("[%d] 127 (DEL、制御文字)  expect=0  ft=%d libc=%d  %s%s%s\n",
		5, !!ft_isprint(c), !!isprint(c), color, label, reset);

	c = 128;
	pass = (!!ft_isprint(c) == !!isprint(c));
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
	printf("[%d] 128 (ASCII 範囲外)  expect=0  ft=%d libc=%d  %s%s%s\n",
		6, !!ft_isprint(c), !!isprint(c), color, label, reset);

	c = -1;
	pass = (!!ft_isprint(c) == !!isprint(c));
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
	printf("[%d] -1 (EOF)  expect=0  ft=%d libc=%d  %s%s%s\n",
		7, !!ft_isprint(c), !!isprint(c), color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

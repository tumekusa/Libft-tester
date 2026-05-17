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

	/*
	** ANSI escape sequence で端末文字に色を付ける。
	** "\033" は ESC、続く "[32m" が「以降を緑」、"[31m" が「赤」、
	** "[36m" が「シアン」、"[0m" が「色リセット」。
	*/
	green = "\033[32m";
	red   = "\033[31m";
	cyan  = "\033[36m";
	reset = "\033[0m";

	ok = 0;
	ko = 0;
	printf("%s========= test_isalpha =========%s\n", cyan, reset);

	c = 'a';
	pass = (!!ft_isalpha(c) == !!isalpha(c));
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
	printf("[%d] 'a' (小文字)  expect=非0  ft=%d libc=%d  %s%s%s\n",
		1, !!ft_isalpha(c), !!isalpha(c), color, label, reset);

	c = 'Z';
	pass = (!!ft_isalpha(c) == !!isalpha(c));
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
	printf("[%d] 'Z' (大文字)  expect=非0  ft=%d libc=%d  %s%s%s\n",
		2, !!ft_isalpha(c), !!isalpha(c), color, label, reset);

	c = '5';
	pass = (!!ft_isalpha(c) == !!isalpha(c));
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
	printf("[%d] '5' (数字、アルファベットでない)  expect=0  ft=%d libc=%d  %s%s%s\n",
		3, !!ft_isalpha(c), !!isalpha(c), color, label, reset);

	c = '!';
	pass = (!!ft_isalpha(c) == !!isalpha(c));
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
	printf("[%d] '!' (記号)  expect=0  ft=%d libc=%d  %s%s%s\n",
		4, !!ft_isalpha(c), !!isalpha(c), color, label, reset);

	c = ' ';
	pass = (!!ft_isalpha(c) == !!isalpha(c));
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
	printf("[%d] ' ' (空白)  expect=0  ft=%d libc=%d  %s%s%s\n",
		5, !!ft_isalpha(c), !!isalpha(c), color, label, reset);

	c = 128;
	pass = (!!ft_isalpha(c) == !!isalpha(c));
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
		6, !!ft_isalpha(c), !!isalpha(c), color, label, reset);

	c = -1;
	pass = (!!ft_isalpha(c) == !!isalpha(c));
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
		7, !!ft_isalpha(c), !!isalpha(c), color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

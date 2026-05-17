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
	** printf にそのまま埋め込めば対応した端末で色が付く。
	*/
	green = "\033[32m";
	red   = "\033[31m";
	cyan  = "\033[36m";
	reset = "\033[0m";

	ok = 0;
	ko = 0;
	printf("%s========= test_isalnum =========%s\n", cyan, reset);

	c = 'a';
	pass = (!!ft_isalnum(c) == !!isalnum(c));
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
	printf("[%d] 'a' (英数字)  expect=非0  ft=%d libc=%d  %s%s%s\n",
		1, !!ft_isalnum(c), !!isalnum(c), color, label, reset);

	c = 'Z';
	pass = (!!ft_isalnum(c) == !!isalnum(c));
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
	printf("[%d] 'Z' (英数字)  expect=非0  ft=%d libc=%d  %s%s%s\n",
		2, !!ft_isalnum(c), !!isalnum(c), color, label, reset);

	c = '5';
	pass = (!!ft_isalnum(c) == !!isalnum(c));
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
	printf("[%d] '5' (英数字)  expect=非0  ft=%d libc=%d  %s%s%s\n",
		3, !!ft_isalnum(c), !!isalnum(c), color, label, reset);

	c = '!';
	pass = (!!ft_isalnum(c) == !!isalnum(c));
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
	printf("[%d] '!' (英数字でない)  expect=0  ft=%d libc=%d  %s%s%s\n",
		4, !!ft_isalnum(c), !!isalnum(c), color, label, reset);

	c = ' ';
	pass = (!!ft_isalnum(c) == !!isalnum(c));
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
	printf("[%d] ' ' (英数字でない)  expect=0  ft=%d libc=%d  %s%s%s\n",
		5, !!ft_isalnum(c), !!isalnum(c), color, label, reset);

	c = '\0';
	pass = (!!ft_isalnum(c) == !!isalnum(c));
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
	printf("[%d] '\\0' (NUL)  expect=0  ft=%d libc=%d  %s%s%s\n",
		6, !!ft_isalnum(c), !!isalnum(c), color, label, reset);

	c = 128;
	pass = (!!ft_isalnum(c) == !!isalnum(c));
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
		7, !!ft_isalnum(c), !!isalnum(c), color, label, reset);

	c = 255;
	pass = (!!ft_isalnum(c) == !!isalnum(c));
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
	printf("[%d] 255 (ASCII 範囲外)  expect=0  ft=%d libc=%d  %s%s%s\n",
		8, !!ft_isalnum(c), !!isalnum(c), color, label, reset);

	c = -1;
	pass = (!!ft_isalnum(c) == !!isalnum(c));
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
		9, !!ft_isalnum(c), !!isalnum(c), color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

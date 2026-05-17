#include "libft.h"
#include <stdio.h>
#include <string.h>

/* 戻り値の符号だけ取り出す: 正→1 / 負→-1 / 0→0 */
static int	sign_of(int x)
{
	if (x > 0)
		return (1);
	if (x < 0)
		return (-1);
	return (0);
}

int	main(void)
{
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
	printf("%s========= test_strncmp =========%s\n", cyan, reset);

	pass = (sign_of(ft_strncmp("hello", "hello", 5)) == sign_of(strncmp("hello", "hello", 5)));
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
	printf("[%d] strncmp(\"hello\", \"hello\", 5)  expect=符号0  ft=%d libc=%d  %s%s%s\n",
		1, ft_strncmp("hello", "hello", 5), strncmp("hello", "hello", 5), color, label, reset);

	pass = (sign_of(ft_strncmp("abc", "abd", 3)) == sign_of(strncmp("abc", "abd", 3)));
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
	printf("[%d] strncmp(\"abc\", \"abd\", 3)  expect=符号-1  ft=%d libc=%d  %s%s%s\n",
		2, ft_strncmp("abc", "abd", 3), strncmp("abc", "abd", 3), color, label, reset);

	pass = (sign_of(ft_strncmp("abd", "abc", 3)) == sign_of(strncmp("abd", "abc", 3)));
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
	printf("[%d] strncmp(\"abd\", \"abc\", 3)  expect=符号+1  ft=%d libc=%d  %s%s%s\n",
		3, ft_strncmp("abd", "abc", 3), strncmp("abd", "abc", 3), color, label, reset);

	pass = (sign_of(ft_strncmp("abc", "abd", 2)) == sign_of(strncmp("abc", "abd", 2)));
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
	printf("[%d] strncmp(\"abc\", \"abd\", 2) (prefix 一致)  expect=符号0  ft=%d libc=%d  %s%s%s\n",
		4, ft_strncmp("abc", "abd", 2), strncmp("abc", "abd", 2), color, label, reset);

	pass = (sign_of(ft_strncmp("", "", 0)) == sign_of(strncmp("", "", 0)));
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
	printf("[%d] strncmp(\"\", \"\", 0)  expect=符号0  ft=%d libc=%d  %s%s%s\n",
		5, ft_strncmp("", "", 0), strncmp("", "", 0), color, label, reset);

	pass = (sign_of(ft_strncmp("\x80", "\x01", 1)) == sign_of(strncmp("\x80", "\x01", 1)));
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
	printf("[%d] strncmp(\"\\x80\", \"\\x01\", 1) (unsigned char)  expect=符号+1  ft=%d libc=%d  %s%s%s\n",
		6, ft_strncmp("\x80", "\x01", 1), strncmp("\x80", "\x01", 1), color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

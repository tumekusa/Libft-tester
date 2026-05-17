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
	const char	*a = "hello";
	const char	*b = "hello";
	const char	*c = "hellx";
	const char	*d = "ab\0cd";
	const char	*e = "ab\0ce";
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
	printf("%s========= test_memcmp =========%s\n", cyan, reset);

	pass = (sign_of(ft_memcmp(a, b, 5)) == sign_of(memcmp(a, b, 5)));
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
	printf("[%d] memcmp(\"hello\", \"hello\", 5)  expect=符号0  ft=%d libc=%d  %s%s%s\n",
		1, ft_memcmp(a, b, 5), memcmp(a, b, 5), color, label, reset);

	pass = (sign_of(ft_memcmp(a, c, 5)) == sign_of(memcmp(a, c, 5)));
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
	printf("[%d] memcmp(\"hello\", \"hellx\", 5)  expect=符号-1  ft=%d libc=%d  %s%s%s\n",
		2, ft_memcmp(a, c, 5), memcmp(a, c, 5), color, label, reset);

	pass = (sign_of(ft_memcmp(a, c, 4)) == sign_of(memcmp(a, c, 4)));
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
	printf("[%d] memcmp(\"hello\", \"hellx\", 4) (prefix 一致)  expect=符号0  ft=%d libc=%d  %s%s%s\n",
		3, ft_memcmp(a, c, 4), memcmp(a, c, 4), color, label, reset);

	pass = (sign_of(ft_memcmp(d, e, 5)) == sign_of(memcmp(d, e, 5)));
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
	printf("[%d] memcmp(\"ab\\0cd\", \"ab\\0ce\", 5) (NUL 越え)  expect=符号-1  ft=%d libc=%d  %s%s%s\n",
		4, ft_memcmp(d, e, 5), memcmp(d, e, 5), color, label, reset);

	pass = (sign_of(ft_memcmp(a, b, 0)) == sign_of(memcmp(a, b, 0)));
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
	printf("[%d] memcmp(a, b, 0)  expect=符号0  ft=%d libc=%d  %s%s%s\n",
		5, ft_memcmp(a, b, 0), memcmp(a, b, 0), color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

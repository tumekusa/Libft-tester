#include "libft.h"
#include <stdio.h>
#include <string.h>

int	main(void)
{
	const char	*s;
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
	printf("%s========= test_strlen =========%s\n", cyan, reset);

	s = "";
	pass = (ft_strlen(s) == strlen(s));
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
	printf("[%d] \"\" (空文字列)  expect=0  ft=%zu libc=%zu  %s%s%s\n",
		1, ft_strlen(s), strlen(s), color, label, reset);

	s = "a";
	pass = (ft_strlen(s) == strlen(s));
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
	printf("[%d] \"a\" (1 文字)  expect=1  ft=%zu libc=%zu  %s%s%s\n",
		2, ft_strlen(s), strlen(s), color, label, reset);

	s = "hello";
	pass = (ft_strlen(s) == strlen(s));
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
	printf("[%d] \"hello\"  expect=5  ft=%zu libc=%zu  %s%s%s\n",
		3, ft_strlen(s), strlen(s), color, label, reset);

	s = "hello, world";
	pass = (ft_strlen(s) == strlen(s));
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
	printf("[%d] \"hello, world\"  expect=12  ft=%zu libc=%zu  %s%s%s\n",
		4, ft_strlen(s), strlen(s), color, label, reset);

	s = "ab\0cd";
	pass = (ft_strlen(s) == strlen(s));
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
	printf("[%d] \"ab\\0cd\" (途中 NUL)  expect=2  ft=%zu libc=%zu  %s%s%s\n",
		5, ft_strlen(s), strlen(s), color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

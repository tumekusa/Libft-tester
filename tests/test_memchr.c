#include "libft.h"
#include <stdio.h>
#include <string.h>

int	main(void)
{
	const char	*s = "hello\0world";
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
	printf("%s========= test_memchr =========%s\n", cyan, reset);

	pass = (ft_memchr(s, 'h', 11) == memchr(s, 'h', 11));
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
	printf("[%d] memchr(s, 'h', 11)  expect=libc と一致  ft=%p libc=%p  %s%s%s\n",
		1, ft_memchr(s, 'h', 11), memchr(s, 'h', 11), color, label, reset);

	pass = (ft_memchr(s, 'w', 11) == memchr(s, 'w', 11));
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
	printf("[%d] memchr(s, 'w', 11) (NUL 越え)  expect=libc と一致  ft=%p libc=%p  %s%s%s\n",
		2, ft_memchr(s, 'w', 11), memchr(s, 'w', 11), color, label, reset);

	pass = (ft_memchr(s, '\0', 11) == memchr(s, '\0', 11));
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
	printf("[%d] memchr(s, '\\0', 11) (NUL 探す)  expect=libc と一致  ft=%p libc=%p  %s%s%s\n",
		3, ft_memchr(s, '\0', 11), memchr(s, '\0', 11), color, label, reset);

	pass = (ft_memchr(s, 'z', 11) == memchr(s, 'z', 11));
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
	printf("[%d] memchr(s, 'z', 11) (見つからない)  expect=NULL  ft=%p  %s%s%s\n",
		4, ft_memchr(s, 'z', 11), color, label, reset);

	pass = (ft_memchr(s, 'h', 0) == memchr(s, 'h', 0));
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
	printf("[%d] memchr(s, 'h', 0) (範囲0)  expect=NULL  ft=%p  %s%s%s\n",
		5, ft_memchr(s, 'h', 0), color, label, reset);

	pass = (ft_memchr(s, 'w', 6) == memchr(s, 'w', 6));
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
	printf("[%d] memchr(s, 'w', 6) (範囲外)  expect=NULL  ft=%p  %s%s%s\n",
		6, ft_memchr(s, 'w', 6), color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

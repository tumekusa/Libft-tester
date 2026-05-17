#include "libft.h"
#include <stdio.h>
#include <string.h>

int	main(void)
{
	const char	*big = "hello world foo";
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
	printf("%s========= test_strnstr =========%s\n", cyan, reset);

	pass = (ft_strnstr(big, "world", 15) == big + 6);
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
	printf("[%d] strnstr(big, \"world\", 15)  expect=big+6  ft=%p  %s%s%s\n",
		1, (void *)ft_strnstr(big, "world", 15), color, label, reset);

	pass = (ft_strnstr(big, "hello", 15) == big);
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
	printf("[%d] strnstr(big, \"hello\", 15)  expect=big+0  ft=%p  %s%s%s\n",
		2, (void *)ft_strnstr(big, "hello", 15), color, label, reset);

	pass = (ft_strnstr(big, "", 15) == big);
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
	printf("[%d] strnstr(big, \"\", 15) (空 needle)  expect=big  ft=%p  %s%s%s\n",
		3, (void *)ft_strnstr(big, "", 15), color, label, reset);

	pass = (ft_strnstr(big, "xyz", 15) == NULL);
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
	printf("[%d] strnstr(big, \"xyz\", 15) (見つからない)  expect=NULL  ft=%p  %s%s%s\n",
		4, (void *)ft_strnstr(big, "xyz", 15), color, label, reset);

	pass = (ft_strnstr(big, "world", 6) == NULL);
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
	printf("[%d] strnstr(big, \"world\", 6) (n 不足)  expect=NULL  ft=%p  %s%s%s\n",
		5, (void *)ft_strnstr(big, "world", 6), color, label, reset);

	pass = (ft_strnstr(big, "world", 11) == big + 6);
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
	printf("[%d] strnstr(big, \"world\", 11) (ちょうど収まる)  expect=big+6  ft=%p  %s%s%s\n",
		6, (void *)ft_strnstr(big, "world", 11), color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

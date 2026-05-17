#include "libft.h"
#include <stdio.h>

int	main(void)
{
	int			c;
	int			ok;
	int			ko;
	int			pass;
	int			expected;
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
	printf("%s========= test_isascii =========%s\n", cyan, reset);

	c = 0;
	expected = (c >= 0 && c <= 127);
	pass = (!!ft_isascii(c) == expected);
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
	printf("[%d] 0 (ASCII 下限)  expect=非0  ft=%d spec=%d  %s%s%s\n",
		1, !!ft_isascii(c), expected, color, label, reset);

	c = 127;
	expected = (c >= 0 && c <= 127);
	pass = (!!ft_isascii(c) == expected);
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
	printf("[%d] 127 (ASCII 上限)  expect=非0  ft=%d spec=%d  %s%s%s\n",
		2, !!ft_isascii(c), expected, color, label, reset);

	c = 'A';
	expected = (c >= 0 && c <= 127);
	pass = (!!ft_isascii(c) == expected);
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
	printf("[%d] 'A' (ASCII 範囲内)  expect=非0  ft=%d spec=%d  %s%s%s\n",
		3, !!ft_isascii(c), expected, color, label, reset);

	c = 128;
	expected = (c >= 0 && c <= 127);
	pass = (!!ft_isascii(c) == expected);
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
	printf("[%d] 128 (ASCII 範囲外)  expect=0  ft=%d spec=%d  %s%s%s\n",
		4, !!ft_isascii(c), expected, color, label, reset);

	c = 255;
	expected = (c >= 0 && c <= 127);
	pass = (!!ft_isascii(c) == expected);
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
	printf("[%d] 255 (ASCII 範囲外)  expect=0  ft=%d spec=%d  %s%s%s\n",
		5, !!ft_isascii(c), expected, color, label, reset);

	c = -1;
	expected = (c >= 0 && c <= 127);
	pass = (!!ft_isascii(c) == expected);
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
	printf("[%d] -1 (負値)  expect=0  ft=%d spec=%d  %s%s%s\n",
		6, !!ft_isascii(c), expected, color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

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
	printf("%s========= test_atoi =========%s\n", cyan, reset);

	s = "0";
	pass = (ft_atoi(s) == atoi(s));
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
	printf("[%d] \"0\"  expect=0  ft=%d libc=%d  %s%s%s\n",
		1, ft_atoi(s), atoi(s), color, label, reset);

	s = "123";
	pass = (ft_atoi(s) == atoi(s));
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
	printf("[%d] \"123\"  expect=123  ft=%d libc=%d  %s%s%s\n",
		2, ft_atoi(s), atoi(s), color, label, reset);

	s = "-42";
	pass = (ft_atoi(s) == atoi(s));
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
	printf("[%d] \"-42\" (負の数)  expect=-42  ft=%d libc=%d  %s%s%s\n",
		3, ft_atoi(s), atoi(s), color, label, reset);

	s = "  +42";
	pass = (ft_atoi(s) == atoi(s));
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
	printf("[%d] \"  +42\" (空白 + 符号)  expect=42  ft=%d libc=%d  %s%s%s\n",
		4, ft_atoi(s), atoi(s), color, label, reset);

	s = "\t\n\v\f\r 42";
	pass = (ft_atoi(s) == atoi(s));
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
	printf("[%d] \"\\t\\n\\v\\f\\r 42\" (全 whitespace)  expect=42  ft=%d libc=%d  %s%s%s\n",
		5, ft_atoi(s), atoi(s), color, label, reset);

	s = "123abc";
	pass = (ft_atoi(s) == atoi(s));
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
	printf("[%d] \"123abc\" (打ち切り)  expect=123  ft=%d libc=%d  %s%s%s\n",
		6, ft_atoi(s), atoi(s), color, label, reset);

	s = "+-123";
	pass = (ft_atoi(s) == atoi(s));
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
	printf("[%d] \"+-123\" (二重符号)  expect=0  ft=%d libc=%d  %s%s%s\n",
		7, ft_atoi(s), atoi(s), color, label, reset);

	s = "2147483647";
	pass = (ft_atoi(s) == atoi(s));
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
	printf("[%d] \"2147483647\" (INT_MAX)  ft=%d libc=%d  %s%s%s\n",
		8, ft_atoi(s), atoi(s), color, label, reset);

	s = "-2147483648";
	pass = (ft_atoi(s) == atoi(s));
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
	printf("[%d] \"-2147483648\" (INT_MIN)  ft=%d libc=%d  %s%s%s\n",
		9, ft_atoi(s), atoi(s), color, label, reset);

	s = "9999999999999999999";
	pass = (ft_atoi(s) == atoi(s));
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
	printf("[%d] \"9999999999999999999\" (overflow)  ft=%d libc=%d  %s%s%s\n",
		10, ft_atoi(s), atoi(s), color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

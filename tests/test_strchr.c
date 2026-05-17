#include "libft.h"
#include <stdio.h>
#include <string.h>

int	main(void)
{
	const char	*s = "hello, world!";
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
	printf("%s========= test_strchr =========%s\n", cyan, reset);

	pass = (ft_strchr(s, 'l') == strchr(s, 'l'));
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
	printf("[%d] strchr(s, 'l')  expect=最初の 'l' (s+2)  ft=%p libc=%p  %s%s%s\n",
		1, (void *)ft_strchr(s, 'l'), (void *)strchr(s, 'l'), color, label, reset);

	pass = (ft_strchr(s, 'h') == strchr(s, 'h'));
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
	printf("[%d] strchr(s, 'h')  expect=s+0  ft=%p libc=%p  %s%s%s\n",
		2, (void *)ft_strchr(s, 'h'), (void *)strchr(s, 'h'), color, label, reset);

	pass = (ft_strchr(s, 'z') == strchr(s, 'z'));
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
	printf("[%d] strchr(s, 'z') (見つからない)  expect=NULL  ft=%p  %s%s%s\n",
		3, (void *)ft_strchr(s, 'z'), color, label, reset);

	pass = (ft_strchr(s, '\0') == strchr(s, '\0'));
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
	printf("[%d] strchr(s, '\\0')  expect=終端 (s+13)  ft=%p libc=%p  %s%s%s\n",
		4, (void *)ft_strchr(s, '\0'), (void *)strchr(s, '\0'), color, label, reset);

	pass = (ft_strchr("", '\0') != NULL && strchr("", '\0') != NULL);
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
	printf("[%d] strchr(\"\", '\\0')  expect=非NULL  ft=%p libc=%p  %s%s%s\n",
		5, (void *)ft_strchr("", '\0'), (void *)strchr("", '\0'), color, label, reset);

	pass = (ft_strchr(s, 256 + 'h') == strchr(s, 256 + 'h'));
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
	printf("[%d] strchr(s, 256+'h') (高位ビット)  expect=s+0  ft=%p libc=%p  %s%s%s\n",
		6, (void *)ft_strchr(s, 256 + 'h'), (void *)strchr(s, 256 + 'h'), color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

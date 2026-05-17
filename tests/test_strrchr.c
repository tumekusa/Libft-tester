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
	printf("%s========= test_strrchr =========%s\n", cyan, reset);

	pass = (ft_strrchr(s, 'l') == strrchr(s, 'l'));
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
	printf("[%d] strrchr(s, 'l')  expect=最後の 'l' (s+10)  ft=%p libc=%p  %s%s%s\n",
		1, (void *)ft_strrchr(s, 'l'), (void *)strrchr(s, 'l'), color, label, reset);

	pass = (ft_strrchr(s, 'o') == strrchr(s, 'o'));
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
	printf("[%d] strrchr(s, 'o')  expect=world の 'o' (s+8)  ft=%p libc=%p  %s%s%s\n",
		2, (void *)ft_strrchr(s, 'o'), (void *)strrchr(s, 'o'), color, label, reset);

	pass = (ft_strrchr(s, 'h') == strrchr(s, 'h'));
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
	printf("[%d] strrchr(s, 'h')  expect=s+0  ft=%p libc=%p  %s%s%s\n",
		3, (void *)ft_strrchr(s, 'h'), (void *)strrchr(s, 'h'), color, label, reset);

	pass = (ft_strrchr(s, 'z') == strrchr(s, 'z'));
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
	printf("[%d] strrchr(s, 'z') (見つからない)  expect=NULL  ft=%p  %s%s%s\n",
		4, (void *)ft_strrchr(s, 'z'), color, label, reset);

	pass = (ft_strrchr(s, '\0') == strrchr(s, '\0'));
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
	printf("[%d] strrchr(s, '\\0')  expect=終端 (s+13)  ft=%p libc=%p  %s%s%s\n",
		5, (void *)ft_strrchr(s, '\0'), (void *)strrchr(s, '\0'), color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

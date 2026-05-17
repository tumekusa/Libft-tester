#include "libft.h"
#include <stdio.h>
#include <string.h>

/* in-place で大文字化 (i は使わない) */
static void	upper_at(unsigned int i, char *c)
{
	(void)i;
	if (*c >= 'a' && *c <= 'z')
		*c -= 32;
}

/* in-place で index 足し算 ("abc" → "ace") */
static void	add_index_at(unsigned int i, char *c)
{
	*c = (char)(*c + i);
}

int	main(void)
{
	char		buf1[] = "hello";
	char		buf2[] = "abc";
	char		buf3[] = "";
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
	printf("%s========= test_striteri =========%s\n", cyan, reset);

	ft_striteri(buf1, upper_at);
	pass = (strcmp(buf1, "HELLO") == 0);
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
	printf("[%d] striteri(\"hello\", upper_at)  expect=\"HELLO\"  ft=\"%s\"  %s%s%s\n",
		1, buf1, color, label, reset);

	ft_striteri(buf2, add_index_at);
	pass = (strcmp(buf2, "ace") == 0);
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
	printf("[%d] striteri(\"abc\", add_index_at)  expect=\"ace\"  ft=\"%s\"  %s%s%s\n",
		2, buf2, color, label, reset);

	ft_striteri(buf3, upper_at);
	pass = (strcmp(buf3, "") == 0);
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
	printf("[%d] striteri(\"\", upper_at) (空文字列)  expect=\"\"  ft=\"%s\"  %s%s%s\n",
		3, buf3, color, label, reset);

	ft_striteri(NULL, upper_at);
	pass = 1;
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
	printf("[%d] striteri(NULL, upper_at) (NULL を弾く実装)  expect=落ちないこと  %s%s%s\n",
		4, color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

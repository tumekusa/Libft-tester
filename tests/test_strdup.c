#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	main(void)
{
	char		*d;
	char		src[] = "mutable";
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
	printf("%s========= test_strdup =========%s\n", cyan, reset);

	d = ft_strdup("hello");
	pass = (d != NULL && strcmp(d, "hello") == 0);
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
	printf("[%d] strdup(\"hello\")  expect=\"hello\"  ft=\"%s\"  %s%s%s\n",
		1, d ? d : "(NULL)", color, label, reset);
	free(d);

	d = ft_strdup("");
	pass = (d != NULL && d[0] == '\0');
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
	printf("[%d] strdup(\"\") (空文字列)  expect=非NULL+\"\"  ft=\"%s\"  %s%s%s\n",
		2, d ? d : "(NULL)", color, label, reset);
	free(d);

	d = ft_strdup(src);
	src[0] = 'X';
	pass = (d != NULL && strcmp(d, "mutable") == 0);
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
	printf("[%d] strdup(src) 後に src 変更  expect=\"mutable\" 維持  ft=\"%s\" src=\"%s\"  %s%s%s\n",
		3, d ? d : "(NULL)", src, color, label, reset);
	free(d);

	d = ft_strdup("There are 10 types of people in this world");
	pass = (d != NULL && strcmp(d, "There are 10 types of people in this world") == 0);
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
	printf("[%d] strdup(\"There are 10 types of people...\")  expect=同文字列  ft=\"%s\"  %s%s%s\n",
		4, d ? d : "(NULL)", color, label, reset);
	free(d);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

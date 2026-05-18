#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	main(void)
{
	char		*r;
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
	printf("%s========= test_strjoin =========%s\n", cyan, reset);

	r = ft_strjoin("Hello, ", "World!");
	pass = (r != NULL && strcmp(r, "Hello, World!") == 0);
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
	printf("[%d] strjoin(\"Hello, \", \"World!\")  expect=\"Hello, World!\"  ft=\"%s\"  %s%s%s\n",
		1, r ? r : "(NULL)", color, label, reset);
	free(r);

	r = ft_strjoin("", "shell");
	pass = (r != NULL && strcmp(r, "shell") == 0);
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
	printf("[%d] strjoin(\"\", \"shell\") (空 s1)  expect=\"shell\"  ft=\"%s\"  %s%s%s\n",
		2, r ? r : "(NULL)", color, label, reset);
	free(r);

	r = ft_strjoin("shell ", "script");
	pass = (r != NULL && strcmp(r, "shell script") == 0);
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
	printf("[%d] strjoin(\"shell \", \"script\")  expect=\"shell script\"  ft=\"%s\"  %s%s%s\n",
		3, r ? r : "(NULL)", color, label, reset);
	free(r);

	r = ft_strjoin("", "");
	pass = (r != NULL && strcmp(r, "") == 0);
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
	printf("[%d] strjoin(\"\", \"\") (両方空)  expect=\"\"  ft=\"%s\"  %s%s%s\n",
		4, r ? r : "(NULL)", color, label, reset);
	free(r);

	r = ft_strjoin(NULL, "kernel");
	pass = (r == NULL);
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
	printf("[%d] strjoin(NULL, \"kernel\") (NULL を弾く実装)  expect=NULL  ft=%p  %s%s%s\n",
		5, (void *)r, color, label, reset);

	r = ft_strjoin("daemon", NULL);
	pass = (r == NULL);
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
	printf("[%d] strjoin(\"daemon\", NULL)  expect=NULL  ft=%p  %s%s%s\n",
		6, (void *)r, color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

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
	printf("%s========= test_strtrim =========%s\n", cyan, reset);

	r = ft_strtrim("  /dev/null  ", " ");
	pass = (r != NULL && strcmp(r, "/dev/null") == 0);
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
	printf("[%d] strtrim(\"  /dev/null  \", \" \")  expect=\"/dev/null\"  ft=\"%s\"  %s%s%s\n",
		1, r ? r : "(NULL)", color, label, reset);
	free(r);

	r = ft_strtrim("###segfault###", "#");
	pass = (r != NULL && strcmp(r, "segfault") == 0);
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
	printf("[%d] strtrim(\"###segfault###\", \"#\")  expect=\"segfault\"  ft=\"%s\"  %s%s%s\n",
		2, r ? r : "(NULL)", color, label, reset);
	free(r);

	r = ft_strtrim(" \t\nfork(2)\n\t ", " \t\n");
	pass = (r != NULL && strcmp(r, "fork(2)") == 0);
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
	printf("[%d] strtrim(\" \\t\\nfork(2)\\n\\t \", \" \\t\\n\")  expect=\"fork(2)\"  ft=\"%s\"  %s%s%s\n",
		3, r ? r : "(NULL)", color, label, reset);
	free(r);

	r = ft_strtrim(";;;;", ";");
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
	printf("[%d] strtrim(\";;;;\", \";\") (全部 trim 対象)  expect=\"\"  ft=\"%s\"  %s%s%s\n",
		4, r ? r : "(NULL)", color, label, reset);
	free(r);

	r = ft_strtrim("==NULL==", "=");
	pass = (r != NULL && strcmp(r, "NULL") == 0);
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
	printf("[%d] strtrim(\"==NULL==\", \"=\") (内側残す)  expect=\"NULL\"  ft=\"%s\"  %s%s%s\n",
		5, r ? r : "(NULL)", color, label, reset);
	free(r);

	r = ft_strtrim("malloc", "");
	pass = (r != NULL && strcmp(r, "malloc") == 0);
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
	printf("[%d] strtrim(\"malloc\", \"\") (空 set)  expect=\"malloc\"  ft=\"%s\"  %s%s%s\n",
		6, r ? r : "(NULL)", color, label, reset);
	free(r);

	r = ft_strtrim(NULL, "a");
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
	printf("[%d] strtrim(NULL, \"a\") (NULL を弾く実装)  expect=NULL  ft=%p  %s%s%s\n",
		7, (void *)r, color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

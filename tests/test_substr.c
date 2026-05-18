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
	printf("%s========= test_substr =========%s\n", cyan, reset);

	r = ft_substr("0xDEADBEEF", 2, 8);
	pass = (r != NULL && strcmp(r, "DEADBEEF") == 0);
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
	printf("[%d] substr(\"0xDEADBEEF\", 2, 8)  expect=\"DEADBEEF\"  ft=\"%s\"  %s%s%s\n",
		1, r ? r : "(NULL)", color, label, reset);
	free(r);

	r = ft_substr("0xCAFEBABE", 2, 4);
	pass = (r != NULL && strcmp(r, "CAFE") == 0);
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
	printf("[%d] substr(\"0xCAFEBABE\", 2, 4)  expect=\"CAFE\"  ft=\"%s\"  %s%s%s\n",
		2, r ? r : "(NULL)", color, label, reset);
	free(r);

	r = ft_substr("0xFEEDFACE", 0, 100);
	pass = (r != NULL && strcmp(r, "0xFEEDFACE") == 0);
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
	printf("[%d] substr(\"0xFEEDFACE\", 0, 100) (len 過大)  expect=\"0xFEEDFACE\"  ft=\"%s\"  %s%s%s\n",
		3, r ? r : "(NULL)", color, label, reset);
	free(r);

	r = ft_substr("0xBADCAFE", 9, 3);
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
	printf("[%d] substr(\"0xBADCAFE\", 9, 3) (start=長さ)  expect=\"\"  ft=\"%s\"  %s%s%s\n",
		4, r ? r : "(NULL)", color, label, reset);
	free(r);

	r = ft_substr("0xBABEFACE", 100, 3);
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
	printf("[%d] substr(\"0xBABEFACE\", 100, 3) (start 過大)  expect=\"\"  ft=\"%s\"  %s%s%s\n",
		5, r ? r : "(NULL)", color, label, reset);
	free(r);

	r = ft_substr("0xDEADC0DE", 5, 0);
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
	printf("[%d] substr(\"0xDEADC0DE\", 5, 0) (len=0)  expect=\"\"  ft=\"%s\"  %s%s%s\n",
		6, r ? r : "(NULL)", color, label, reset);
	free(r);

	r = ft_substr(NULL, 0, 5);
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
	printf("[%d] substr(NULL, 0, 5) (NULL を弾く実装)  expect=NULL  ft=%p  %s%s%s\n",
		7, (void *)r, color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* テスト用 callback: 各文字に index を足す ("abc" → "ace") */
static char	add_index(unsigned int i, char c)
{
	return ((char)(c + i));
}

/* テスト用 callback: 小文字 → 大文字 (i は使わない) */
static char	to_upper_cb(unsigned int i, char c)
{
	(void)i;
	if (c >= 'a' && c <= 'z')
		return ((char)(c - 32));
	return (c);
}

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
	printf("%s========= test_strmapi =========%s\n", cyan, reset);

	r = ft_strmapi("abc", add_index);
	pass = (r != NULL && strcmp(r, "ace") == 0);
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
	printf("[%d] strmapi(\"abc\", add_index)  expect=\"ace\"  ft=\"%s\"  %s%s%s\n",
		1, r ? r : "(NULL)", color, label, reset);
	free(r);

	r = ft_strmapi("norminette", to_upper_cb);
	pass = (r != NULL && strcmp(r, "NORMINETTE") == 0);
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
	printf("[%d] strmapi(\"norminette\", to_upper_cb)  expect=\"NORMINETTE\"  ft=\"%s\"  %s%s%s\n",
		2, r ? r : "(NULL)", color, label, reset);
	free(r);

	r = ft_strmapi("", add_index);
	pass = (r != NULL && r[0] == '\0');
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
	printf("[%d] strmapi(\"\", add_index) (空文字列)  expect=非NULL+\"\"  ft=%p ft[0]=%d  %s%s%s\n",
		3, (void *)r, r ? r[0] : -1, color, label, reset);
	free(r);

	r = ft_strmapi(NULL, add_index);
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
	printf("[%d] strmapi(NULL, add_index)  expect=NULL  ft=%p  %s%s%s\n",
		4, (void *)r, color, label, reset);

	r = ft_strmapi("anything", NULL);
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
	printf("[%d] strmapi(\"anything\", NULL)  expect=NULL  ft=%p  %s%s%s\n",
		5, (void *)r, color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

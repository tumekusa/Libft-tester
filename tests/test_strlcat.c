#include "libft.h"
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char		dst[16];
	size_t		r;
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
	printf("%s========= test_strlcat =========%s\n", cyan, reset);

	strcpy(dst, "he");
	r = ft_strlcat(dst, "llo", 16);
	pass = (r == 5 && strcmp(dst, "hello") == 0);
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
	printf("[%d] strlcat(\"he\", \"llo\", 16)  expect=戻り5+dst=\"hello\"  ft戻り=%zu dst=\"%s\"  %s%s%s\n",
		1, r, dst, color, label, reset);

	strcpy(dst, "he");
	r = ft_strlcat(dst, "llo", 4);
	pass = (r == 5 && strcmp(dst, "hel") == 0);
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
	printf("[%d] strlcat(\"he\", \"llo\", 4) (切り詰め)  expect=戻り5+dst=\"hel\"  ft戻り=%zu dst=\"%s\"  %s%s%s\n",
		2, r, dst, color, label, reset);

	strcpy(dst, "he");
	r = ft_strlcat(dst, "llo", 0);
	pass = (r == 3 && strcmp(dst, "he") == 0);
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
	printf("[%d] strlcat(\"he\", \"llo\", 0)  expect=戻り3+dst=\"he\"  ft戻り=%zu dst=\"%s\"  %s%s%s\n",
		3, r, dst, color, label, reset);

	memset(dst, 'X', 16);
	memcpy(dst, "abcde", 5);
	r = ft_strlcat(dst, "z", 5);
	pass = (r == 6 && dst[0] == 'a' && dst[4] == 'e' && dst[5] == 'X');
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
	printf("[%d] strlcat(dst 非終端, \"z\", 5)  expect=戻り6+dst 不変+dst[5]='X'  ft戻り=%zu dst[4]='%c' dst[5]='%c'  %s%s%s\n",
		4, r, dst[4], dst[5], color, label, reset);

	strcpy(dst, "hello");
	r = ft_strlcat(dst, "", 16);
	pass = (r == 5 && strcmp(dst, "hello") == 0);
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
	printf("[%d] strlcat(\"hello\", \"\", 16) (空 src)  expect=戻り5+dst=\"hello\"  ft戻り=%zu  %s%s%s\n",
		5, r, color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

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
	printf("%s========= test_strlcpy =========%s\n", cyan, reset);

	memset(dst, 'X', 16);
	r = ft_strlcpy(dst, "hello", 16);
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
	printf("[%d] strlcpy(dst, \"hello\", 16)  expect=戻り5+dst=\"hello\"  ft戻り=%zu dst=\"%s\"  %s%s%s\n",
		1, r, dst, color, label, reset);

	memset(dst, 'X', 16);
	r = ft_strlcpy(dst, "hello", 3);
	pass = (r == 5 && strcmp(dst, "he") == 0);
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
	printf("[%d] strlcpy(dst, \"hello\", 3) (切り詰め)  expect=戻り5+dst=\"he\"  ft戻り=%zu dst=\"%s\"  %s%s%s\n",
		2, r, dst, color, label, reset);

	memset(dst, 'X', 16);
	r = ft_strlcpy(dst, "hello", 1);
	pass = (r == 5 && dst[0] == '\0');
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
	printf("[%d] strlcpy(dst, \"hello\", 1) (NUL のみ)  expect=戻り5+dst=\"\"  ft戻り=%zu dst[0]=%d  %s%s%s\n",
		3, r, dst[0], color, label, reset);

	memset(dst, 'X', 16);
	r = ft_strlcpy(dst, "hello", 0);
	pass = (r == 5 && dst[0] == 'X');
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
	printf("[%d] strlcpy(dst, \"hello\", 0)  expect=戻り5+dst 不変  ft戻り=%zu dst[0]='%c'  %s%s%s\n",
		4, r, dst[0], color, label, reset);

	memset(dst, 'X', 16);
	r = ft_strlcpy(dst, "", 16);
	pass = (r == 0 && dst[0] == '\0');
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
	printf("[%d] strlcpy(dst, \"\", 16) (空 src)  expect=戻り0+dst=\"\"  ft戻り=%zu dst[0]=%d  %s%s%s\n",
		5, r, dst[0], color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

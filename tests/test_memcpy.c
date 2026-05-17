#include "libft.h"
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char		src[16] = "hello, world!";
	char		dst1[16];
	char		dst2[16];
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
	printf("%s========= test_memcpy =========%s\n", cyan, reset);

	memset(dst1, 0, 16);
	memset(dst2, 0, 16);
	ft_memcpy(dst1, src, 5);
	memcpy(dst2, src, 5);
	pass = (memcmp(dst1, dst2, 16) == 0);
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
	printf("[%d] memcpy(dst, src, 5)  expect=\"hello\"  ft=\"%.5s\"  %s%s%s\n",
		1, dst1, color, label, reset);

	memset(dst1, 0xAA, 16);
	memset(dst2, 0xAA, 16);
	ft_memcpy(dst1, src, 14);
	memcpy(dst2, src, 14);
	pass = (memcmp(dst1, dst2, 16) == 0);
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
	printf("[%d] memcpy(dst, src, 14) (NUL 含む)  expect=libc と一致  %s%s%s\n",
		2, color, label, reset);

	memset(dst1, 0xAA, 16);
	memset(dst2, 0xAA, 16);
	ft_memcpy(dst1, src, 0);
	pass = (memcmp(dst1, dst2, 16) == 0);
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
	printf("[%d] memcpy(dst, src, 0)  expect=変化なし  %s%s%s\n",
		3, color, label, reset);

	pass = (ft_memcpy(dst1, src, 5) == dst1);
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
	printf("[%d] 戻り値  expect=dst 自身  ft=%p expect=%p  %s%s%s\n",
		4, ft_memcpy(dst1, src, 5), (void *)dst1, color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

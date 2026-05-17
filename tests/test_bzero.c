#include "libft.h"
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char		buf1[16];
	char		buf2[16];
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
	printf("%s========= test_bzero =========%s\n", cyan, reset);

	memset(buf1, 0xAA, 16);
	memset(buf2, 0xAA, 16);
	ft_bzero(buf1, 16);
	memset(buf2, 0, 16);
	pass = (memcmp(buf1, buf2, 16) == 0);
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
	printf("[%d] bzero(buf, 16) (元 0xAA)  expect=全0  buf[0]=%d  %s%s%s\n",
		1, buf1[0], color, label, reset);

	memset(buf1, 0xAA, 16);
	ft_bzero(buf1, 5);
	memset(buf2, 0xAA, 16);
	memset(buf2, 0, 5);
	pass = (memcmp(buf1, buf2, 16) == 0);
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
	printf("[%d] bzero(buf, 5) (先頭 5 だけ)  expect=先頭0+残り0xAA  buf[4]=%d buf[5]=%d  %s%s%s\n",
		2, buf1[4], (unsigned char)buf1[5], color, label, reset);

	memset(buf1, 0xAA, 16);
	memset(buf2, 0xAA, 16);
	ft_bzero(buf1, 0);
	pass = (memcmp(buf1, buf2, 16) == 0);
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
	printf("[%d] bzero(buf, 0)  expect=変化なし  buf[0]=%d  %s%s%s\n",
		3, (unsigned char)buf1[0], color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

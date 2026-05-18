#include "libft.h"
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char		buf1[10];
	char		buf2[10];
	void		*r;
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
	printf("%s========= test_memmove =========%s\n", cyan, reset);

	memcpy(buf1, "ABCDEFGHIJ", 10);
	memcpy(buf2, "ABCDEFGHIJ", 10);
	r = ft_memmove(buf1 + 5, buf1, 5);
	memmove(buf2 + 5, buf2, 5);
	pass = (memcmp(buf1, buf2, 10) == 0);
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
	printf("[%d] memmove(buf+5, buf, 5) (重複なし)  expect=libc と一致  ft=\"%.10s\" r=%p  %s%s%s\n",
		1, buf1, r, color, label, reset);

	memcpy(buf1, "0123456789", 10);
	memcpy(buf2, "0123456789", 10);
	r = ft_memmove(buf1 + 2, buf1, 5);
	memmove(buf2 + 2, buf2, 5);
	pass = (memcmp(buf1, buf2, 10) == 0);
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
	printf("[%d] memmove(buf+2, buf, 5) (前方 overlap)  expect=\"0101234789\"  ft=\"%.10s\" r=%p  %s%s%s\n",
		2, buf1, r, color, label, reset);

	memcpy(buf1, "0123456789", 10);
	memcpy(buf2, "0123456789", 10);
	r = ft_memmove(buf1, buf1 + 2, 5);
	memmove(buf2, buf2 + 2, 5);
	pass = (memcmp(buf1, buf2, 10) == 0);
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
	printf("[%d] memmove(buf, buf+2, 5) (後方 overlap)  expect=\"2345656789\"  ft=\"%.10s\" r=%p  %s%s%s\n",
		3, buf1, r, color, label, reset);

	memcpy(buf1, "0123456789", 10);
	memcpy(buf2, "0123456789", 10);
	r = ft_memmove(buf1 + 3, buf1, 0);
	pass = (memcmp(buf1, buf2, 10) == 0);
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
	printf("[%d] memmove(buf+3, buf, 0)  expect=変化なし  r=%p  %s%s%s\n",
		4, r, color, label, reset);

	r = ft_memmove(buf1, buf2, 5);
	pass = (r == buf1);
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
		5, r, (void *)buf1, color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

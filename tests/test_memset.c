#include "libft.h"
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char		buf1[16];
	char		buf2[16];
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
	printf("%s========= test_memset =========%s\n", cyan, reset);

	memset(buf1, 0, 16);
	memset(buf2, 0, 16);
	r = ft_memset(buf1, 'A', 5);
	memset(buf2, 'A', 5);
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
	printf("[%d] memset(buf, 'A', 5)  expect=libc と一致  ft=\"%.5s\" r=%p  %s%s%s\n",
		1, buf1, r, color, label, reset);

	memset(buf1, 0, 16);
	memset(buf2, 0, 16);
	r = ft_memset(buf1, 'X', 0);
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
	printf("[%d] memset(buf, 'X', 0)  expect=変化なし  buf[0]=%d r=%p  %s%s%s\n",
		2, buf1[0], r, color, label, reset);

	memset(buf1, 0, 16);
	memset(buf2, 0, 16);
	r = ft_memset(buf1, 256 + 'A', 4);
	memset(buf2, 'A', 4);
	pass = (memcmp(buf1, buf2, 4) == 0);
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
	printf("[%d] memset(buf, 256+'A', 4) (高位ビット切り捨て)  expect=\"AAAA\"  ft=\"%.4s\" r=%p  %s%s%s\n",
		3, buf1, r, color, label, reset);

	r = ft_memset(buf1, 'Z', 1);
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
	printf("[%d] 戻り値  expect=buf 自身  ft=%p expect=%p  %s%s%s\n",
		4, r, (void *)buf1, color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	char		*p;
	int			*q;
	void		*r;
	int			all_zero;
	int			i;
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
	printf("%s========= test_calloc =========%s\n", cyan, reset);

	p = ft_calloc(10, sizeof(char));
	all_zero = 1;
	i = 0;
	while (i < 10)
	{
		if (p[i] != 0)
			all_zero = 0;
		i++;
	}
	pass = (p != NULL && all_zero == 1);
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
	printf("[%d] ft_calloc(10, sizeof(char))  expect=非NULL+全0  ft=%p all_zero=%d  %s%s%s\n",
		1, (void *)p, all_zero, color, label, reset);
	free(p);

	q = ft_calloc(5, sizeof(int));
	pass = (q != NULL && q[0] == 0 && q[4] == 0);
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
	printf("[%d] ft_calloc(5, sizeof(int))  expect=非NULL+全0  ft=%p q[0]=%d q[4]=%d  %s%s%s\n",
		2, (void *)q, q ? q[0] : -1, q ? q[4] : -1, color, label, reset);
	free(q);

	r = ft_calloc(0, sizeof(int));
	pass = 1;
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
	printf("[%d] ft_calloc(0, sizeof(int))  expect=落ちないこと  ft=%p  %s%s%s\n",
		3, r, color, label, reset);
	free(r);

	r = ft_calloc((size_t)-1, 2);
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
	printf("[%d] ft_calloc((size_t)-1, 2) (overflow)  expect=NULL  ft=%p  %s%s%s\n",
		4, r, color, label, reset);

	r = ft_calloc(2, (size_t)-1);
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
	printf("[%d] ft_calloc(2, (size_t)-1) (overflow)  expect=NULL  ft=%p  %s%s%s\n",
		5, r, color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

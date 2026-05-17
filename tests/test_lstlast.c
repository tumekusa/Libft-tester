#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	t_list		*a;
	t_list		*b;
	t_list		*c;
	t_list		*solo;
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
	printf("%s========= test_lstlast =========%s\n", cyan, reset);

	pass = (ft_lstlast(NULL) == NULL);
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
	printf("[%d] lstlast(NULL)  expect=NULL  ft=%p  %s%s%s\n",
		1, (void *)ft_lstlast(NULL), color, label, reset);

	solo = ft_lstnew("solo");
	pass = (ft_lstlast(solo) == solo);
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
	printf("[%d] 1 ノードのみ  expect=自分自身  ft=%p (solo=%p)  %s%s%s\n",
		2, (void *)ft_lstlast(solo), (void *)solo, color, label, reset);
	free(solo);

	a = ft_lstnew("A");
	b = ft_lstnew("B");
	c = ft_lstnew("C");
	a->next = b;
	b->next = c;
	pass = (ft_lstlast(a) == c && ft_lstlast(a)->next == NULL);
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
	printf("[%d] a→b→c の 3 ノード  expect=c  ft=%p (c=%p)  %s%s%s\n",
		3, (void *)ft_lstlast(a), (void *)c, color, label, reset);

	free(a);
	free(b);
	free(c);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

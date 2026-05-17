#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	t_list		*lst;
	t_list		*a;
	t_list		*b;
	t_list		*c;
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
	printf("%s========= test_lstadd_front =========%s\n", cyan, reset);

	lst = NULL;
	a = ft_lstnew("A");
	b = ft_lstnew("B");
	c = ft_lstnew("C");

	ft_lstadd_front(&lst, a);
	pass = (lst == a && lst->next == NULL);
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
	printf("[%d] 空 lst に a を追加  expect=lst==a  lst=%p (a=%p)  %s%s%s\n",
		1, (void *)lst, (void *)a, color, label, reset);

	ft_lstadd_front(&lst, b);
	pass = (lst == b && lst->next == a);
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
	printf("[%d] 既存先頭 (a) に b を追加  expect=b→a  lst=%p->%p  %s%s%s\n",
		2, (void *)lst, (void *)lst->next, color, label, reset);

	ft_lstadd_front(&lst, c);
	pass = (lst == c && lst->next == b && lst->next->next == a && lst->next->next->next == NULL);
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
	printf("[%d] さらに c を追加  expect=c→b→a  %s%s%s\n",
		3, color, label, reset);

	ft_lstadd_front(&lst, NULL);
	pass = (lst == c);
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
	printf("[%d] NULL new (NULL を弾く実装)  expect=lst 不変  lst=%p (c=%p)  %s%s%s\n",
		4, (void *)lst, (void *)c, color, label, reset);

	free(a);
	free(b);
	free(c);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

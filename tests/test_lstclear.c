#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

static int	g_del_calls;

static void	spy_del(void *p)
{
	(void)p;
	g_del_calls++;
}

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
	printf("%s========= test_lstclear =========%s\n", cyan, reset);

	a = ft_lstnew(NULL);
	b = ft_lstnew(NULL);
	c = ft_lstnew(NULL);
	a->next = b;
	b->next = c;
	lst = a;
	g_del_calls = 0;
	ft_lstclear(&lst, spy_del);
	pass = (g_del_calls == 3 && lst == NULL);
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
	printf("[%d] 3 ノードを clear  expect=del 3 回+lst=NULL  calls=%d lst=%p  %s%s%s\n",
		1, g_del_calls, (void *)lst, color, label, reset);

	lst = NULL;
	g_del_calls = 0;
	ft_lstclear(&lst, spy_del);
	pass = (g_del_calls == 0 && lst == NULL);
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
	printf("[%d] 空リストを clear  expect=del 0 回  calls=%d  %s%s%s\n",
		2, g_del_calls, color, label, reset);

	a = ft_lstnew(NULL);
	lst = a;
	g_del_calls = 0;
	ft_lstclear(&lst, NULL);
	pass = (g_del_calls == 0 && lst == a);
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
	printf("[%d] NULL del (NULL チェックする実装の場合)  expect=del 呼ばれず+lst 不変  calls=%d  %s%s%s\n",
		3, g_del_calls, color, label, reset);
	free(a);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

/*
** spy パターン: del がいつ何で呼ばれたかを static 変数で記録する。
** ft_lstdelone が del を正しい引数で呼んでいるか確認する古典手法。
*/
static int		g_del_calls;
static void		*g_del_last;

static void	spy_del(void *p)
{
	g_del_calls++;
	g_del_last = p;
}

int	main(void)
{
	t_list		*n;
	char		content[] = "hello";
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
	printf("%s========= test_lstdelone =========%s\n", cyan, reset);

	n = ft_lstnew(content);
	g_del_calls = 0;
	g_del_last = NULL;
	ft_lstdelone(n, spy_del);
	pass = (g_del_calls == 1 && g_del_last == content);
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
	printf("[%d] lstdelone(n, spy_del)  expect=del 1 回+引数==content  calls=%d last=%p  %s%s%s\n",
		1, g_del_calls, g_del_last, color, label, reset);

	g_del_calls = 0;
	ft_lstdelone(NULL, spy_del);
	pass = (g_del_calls == 0);
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
	printf("[%d] lstdelone(NULL, spy_del)  expect=del 呼ばれない  calls=%d  %s%s%s\n",
		2, g_del_calls, color, label, reset);

	n = ft_lstnew(content);
	ft_lstdelone(n, NULL);
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
	printf("[%d] lstdelone(n, NULL) (NULL チェックする実装の場合)  expect=落ちないこと  %s%s%s\n",
		3, color, label, reset);
	free(n);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

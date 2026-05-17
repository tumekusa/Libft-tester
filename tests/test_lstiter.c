#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
** f に渡された content (char) を順に g_buf に溜める。
** 呼ばれた回数・順序・content の中身を一度に確認できる spy。
*/
static char	g_buf[16];
static int	g_buf_pos;

static void	append_char(void *p)
{
	g_buf[g_buf_pos] = *(char *)p;
	g_buf_pos++;
}

int	main(void)
{
	t_list		*lst;
	t_list		*a;
	t_list		*b;
	t_list		*c;
	char		ca = 'A';
	char		cb = 'B';
	char		cc = 'C';
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
	printf("%s========= test_lstiter =========%s\n", cyan, reset);

	a = ft_lstnew(&ca);
	b = ft_lstnew(&cb);
	c = ft_lstnew(&cc);
	a->next = b;
	b->next = c;
	lst = a;

	g_buf_pos = 0;
	memset(g_buf, 0, sizeof(g_buf));
	ft_lstiter(lst, append_char);
	g_buf[g_buf_pos] = '\0';
	pass = (strcmp(g_buf, "ABC") == 0);
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
	printf("[%d] lstiter(lst, append_char) (3 ノード)  expect=\"ABC\"  ft=\"%s\"  %s%s%s\n",
		1, g_buf, color, label, reset);

	g_buf_pos = 0;
	ft_lstiter(NULL, append_char);
	pass = (g_buf_pos == 0);
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
	printf("[%d] lstiter(NULL, append_char)  expect=f 呼ばれない  calls=%d  %s%s%s\n",
		2, g_buf_pos, color, label, reset);

	g_buf_pos = 0;
	ft_lstiter(lst, NULL);
	pass = (g_buf_pos == 0);
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
	printf("[%d] lstiter(lst, NULL) (NULL チェックする実装の場合)  expect=f 呼ばれず+落ちない  calls=%d  %s%s%s\n",
		3, g_buf_pos, color, label, reset);

	free(a);
	free(b);
	free(c);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	t_list		*n;
	char		*content;
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
	printf("%s========= test_lstnew =========%s\n", cyan, reset);

	content = "hello";
	n = ft_lstnew(content);
	pass = (n != NULL && n->content == content && n->next == NULL);
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
	printf("[%d] lstnew(\"hello\")  expect=非NULL+content==渡したポインタ+next=NULL  ft=%p  %s%s%s\n",
		1, (void *)n, color, label, reset);
	free(n);

	n = ft_lstnew(NULL);
	pass = (n != NULL && n->content == NULL && n->next == NULL);
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
	printf("[%d] lstnew(NULL)  expect=非NULL ノード+content=NULL  ft=%p  %s%s%s\n",
		2, (void *)n, color, label, reset);
	free(n);

	n = ft_lstnew((void *)0x1234);
	pass = (n != NULL && n->content == (void *)0x1234 && n->next == NULL);
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
	printf("[%d] lstnew((void *)0x1234)  expect=content=0x1234  ft=%p content=%p  %s%s%s\n",
		3, (void *)n, n ? n->content : NULL, color, label, reset);
	free(n);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
** map 用 f: content (char *) を新たに malloc して大文字化したコピーを返す。
** ft_lstmap は新リストを作るので、content も新規 alloc である必要がある。
*/
static void	*content_to_upper(void *p)
{
	char	*src;
	char	*ret;
	size_t	i;

	src = (char *)p;
	ret = malloc(strlen(src) + 1);
	if (ret == NULL)
		return (NULL);
	i = 0;
	while (src[i] != '\0')
	{
		if (src[i] >= 'a' && src[i] <= 'z')
			ret[i] = src[i] - 32;
		else
			ret[i] = src[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

/* del 用: 新リストの content を解放する */
static void	free_str(void *p)
{
	free(p);
}

int	main(void)
{
	t_list		*src_list;
	t_list		*dst_list;
	t_list		*a;
	t_list		*b;
	t_list		*c;
	t_list		*tmp;
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
	printf("%s========= test_lstmap =========%s\n", cyan, reset);

	a = ft_lstnew("hello");
	b = ft_lstnew("world");
	c = ft_lstnew("foo");
	a->next = b;
	b->next = c;
	src_list = a;

	dst_list = ft_lstmap(src_list, content_to_upper, free_str);
	pass = (dst_list != NULL
			&& strcmp((char *)dst_list->content, "HELLO") == 0
			&& strcmp((char *)dst_list->next->content, "WORLD") == 0
			&& strcmp((char *)dst_list->next->next->content, "FOO") == 0
			&& dst_list->next->next->next == NULL);
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
	printf("[%d] lstmap(src, to_upper, free_str)  expect=\"HELLO\"/\"WORLD\"/\"FOO\"  %s%s%s\n",
		1, color, label, reset);

	pass = (strcmp((char *)src_list->content, "hello") == 0
			&& strcmp((char *)src_list->next->content, "world") == 0);
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
	printf("[%d] src 不変か  expect=\"hello\"/\"world\" 維持  ft[0]=\"%s\"  %s%s%s\n",
		2, (char *)src_list->content, color, label, reset);

	while (dst_list != NULL)
	{
		tmp = dst_list->next;
		free(dst_list->content);
		free(dst_list);
		dst_list = tmp;
	}
	free(a);
	free(b);
	free(c);

	dst_list = ft_lstmap(NULL, content_to_upper, free_str);
	pass = (dst_list == NULL);
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
	printf("[%d] lstmap(NULL, ...)  expect=NULL  ft=%p  %s%s%s\n",
		3, (void *)dst_list, color, label, reset);

	a = ft_lstnew("x");
	dst_list = ft_lstmap(a, NULL, free_str);
	pass = (dst_list == NULL);
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
	printf("[%d] lstmap(lst, NULL, free_str) (NULL チェックする実装の場合)  expect=NULL  ft=%p  %s%s%s\n",
		4, (void *)dst_list, color, label, reset);
	free(a);

	a = ft_lstnew("x");
	dst_list = ft_lstmap(a, content_to_upper, NULL);
	pass = (dst_list == NULL);
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
	printf("[%d] lstmap(lst, to_upper, NULL)  expect=NULL  ft=%p  %s%s%s\n",
		5, (void *)dst_list, color, label, reset);
	free(a);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

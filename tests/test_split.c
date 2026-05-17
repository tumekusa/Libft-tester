#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 結果配列を free する小さなヘルパー (1 ファイル内で完結) */
static void	free_split(char **arr)
{
	size_t	i;

	if (arr == NULL)
		return ;
	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

/* NULL 終端までの単語数を数える */
static int	count_words(char **arr)
{
	int	i;

	if (arr == NULL)
		return (-1);
	i = 0;
	while (arr[i] != NULL)
		i++;
	return (i);
}

int	main(void)
{
	char		**r;
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
	printf("%s========= test_split =========%s\n", cyan, reset);

	r = ft_split("hello world foo", ' ');
	pass = (r != NULL && count_words(r) == 3
			&& strcmp(r[0], "hello") == 0
			&& strcmp(r[1], "world") == 0
			&& strcmp(r[2], "foo") == 0);
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
	printf("[%d] split(\"hello world foo\", ' ')  expect=3 単語 hello/world/foo  ft=%d 単語  %s%s%s\n",
		1, count_words(r), color, label, reset);
	free_split(r);

	r = ft_split("a,,b,,,c", ',');
	pass = (r != NULL && count_words(r) == 3
			&& strcmp(r[0], "a") == 0
			&& strcmp(r[1], "b") == 0
			&& strcmp(r[2], "c") == 0);
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
	printf("[%d] split(\"a,,b,,,c\", ',') (連続区切り)  expect=3 単語 a/b/c  ft=%d 単語  %s%s%s\n",
		2, count_words(r), color, label, reset);
	free_split(r);

	r = ft_split(",,hello,,", ',');
	pass = (r != NULL && count_words(r) == 1 && strcmp(r[0], "hello") == 0);
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
	printf("[%d] split(\",,hello,,\", ',') (先頭末尾区切り)  expect=1 単語 hello  ft=%d 単語  %s%s%s\n",
		3, count_words(r), color, label, reset);
	free_split(r);

	r = ft_split(",,,", ',');
	pass = (r != NULL && count_words(r) == 0);
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
	printf("[%d] split(\",,,\", ',') (全区切り)  expect=0 単語  ft=%d 単語  %s%s%s\n",
		4, count_words(r), color, label, reset);
	free_split(r);

	r = ft_split("", ',');
	pass = (r != NULL && count_words(r) == 0);
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
	printf("[%d] split(\"\", ',') (空文字列)  expect=0 単語  ft=%d 単語  %s%s%s\n",
		5, count_words(r), color, label, reset);
	free_split(r);

	r = ft_split(NULL, ',');
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
	printf("[%d] split(NULL, ',') (NULL を弾く実装)  expect=NULL  ft=%p  %s%s%s\n",
		6, (void *)r, color, label, reset);

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

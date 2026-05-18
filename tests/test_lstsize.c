#include "libft.h"
#include <stdio.h>

int	main(void)
{
	t_list		*lst;
	t_list		*n;
	t_list		nodes[5];
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
	printf("%s========= test_lstsize =========%s\n", cyan, reset);

	pass = (ft_lstsize(NULL) == 0);
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
	printf("[%d] lstsize(NULL)  expect=0  ft=%d  %s%s%s\n",
		1, ft_lstsize(NULL), color, label, reset);

	lst = NULL;
	i = 1;
	while (i <= 5)
	{
		n = &nodes[i - 1];
		n->content = NULL;
		n->next = lst;
		lst = n;
		pass = (ft_lstsize(lst) == i);
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
		printf("[%d] %d ノード追加後 lstsize  expect=%d  ft=%d  %s%s%s\n",
			i + 1, i, i, ft_lstsize(lst), color, label, reset);
		i++;
	}

	printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
		cyan, green, ok, reset, red, ko, reset, cyan, reset);
	if (ko > 0)
		return (1);
	return (0);
}

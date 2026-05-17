#include "libft.h"
#include <stdio.h>

int	main(void)
{
	const char	*cyan;
	const char	*reset;

	cyan  = "\033[36m";
	reset = "\033[0m";

	printf("%s========= test_putendl_fd =========%s\n", cyan, reset);

	printf("[1] putendl_fd(\"hello\", 1)  expect=hello+改行  ft=[");
	fflush(stdout);
	ft_putendl_fd("hello", 1);
	printf("]  (目視確認: hello のあと改行)\n");

	printf("[2] putendl_fd(\"\", 1) (空文字列)  expect=改行のみ  ft=[");
	fflush(stdout);
	ft_putendl_fd("", 1);
	printf("]  (目視確認)\n");

	printf("[3] putendl_fd(NULL, 1) (NULL を弾く実装)  expect=何も出ない  ft=[");
	fflush(stdout);
	ft_putendl_fd(NULL, 1);
	printf("]  (目視確認: 改行も出ないこと)\n");

	printf("[4] putendl_fd(\"the quick brown fox\", 1)  expect=全文+改行  ft=[");
	fflush(stdout);
	ft_putendl_fd("the quick brown fox", 1);
	printf("]  (目視確認)\n");

	printf("%s----- summary: 4 ケース (目視確認のみ) -----%s\n", cyan, reset);
	return (0);
}

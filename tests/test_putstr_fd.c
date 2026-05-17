#include "libft.h"
#include <stdio.h>

int	main(void)
{
	const char	*cyan;
	const char	*reset;

	cyan  = "\033[36m";
	reset = "\033[0m";

	printf("%s========= test_putstr_fd =========%s\n", cyan, reset);

	printf("[1] putstr_fd(\"hello\", 1)  expect=hello  ft=[");
	fflush(stdout);
	ft_putstr_fd("hello", 1);
	printf("]  (目視確認)\n");

	printf("[2] putstr_fd(\"\", 1) (空文字列)  expect=何も出ない  ft=[");
	fflush(stdout);
	ft_putstr_fd("", 1);
	printf("]  (目視確認)\n");

	printf("[3] putstr_fd(NULL, 1) (NULL を弾く実装)  expect=何も出ない  ft=[");
	fflush(stdout);
	ft_putstr_fd(NULL, 1);
	printf("]  (目視確認)\n");

	printf("[4] putstr_fd(\"ab\\ncd\", 1) (改行透過)  expect=2 行  ft=[");
	fflush(stdout);
	ft_putstr_fd("ab\ncd", 1);
	printf("]  (目視確認)\n");

	printf("[5] putstr_fd(\"the quick brown fox\", 1)  expect=全文  ft=[");
	fflush(stdout);
	ft_putstr_fd("the quick brown fox", 1);
	printf("]  (目視確認)\n");

	printf("%s----- summary: 5 ケース (目視確認のみ) -----%s\n", cyan, reset);
	return (0);
}

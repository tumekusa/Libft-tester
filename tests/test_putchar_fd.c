#include "libft.h"
#include <stdio.h>

/*
** put*_fd 系は fd=1 (stdout) にそのまま吐かせて目視確認するスタイル。
** fflush(stdout) を挟まないと printf のバッファと ft_put* の write が
** 順序逆転して見えるので必ず flush する。
** 自動判定 (OK/KO) はせず、サマリーには件数だけ出して「目視確認のみ」と
** 添える。
*/
int	main(void)
{
	const char	*cyan;
	const char	*reset;

	cyan  = "\033[36m";
	reset = "\033[0m";

	printf("%s========= test_putchar_fd =========%s\n", cyan, reset);

	printf("[1] putchar_fd('A', 1)  expect=A  ft=[");
	fflush(stdout);
	ft_putchar_fd('A', 1);
	printf("]  (目視確認)\n");

	printf("[2] putchar_fd(' ', 1) (空白)  expect=[ ]  ft=[");
	fflush(stdout);
	ft_putchar_fd(' ', 1);
	printf("]  (目視確認)\n");

	printf("[3] putchar_fd('\\n', 1)  expect=改行  ft=[");
	fflush(stdout);
	ft_putchar_fd('\n', 1);
	printf("]  (目視確認)\n");

	printf("[4] putchar_fd('7', 1)  expect=7  ft=[");
	fflush(stdout);
	ft_putchar_fd('7', 1);
	printf("]  (目視確認)\n");

	printf("%s----- summary: 4 ケース (目視確認のみ) -----%s\n", cyan, reset);
	return (0);
}

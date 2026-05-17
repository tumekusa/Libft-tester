#include "libft.h"
#include <stdio.h>
#include <limits.h>

int	main(void)
{
	const char	*cyan;
	const char	*reset;

	cyan  = "\033[36m";
	reset = "\033[0m";

	printf("%s========= test_putnbr_fd =========%s\n", cyan, reset);

	printf("[1] putnbr_fd(0, 1)  expect=0  ft=[");
	fflush(stdout);
	ft_putnbr_fd(0, 1);
	printf("]  (目視確認)\n");

	printf("[2] putnbr_fd(42, 1)  expect=42  ft=[");
	fflush(stdout);
	ft_putnbr_fd(42, 1);
	printf("]  (目視確認)\n");

	printf("[3] putnbr_fd(-42, 1)  expect=-42  ft=[");
	fflush(stdout);
	ft_putnbr_fd(-42, 1);
	printf("]  (目視確認)\n");

	printf("[4] putnbr_fd(INT_MAX, 1)  expect=2147483647  ft=[");
	fflush(stdout);
	ft_putnbr_fd(INT_MAX, 1);
	printf("]  (目視確認)\n");

	printf("[5] putnbr_fd(INT_MIN, 1)  expect=-2147483648  ft=[");
	fflush(stdout);
	ft_putnbr_fd(INT_MIN, 1);
	printf("]  (目視確認)\n");

	printf("%s----- summary: 5 ケース (目視確認のみ) -----%s\n", cyan, reset);
	return (0);
}

CC        = cc
CFLAGS    = -Wall -Wextra -Werror -g
LIBFT_DIR = ../Libft_git
LIBFT     = $(LIBFT_DIR)/libft.a
INCLUDES  = -I. -I$(LIBFT_DIR)

# 新しいテストを足したらここに 1 行追加する。
TESTS = \
	test_isalpha \
	test_isdigit \
	test_isalnum \
	test_isascii \
	test_isprint \
	test_toupper \
	test_tolower \
	test_strlen \
	test_memset \
	test_bzero \
	test_memcpy \
	test_memmove \
	test_strlcpy \
	test_strlcat \
	test_strchr \
	test_strrchr \
	test_strncmp \
	test_memchr \
	test_memcmp \
	test_strnstr \
	test_atoi \
	test_calloc \
	test_strdup \
	test_substr \
	test_strjoin \
	test_strtrim \
	test_split \
	test_itoa \
	test_strmapi \
	test_striteri \
	test_putchar_fd \
	test_putstr_fd \
	test_putendl_fd \
	test_putnbr_fd \
	test_lstnew \
	test_lstadd_front \
	test_lstsize \
	test_lstlast \
	test_lstadd_back \
	test_lstdelone \
	test_lstclear \
	test_lstiter \
	test_lstmap

all: $(LIBFT) $(TESTS)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# パターンルール: tests/test_<name>.c から test_<name> バイナリを作る。
# % にマッチした部分が $* で取れる。
test_%: tests/test_%.c $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) tests/test_$*.c $(LIBFT) -o $@

# 全テストを順に実行。テスト名見出しは各バイナリの冒頭で出る。
# バイナリは KO がゼロなら exit 0、1 つでも KO があれば exit 1 を返す。
test: all
	@for t in $(TESTS); do ./$$t; done

clean:
	rm -f $(TESTS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all test clean fclean re

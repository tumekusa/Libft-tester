# libft_tester

42 Tokyo の Libft 課題 (43 関数) のための自作テスター。`printf` と `if` だけで読み下せる「ベタ書き積み重ね」スタイルで、libft が書ける人なら **テスター本体も自力で改変・追加できる** ことを目指した。

---

## 目次

- [特徴](#特徴)
- [動作環境](#動作環境)
- [クイックスタート](#クイックスタート)
- [使い方](#使い方)
- [出力フォーマット](#出力フォーマット)
- [設計方針](#設計方針)
- [テストの追加手順](#テストの追加手順)
- [テスト方針一覧](#テスト方針一覧)
- [既知の限界](#既知の限界)
- [想定する libft 側の方針](#想定する-libft-側の方針)
- [ディレクトリ構成](#ディレクトリ構成)

---

## 特徴

- **1 関数 1 main、1 バイナリ**: `tests/test_<関数名>.c` をそのままコンパイルして個別実行ファイルにする。1 つが segfault しても他は走る。
- **共通ヘルパなし、マクロなし**: `helper.c` も `#define` も廃して、各テストファイルが `#include "libft.h"` と `#include <stdio.h>` だけで完結する。「libft が書ける」レベルの C 知識だけで全コード読める。
- **1 ケース 1 行に圧縮 + 色付き OK / KO**: 各ケースは 1 行で `[N] 入力  expect=...  ft=... libc=...  OK/KO` の形式。OK は緑、KO は赤、テスト名見出しとサマリーはシアン。長い縦スクロールにならない。
- **末尾サマリー + exit code**: 各テストバイナリは末尾で `----- summary: N OK / M KO -----` を出し、KO が 1 つでもあれば `exit 1` を返す。`make test` のフックや CI 連携で使える。
- **`libc` 並走で期待値を生成**: ctype / 文字列比較 / `atoi` など `libc` に対応関数がある場合は並走させて手書き期待値を省く。`libc` に対応が無いもの (`ft_substr` / `ft_split` / `ft_itoa` / `ft_lst*` など) は期待値を手書きで持つ。
- **失敗しても続行**: `KO` を出してから次のケースに進む。途中で止まらないので、全体の赤い場所を一度で見渡せる。
- **メモリリーク検知は外部に任せる**: `valgrind` または `-fsanitize=address` で別途確認する設計。
- **fd 出力系は目視確認**: `ft_putchar_fd` 等は `fd=1` (stdout) に出して、出力を `[ ]` で囲んで見せる。自動比較しない。

---

## 動作環境

**42 Tokyo の校内 PC (iMac / Linux クラスタ) を想定**。`cc` / `make` と libft 一式があれば手元の環境でも動くはず。

- 42 Tokyo の標準環境: `cc` (Apple clang または gcc) と `make` が入っている
- libft の静的ライブラリ `libft.a` を `../Libft_git/` に配置する想定 (Makefile の `LIBFT_DIR` 変数で変更可)
- ライブラリパスはピア評価向けの一般的な構成に合わせている: テスター本体と libft をクローン先で隣り合わせに置いて使う

---

## クイックスタート

```bash
# 1. テスター本体を clone
git clone https://github.com/tumekusa/Libft-tester.git

# 2. libft 本体を Libft_git ディレクトリ名で隣に clone
#    (テスター側の Makefile が ../Libft_git/ を見るので、ディレクトリ名はこの通り)
git clone <あなたの libft リポジトリ URL> Libft_git

# 3. テスターのディレクトリに入って一括ビルド + 実行
cd Libft-tester
make test
```

期待出力 (抜粋。実際の端末では OK が緑、KO が赤、見出し / サマリーがシアンで表示される):

```
========= test_isalnum =========
[1] 'a' (英数字)  expect=非0  ft=1 libc=1  OK
[2] 'Z' (英数字)  expect=非0  ft=1 libc=1  OK
[3] '5' (英数字)  expect=非0  ft=1 libc=1  OK
[4] '!' (英数字でない)  expect=0  ft=0 libc=0  OK
...
----- summary: 9 OK / 0 KO -----
```

`libft.a` は最初の `make` 時に `make -C ../Libft_git` で自動ビルドされる。

---

## 使い方

| コマンド                | 動作                                            |
| ----------------------- | ----------------------------------------------- |
| `make`                  | 全 43 テストをビルド                            |
| `make test`             | 全テストを順に実行                              |
| `make test_strlen`      | 個別ビルド (実行は `./test_strlen`)             |
| `make clean`            | テストバイナリ削除                              |
| `make fclean`           | テストバイナリ + `libft.a` を削除               |
| `make re`               | 完全リビルド                                    |

### 失敗ケースだけ拾う

各テストバイナリは KO がゼロなら `exit 0`、1 つでも KO があれば `exit 1` を返す。失敗だけ眺めたいときは `grep` か exit code を使う:

```bash
# テスト名と KO 行だけ抽出 (シアンの見出し行と赤の KO 行)
make test 2>&1 | grep -E '(========= test_|KO)'

# どのテストが失敗したかを exit code で拾う
make test 2>&1 >/dev/null
for t in test_*; do ./$t >/dev/null || echo "FAIL: $t"; done
```

### valgrind で leak チェック

```bash
valgrind --leak-check=full ./test_split
```

または `-fsanitize=address` を Makefile の `CFLAGS` に足す:

```bash
make CFLAGS='-Wall -Wextra -Werror -g -fsanitize=address' re
./test_split
```

---

## 出力フォーマット

各テストバイナリの出力は **見出し / ケース 1 行 × N / サマリー** の 3 部構成:

```
========= test_isalnum =========           ← 見出し (シアン)
[1] 'a' (英数字)  expect=非0  ft=1 libc=1  OK      ← ケース (OK = 緑、KO = 赤)
[2] 'Z' (英数字)  expect=非0  ft=1 libc=1  OK
[3] '!' (英数字でない)  expect=0  ft=0 libc=0  OK
...
----- summary: 9 OK / 0 KO -----            ← サマリー (シアン)
```

| 要素                                       | 役割                                                         |
| ------------------------------------------ | ------------------------------------------------------------ |
| `========= test_xxx =========`             | テスト名見出し。`make test` でテストが切り替わる目印         |
| `[N]`                                      | ケース番号                                                   |
| 入力の説明                                 | 「`'a'` (英数字)」「`split("hello world", ' ')`」 など       |
| `expect=...`                               | 期待する戻り値 (「非0」「`"hel"`」「NULL」など)              |
| `ft=...`                                   | `ft_xxx` の実際の戻り値                                      |
| `libc=...` または `expect=...`             | 比較対象。`libc` 並走可能なら `libc` の戻り値、対応関数が無い手書き比較系では `expect=...` |
| `OK` (緑) / `KO` (赤)                      | 判定結果                                                     |
| `----- summary: N OK / M KO -----`         | 末尾の集計                                                   |

### 色付け (ANSI escape sequence)

色は `printf` 内に `\033[32m` (緑) / `\033[31m` (赤) / `\033[36m` (シアン) / `\033[0m` (リセット) を埋め込んで実現している。マクロや `#include` 不要、各テストファイルの先頭で `const char *green = "\033[32m";` のような変数として持つだけ。

`\033` (10 進 27 / 16 進 0x1B) は ESC 文字。続く `[xxm` の `xx` が色番号で、ターミナルが対応していれば自動で色付きになる。リダイレクト先 (ファイル) に出すとエスケープシーケンスがそのまま見えるので、必要に応じて `--no-color` 相当を入れたい場合は `green = "";` などに置き換えれば消せる。

### `!!` 正規化 (ctype 系)

`isalpha` などの戻り値は `libc` 実装によって `1`, `8`, `1024` などマジック値が違う。生値で比較すると不一致と判定されてしまうので、`!!` (二重否定) で `0` か `1` に正規化してから比較する:

```c
pass = (!!ft_isalpha(c) == !!isalpha(c));
```

出力表示にも `!!` を反映しているので `ft=1 libc=1` と両者が同じ表示になる。

### 比較系 (`strncmp` / `memcmp`) の符号正規化

`strncmp` の戻り値も実装依存 (`glibc` は差分値、他処理系は `-1 / 0 / 1` だけ等)。各ファイル内の `static int sign_of(int x)` で `-1 / 0 / 1` に正規化してから比較する。`static` 関数なのでファイル外には漏れない。

### `expect=...` の意味

`libc` に対応関数が無いとき (`ft_substr` など) は手書きの期待値を `expect=...` 列に書く:

```
[1] substr("hello", 0, 3)  expect="hel"  ft="hel"  OK
```

### 終了コード

各テストバイナリは末尾で:

```c
if (ko > 0)
    return (1);
return (0);
```

を実行する。シェルから `exit code` で集計できる。ただし fd 出力系 (`test_put*_fd`) は目視確認のみで OK / KO 判定しないため、常に `exit 0` を返す。

---

## 設計方針

### なぜ 1 関数 1 main か

- 1 つが segfault やループに陥っても、他のテストには影響しない (バイナリが独立しているため)。
- `make test_strlen` のように単体ビルドで開発しやすい。
- テストランナーやテスト登録マクロを書かなくていい (関数を main に並べるだけ)。

### なぜ共通ヘルパ (`helper.c`) を使わないのか

- `CHECK` マクロや `summary()` のような共通ヘルパを作ると、`#cond` (プリプロセッサ文字列化演算子) や `extern int` のような **libft 課題では出てこない C のテクニック** が必要になる。「libft が書ける = テスターも読める」を満たすため、共通化を避けた。
- 各テストファイルは `printf` と `if` だけで完結している。冗長だが上から下に読み下せる。

### なぜメモリリークの自動検知をしないのか

- リンカフック (`-Wl,--wrap=malloc`) や `__attribute__((destructor))` を使えば自前で leak 検知も書けるが、いずれも標準 C の世界から外れた仕組みで、初見では何が起きているか追えない。
- leak チェックは `valgrind` または `-fsanitize=address` という世間的に標準のツールがあるので、それに任せる。テスター本体は軽く保つ。

### なぜ fd 出力を目視にしたのか

- `pipe` で fd を捕獲して `strcmp` で比較する方法もあるが、`pipe` / `dup` / `close` の組み合わせと `read` のタイミング管理は libft の手の内訓ではない。
- 代わりに `fd=1` (stdout) にそのまま出力させ、テスト出力中に `[A]` のように囲んで見せる。`fflush(stdout)` を挟んでバッファリングのズレを防ぐ程度の細工で済む。

### なぜ失敗時に続行するのか

- 1 つのテストファイル内で 5〜10 ケース走らせる構成。最初の失敗で `return` してしまうと、後ろのケースが見えない。
- 全部走らせて `grep KO` で拾える方が、修正の見通しが良い。

### なぜ `libc` 並走を使うのか

- `ctype` 系や `strchr` 系は仕様が `libc` と同じはずなので、手で期待値を書く代わりに `libc` 関数の結果と比較する。手書き期待値を書く手間が無くなる。
- `libc` に対応が無い `ft_substr` / `ft_split` / `ft_itoa` / `ft_lst*` などは手書き期待値を持つ。

---

## テストの追加手順

1. **テストファイルを書く**: `tests/test_<関数名>.c` を新規作成。既存のどれかをコピーして書き換えるのが早い。
2. **`Makefile` の `TESTS` リストに 1 行追加**: 関数名 (`test_<関数名>`) を加える。
3. **ビルド**: `make test_<関数名>` で個別ビルド + 実行ファイル生成。

### 最小テンプレ

```c
#include "libft.h"
#include <stdio.h>
// 必要に応じて: <string.h> / <ctype.h> / <stdlib.h> / <limits.h>

int main(void)
{
    int          ok;
    int          ko;
    int          pass;
    const char  *color;
    const char  *label;
    const char  *green;
    const char  *red;
    const char  *cyan;
    const char  *reset;

    /* ANSI escape sequence で色を付ける (詳細は README 参照) */
    green = "\033[32m";
    red   = "\033[31m";
    cyan  = "\033[36m";
    reset = "\033[0m";

    ok = 0;
    ko = 0;
    printf("%s========= test_<関数名> =========%s\n", cyan, reset);

    /* --- ケース 1 --- */
    pass = (/* テスト式 */);
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
    printf("[%d] <入力の説明>  expect=<期待値>  ft=<実値>  %s%s%s\n",
        1, color, label, reset);

    /* --- ケース 2、3 ... 同じ構造で並べる --- */

    printf("%s----- summary: %s%d OK%s / %s%d KO%s%s -----%s\n",
        cyan, green, ok, reset, red, ko, reset, cyan, reset);
    if (ko > 0)
        return (1);
    return (0);
}
```

### 検証パターン別のひな形

**`libc` 並走 (ctype 系)** — `!!` でブール正規化:

```c
c = 'a';
pass = (!!ft_isalpha(c) == !!isalpha(c));
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
printf("[%d] 'a' (小文字)  expect=非0  ft=%d libc=%d  %s%s%s\n",
    1, !!ft_isalpha(c), !!isalpha(c), color, label, reset);
```

**手書き期待値 (alloc 系)** — `strcmp` で文字列比較、最後に `free`:

```c
r = ft_substr("hello", 0, 3);
pass = (r != NULL && strcmp(r, "hel") == 0);
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
printf("[%d] substr(\"hello\", 0, 3)  expect=\"hel\"  ft=\"%s\"  %s%s%s\n",
    1, r ? r : "(NULL)", color, label, reset);
free(r);
```

**fd 目視 (put*_fd)** — OK / KO 判定なし、stdout に出して目視:

```c
printf("[1] putchar_fd('A', 1)  expect=A  ft=[");
fflush(stdout);
ft_putchar_fd('A', 1);
printf("]  (目視確認)\n");
```

fd 系では `ok` / `ko` カウンタを使わず、サマリーは `printf("%s----- summary: N ケース (目視確認のみ) -----%s\n", cyan, reset);` で出す。

---

## テスト方針一覧

各関数のテストで何を見ているかの早見表。`手段` は検証スタイル、`主なケース` は実装ファイルで確認しているケースの要約。

### ctype 系 (Part 1)

| 関数         | 手段                | 主なケース                                       |
| ------------ | ------------------- | ------------------------------------------------ |
| `ft_isalpha` | `libc` 並走 + `!!`  | 'a' 'Z' / '5' '!' ' ' / 128 / -1                 |
| `ft_isdigit` | 同上                | '0' '9' / '/' ':' 'a' / 128 / -1                 |
| `ft_isalnum` | 同上                | 'a' 'Z' '5' / '!' ' ' '\0' / 128 255 / -1        |
| `ft_isascii` | 仕様式 `0..127` 並走| 0 127 'A' / 128 255 / -1                         |
| `ft_isprint` | `libc` 並走         | ' ' '~' 'A' / '\t' 127 128 / -1                  |
| `ft_toupper` | `libc` 並走 (生値)  | 'a' 'z' / 'A' / '5' '!'                          |
| `ft_tolower` | 同上                | 'A' 'Z' / 'a' / '5' '@'                          |

### 文字列長・コピー (Part 1)

| 関数         | 手段                  | 主なケース                                                |
| ------------ | --------------------- | --------------------------------------------------------- |
| `ft_strlen`  | `libc` 並走           | "" "a" "hello" / "hello, world" / 途中 NUL                |
| `ft_strdup`  | `strcmp`              | "hello" / "" / 元配列と独立 / 長め                        |
| `ft_strlcpy` | 戻り値 + `strcmp`     | 収まる / 切り詰め / size=1 / size=0 / 空 src              |
| `ft_strlcat` | 同上                  | 通常 / 切り詰め / size=0 / dst 非終端 / 空 src            |

### メモリ操作 (Part 1)

| 関数         | 手段                  | 主なケース                                       |
| ------------ | --------------------- | ------------------------------------------------ |
| `ft_memset`  | `libc` 並走 + `memcmp`| 'A' 5 / n=0 / c=256+'A' / 戻り値                 |
| `ft_bzero`   | 同上                  | 16 全部 / 部分 / n=0                             |
| `ft_memcpy`  | 同上                  | 5 / NUL 含む 14 / n=0 / 戻り値                   |
| `ft_memmove` | 同上                  | 非重複 / 前方 overlap / 後方 overlap / n=0       |

### 文字列探索・比較 (Part 1)

| 関数         | 手段                  | 主なケース                                                |
| ------------ | --------------------- | --------------------------------------------------------- |
| `ft_strchr`  | `libc` 並走           | 'l' 'h' / 'z' / '\0' / 空文字列 / 高位ビット              |
| `ft_strrchr` | 同上                  | 最後の 'l' / 'o' 'h' / 'z' / '\0'                         |
| `ft_strncmp` | `libc` 並走 (符号)    | 等しい / 大小 / 短い prefix / 0x80                        |
| `ft_memchr`  | `libc` 並走           | 通常 / NUL 越え / NUL バイト / 見つからない / n=0         |
| `ft_memcmp`  | `libc` 並走 (符号)    | 等しい / 大小 / prefix / NUL 越え                         |
| `ft_strnstr` | 戻り値                | 通常 / 空 needle / 見つからない / 切り詰め / ちょうど収まる |

### 数値変換・確保 (Part 1)

| 関数         | 手段                  | 主なケース                                                       |
| ------------ | --------------------- | ---------------------------------------------------------------- |
| `ft_atoi`    | `libc` 並走           | 0 / 123 / -42 / +42 / whitespace / 数字後文字 / 二重符号 / INT_MAX / INT_MIN / overflow |
| `ft_calloc`  | 戻りバッファの 0 確認 | size=10 / int×5 / nmemb=0 / overflow×2                           |

### 文字列加工 (Part 2)

| 関数         | 手段                  | 主なケース                                                            |
| ------------ | --------------------- | --------------------------------------------------------------------- |
| `ft_substr`  | `strcmp`              | 先頭 / 途中 / len 過大 / start 過大 / len=0 / NULL                    |
| `ft_strjoin` | `strcmp`              | 通常 / 空 s1 / 空 s2 / 両方空 / NULL                                  |
| `ft_strtrim` | `strcmp`              | 空白 / 'x' trim / 複数文字 / 全 trim / 空 set / 内側残し / NULL       |
| `ft_split`   | 単語数 + `strcmp`     | 通常 / 連続区切り / 先頭末尾 / 全区切り / 空 / NULL                   |
| `ft_itoa`    | `strcmp`              | 0 / 42 / -42 / 多桁 / INT_MAX / INT_MIN                               |
| `ft_strmapi` | `strcmp`              | index 足し / 大文字化 / 空 / NULL s / NULL f                          |
| `ft_striteri`| `strcmp`              | 大文字化 / index 足し / 空 / NULL s                                   |

### fd 出力 (Part 2)

| 関数            | 手段          | 主なケース                          |
| --------------- | ------------- | ----------------------------------- |
| `ft_putchar_fd` | stdout 目視   | 'A' / ' ' / '\n' / '7'              |
| `ft_putstr_fd`  | stdout 目視   | "hello" / "" / NULL / 改行 / 長め   |
| `ft_putendl_fd` | stdout 目視   | "hello" / "" / NULL / 長め          |
| `ft_putnbr_fd`  | stdout 目視   | 0 / 42 / -42 / INT_MAX / INT_MIN    |

### 連結リスト (Part 3)

| 関数              | 手段                       | 主なケース                                                       |
| ----------------- | -------------------------- | ---------------------------------------------------------------- |
| `ft_lstnew`       | フィールド確認             | 文字列 / NULL / 任意ポインタ                                     |
| `ft_lstadd_front` | ポインタ比較               | 空に追加 / 先頭追加 / 3 つ追加 / NULL new                        |
| `ft_lstsize`      | 数値比較                   | NULL / 1〜5 ノード                                               |
| `ft_lstlast`      | ポインタ比較               | NULL / 1 ノード / 3 ノード                                       |
| `ft_lstadd_back`  | ポインタ比較               | 空 / 末尾追加 / 3 つ / NULL new                                  |
| `ft_lstdelone`    | spy パターン (呼び出し記録) | 通常 / NULL ノード / NULL del                                    |
| `ft_lstclear`     | spy パターン               | 3 ノード / 空 / NULL del                                         |
| `ft_lstiter`      | spy パターン (`g_buf`)     | 3 ノード / NULL lst / NULL f                                     |
| `ft_lstmap`       | 新リストを `strcmp`        | 通常 / 原リスト不変 / NULL lst / NULL f / NULL del               |

---

## 既知の限界

- **メモリリークの自動検知はしない**: `valgrind` または `-fsanitize=address` で別途確認する。
- **fd 出力は目視確認**: `ft_put*_fd` 系は自動比較せず、テスト出力中に `[A]` のように囲んで見せるだけ。
- **ケースは完全網羅ではない**: 各関数 3〜10 ケース程度。実装方針が違う libft では追加・修正が必要。
- **マルチスレッド / シグナルは想定外**: libft 課題に含まれないため。
- **`mac` (`musl` 等の非 glibc 環境) では `ft_atoi` overflow 等の挙動が違う可能性**: glibc 互換前提のケースがある (詳細は次節)。

---

## 想定する libft 側の方針

このテスターは特定の libft 実装方針に合わせている。**異なる実装方針の libft をテストする場合、該当ケースの修正が必要**。

### 不正入力 (NULL ポインタ) は NULL / no-op で安全側

`ft_substr` / `ft_strjoin` / `ft_strtrim` / `ft_split` / `ft_strmapi` / `ft_striteri` / `ft_putstr_fd` / `ft_putendl_fd` 等は **入力が NULL なら NULL を返す or 何もしない**。

`ft_lstmap` / `ft_lstiter` / `ft_lstclear` / `ft_lstdelone` は **関数ポインタ (`del` / `f`) が NULL なら NULL / no-op**。

`subject` 規定にないものも含むが、本テスターはこの方針前提でケースを書いている。NULL 入力で segfault するのが正解な libft では、関連ケースを削除する必要がある。

### `ft_atoi` は glibc 互換のオーバーフロー処理

オーバーフロー時に `(int)LONG_MAX` (`= -1`) や `(int)LONG_MIN` (`= 0`) のキャスト値を返す。`INT_MAX` / `INT_MIN` でクランプする実装ではない。

テストでは `"9999999999999999999"` を渡して `libc atoi()` と一致するかを見ている。`musl` 等の非 glibc 環境では結果が変わる可能性がある。

### `ft_strlcat` の `dest_size` は size 内で打ち切るループ

`ft_strlen(dest)` ではなく size 内で打ち切るループで `dest_size` を取得する。dst が size 内で NUL 終端されていない場合は `size + src_len` を返す (man 規定通り)。

### `ft_calloc` 関連

`nmemb=0` または `size=0` のとき、`malloc` 経由で 0 バイト確保した「free に渡せる値」を返す。`NULL` も `非NULL` も許容 (落ちなければ OK としている)。

---

## ディレクトリ構成

```
libft_tester/
├── README.md           ← このファイル
├── Makefile            ← ビルド・実行
├── LICENSE             ← MIT
├── .gitignore
└── tests/
    ├── test_isalpha.c   ← 1 関数 1 main、ベタ書きテスト
    ├── test_isdigit.c
    ├── ...              ← 全 43 ファイル
    └── test_lstmap.c
```

`../Libft_git/` に対象の libft を置いて、そこから `libft.a` と `libft.h` を引く構成。`LIBFT_DIR` を Makefile で変更すれば別ディレクトリでも可。

---

## おまけ: 全テストケース一覧

各 `tests/test_*.c` に実装してあるテストケースの全件リスト。「このテスター何見てるの?」の細部を知りたい人向け。

### ctype 系

#### `test_isalpha` (7 ケース)

1. `'a'` (小文字) → 非0
2. `'Z'` (大文字) → 非0
3. `'5'` (数字、アルファベットでない) → 0
4. `'!'` (記号) → 0
5. `' '` (空白) → 0
6. `128` (ASCII 範囲外) → 0
7. `-1` (EOF) → 0

#### `test_isdigit` (7 ケース)

1. `'0'` (数字) → 非0
2. `'9'` (数字) → 非0
3. `'/'` (`'0'` のひとつ前) → 0
4. `':'` (`'9'` のひとつ後) → 0
5. `'a'` (アルファベット) → 0
6. `128` (ASCII 範囲外) → 0
7. `-1` (EOF) → 0

#### `test_isalnum` (9 ケース)

1. `'a'` (英数字) → 非0
2. `'Z'` (英数字) → 非0
3. `'5'` (英数字) → 非0
4. `'!'` (英数字でない) → 0
5. `' '` (英数字でない) → 0
6. `'\0'` (NUL、英数字でない) → 0
7. `128` (ASCII 範囲外) → 0
8. `255` (ASCII 範囲外 上限) → 0
9. `-1` (EOF) → 0

#### `test_isascii` (6 ケース)

1. `0` (ASCII 下限) → 非0
2. `127` (ASCII 上限) → 非0
3. `'A'` (ASCII 範囲内) → 非0
4. `128` (ASCII 範囲外) → 0
5. `255` (ASCII 範囲外) → 0
6. `-1` (負値) → 0

#### `test_isprint` (7 ケース)

1. `' '` (印字可能の下限) → 非0
2. `'~'` (印字可能の上限) → 非0
3. `'A'` → 非0
4. `'\t'` (制御文字) → 0
5. `127` (DEL、制御文字) → 0
6. `128` (ASCII 範囲外) → 0
7. `-1` (EOF) → 0

#### `test_toupper` (5 ケース)

1. `'a'` (小文字) → `'A'`
2. `'z'` (小文字) → `'Z'`
3. `'A'` (既に大文字、そのまま) → `'A'`
4. `'5'` (アルファベット外) → `'5'`
5. `'!'` (記号) → `'!'`

#### `test_tolower` (5 ケース)

1. `'A'` (大文字) → `'a'`
2. `'Z'` (大文字) → `'z'`
3. `'a'` (既に小文字、そのまま) → `'a'`
4. `'5'` (アルファベット外) → `'5'`
5. `'@'` (記号) → `'@'`

### 文字列長・コピー系

#### `test_strlen` (5 ケース)

1. `""` (空文字列) → 0
2. `"a"` (1 文字) → 1
3. `"hello"` → 5
4. `"hello, world"` (空白含む) → 12
5. `"ab\0cd"` (途中 NUL、そこで打ち切り) → 2

#### `test_strdup` (4 ケース)

1. `"hello"` → `"hello"`
2. `""` (空文字列) → 非NULL の `""`
3. `src=\"mutable\"` を dup 後に `src[0]='X'` → コピー側は `"mutable"` のまま
4. `"the quick brown fox jumps over the lazy dog"` → 同じ文字列

#### `test_strlcpy` (5 ケース)

1. `src="hello"`, `size=16` → 戻り 5 / `dst="hello"`
2. `src="hello"`, `size=3` (切り詰め) → 戻り 5 / `dst="he"`
3. `size=1` (NUL だけ) → 戻り 5 / `dst=""`
4. `size=0` (dst 触らない) → 戻り 5 / `dst` 不変
5. 空 src → 戻り 0 / `dst=""`

#### `test_strlcat` (5 ケース)

1. `dst="he"`, `src="llo"`, `size=16` → 戻り 5 / `dst="hello"`
2. `dst="he"`, `src="llo"`, `size=4` (切り詰め) → 戻り 5 / `dst="hel"`
3. `dst="he"`, `src="llo"`, `size=0` → 戻り 3 / `dst="he"`
4. `dst="abcde"` (size 内で NUL 終端なし), `size=5`, `src="z"` → 戻り 6 / `dst` 不変
5. 空 src → 戻り 5 / `dst="hello"`

### メモリ操作系

#### `test_memset` (4 ケース)

1. buf を `'A'` で 5 バイト埋める → libc `memset` と一致
2. 長さ 0 → buf 変化なし
3. `c=256+'A'` (高位ビット切り捨て) → `'A'` で埋まる
4. 戻り値 → buf 自身のアドレス

#### `test_bzero` (3 ケース)

1. 全 16 バイトを 0 クリア (元 `0xAA`) → 全 0
2. 先頭 5 バイトだけ 0、6〜15 は `0xAA` のまま
3. `n=0` → 何もしない

#### `test_memcpy` (4 ケース)

1. src の先頭 5 バイトコピー → libc `memcpy` と一致
2. NUL 含む 14 バイトコピー
3. `n=0` → 何もコピーしない
4. 戻り値 → dst 自身のアドレス

#### `test_memmove` (5 ケース)

1. 重ならない領域 (buf+5 に buf 先頭 5 バイト)
2. 前方 overlap → `"0101234789"`
3. 後方 overlap → `"2345656789"`
4. `n=0`
5. 戻り値 → dst 自身

### 文字列探索・比較系

#### `test_strchr` (6 ケース)

1. `strchr("hello, world!", 'l')` → s+2
2. `strchr(s, 'h')` → s+0
3. `strchr(s, 'z')` (見つからない) → NULL
4. `strchr(s, '\0')` → s+13
5. `strchr("", '\0')` (空文字列で NUL) → s 自身
6. `strchr(s, 256+'h')` (高位ビット) → s+0

#### `test_strrchr` (5 ケース)

1. `strrchr(s, 'l')` → 最後の 'l' (s+10)
2. `strrchr(s, 'o')` → world の 'o' (s+8)
3. `strrchr(s, 'h')` → s+0
4. `strrchr(s, 'z')` (見つからない) → NULL
5. `strrchr(s, '\0')` → 終端位置 (s+13)

#### `test_strncmp` (6 ケース)

1. `"hello"` vs `"hello"`, n=5 → 符号 0
2. `"abc"` vs `"abd"`, n=3 → 符号 -1
3. `"abd"` vs `"abc"`, n=3 → 符号 +1
4. `"abc"` vs `"abd"`, n=2 (prefix 一致) → 符号 0
5. `""` vs `""`, n=0 → 符号 0
6. `"\x80"` vs `"\x01"`, n=1 (unsigned char で比較) → 符号 +1

#### `test_memchr` (6 ケース)

1. `memchr("hello\0world", 'h', 11)` → s+0
2. `memchr(s, 'w', 11)` (NUL 越え) → s+6
3. `memchr(s, '\0', 11)` (NUL バイトを探す) → s+5
4. `memchr(s, 'z', 11)` (見つからない) → NULL
5. `memchr(s, 'h', 0)` (範囲 0) → NULL
6. `memchr(s, 'w', 6)` (範囲外) → NULL

#### `test_memcmp` (5 ケース)

1. `"hello"` vs `"hello"`, n=5 → 符号 0
2. `"hello"` vs `"hellx"`, n=5 → 符号 -1
3. `"hello"` vs `"hellx"`, n=4 (prefix 一致) → 符号 0
4. `"ab\0cd"` vs `"ab\0ce"`, n=5 (NUL 越え) → 符号 -1
5. `n=0` → 符号 0

#### `test_strnstr` (6 ケース)

1. `strnstr("hello world foo", "world", 15)` → big+6
2. `strnstr(big, "hello", 15)` → big+0
3. `strnstr(big, "", 15)` (空 needle) → big 自身
4. `strnstr(big, "xyz", 15)` (見つからない) → NULL
5. `strnstr(big, "world", 6)` (n 不足) → NULL
6. `strnstr(big, "world", 11)` (ちょうど収まる) → big+6

### 数値変換・確保系

#### `test_atoi` (10 ケース)

1. `"0"` → 0
2. `"123"` → 123
3. `"-42"` (負の数) → -42
4. `"  +42"` (空白 + 符号) → 42
5. `"\t\n\v\f\r 42"` (全 whitespace + 数字) → 42
6. `"123abc"` (数字以外で打ち切り) → 123
7. `"+-123"` (二重符号、変換が始まらない) → 0
8. `"2147483647"` (INT_MAX) → 2147483647
9. `"-2147483648"` (INT_MIN) → -2147483648
10. `"9999999999999999999"` (overflow) → glibc 互換 `(int)LONG_MAX` のキャスト値

#### `test_calloc` (5 ケース)

1. `ft_calloc(10, sizeof(char))` → 非NULL、10 バイト全部 0
2. `ft_calloc(5, sizeof(int))` → 非NULL、5 要素全部 0
3. `ft_calloc(0, sizeof(int))` (nmemb=0) → 落ちないこと
4. `ft_calloc((size_t)-1, 2)` (overflow) → NULL
5. `ft_calloc(2, (size_t)-1)` (overflow) → NULL

### 文字列加工系

#### `test_substr` (7 ケース)

1. `ft_substr("hello", 0, 3)` → `"hel"`
2. `ft_substr("hello", 1, 3)` → `"ell"`
3. `ft_substr("hello", 0, 100)` (len 過大) → `"hello"`
4. `ft_substr("hello", 5, 3)` (start が長さと同じ) → `""`
5. `ft_substr("hello", 100, 3)` (start 範囲外) → `""`
6. `ft_substr("hello", 1, 0)` (len=0) → `""`
7. `ft_substr(NULL, 0, 5)` → NULL

#### `test_strjoin` (6 ケース)

1. `ft_strjoin("hello", " world")` → `"hello world"`
2. `ft_strjoin("", "foo")` (空 s1) → `"foo"`
3. `ft_strjoin("foo", "")` (空 s2) → `"foo"`
4. `ft_strjoin("", "")` (両方空) → `""`
5. `ft_strjoin(NULL, "x")` → NULL
6. `ft_strjoin("x", NULL)` → NULL

#### `test_strtrim` (7 ケース)

1. `ft_strtrim("  hello  ", " ")` → `"hello"`
2. `ft_strtrim("xxhelloxx", "x")` → `"hello"`
3. `ft_strtrim(" \t\nhello\n\t ", " \t\n")` (複数文字 set) → `"hello"`
4. `ft_strtrim("aaaa", "a")` (全部 trim 対象) → `""`
5. `ft_strtrim("aabaa", "a")` (内側は触らない) → `"b"`
6. `ft_strtrim("hello", "")` (空 set) → `"hello"`
7. `ft_strtrim(NULL, "a")` → NULL

#### `test_split` (6 ケース)

1. `ft_split("hello world foo", ' ')` → 3 単語 `{"hello","world","foo"}`
2. `ft_split("a,,b,,,c", ',')` (連続区切り) → 3 単語 `{"a","b","c"}`
3. `ft_split(",,hello,,", ',')` (先頭末尾区切り) → 1 単語 `{"hello"}`
4. `ft_split(",,,", ',')` (全部区切り) → 0 単語、NULL 終端だけ
5. `ft_split("", ',')` (空文字列) → 0 単語
6. `ft_split(NULL, ',')` → NULL

#### `test_itoa` (6 ケース)

1. `ft_itoa(0)` → `"0"`
2. `ft_itoa(42)` → `"42"`
3. `ft_itoa(-42)` → `"-42"`
4. `ft_itoa(123456)` → `"123456"`
5. `ft_itoa(INT_MAX)` → `"2147483647"`
6. `ft_itoa(INT_MIN)` → `"-2147483648"` (overflow しない実装か?)

#### `test_strmapi` (5 ケース)

1. `ft_strmapi("abc", add_index)` (`i` を足す) → `"ace"`
2. `ft_strmapi("hello", to_upper_cb)` → `"HELLO"`
3. `ft_strmapi("", add_index)` (空文字列) → 非NULL の `""`
4. `ft_strmapi(NULL, add_index)` → NULL
5. `ft_strmapi("anything", NULL)` → NULL

#### `test_striteri` (4 ケース)

1. `ft_striteri("hello", upper_at)` (in-place 大文字化) → `"HELLO"`
2. `ft_striteri("abc", add_index_at)` → `"ace"`
3. `ft_striteri("", upper_at)` (空文字列) → `""`
4. `ft_striteri(NULL, upper_at)` → 落ちないこと

### fd 出力系 (目視)

#### `test_putchar_fd` (4 ケース)

1. `ft_putchar_fd('A', 1)` → `A` が表示される
2. `ft_putchar_fd(' ', 1)` → `[ ]` のように空白 1 文字
3. `ft_putchar_fd('\n', 1)` → 改行
4. `ft_putchar_fd('7', 1)` → `7`

#### `test_putstr_fd` (5 ケース)

1. `ft_putstr_fd("hello", 1)` → `hello`
2. `ft_putstr_fd("", 1)` (空文字列) → 何も出ない
3. `ft_putstr_fd(NULL, 1)` → 何も出ない (落ちない)
4. `ft_putstr_fd("ab\ncd", 1)` (改行透過) → 2 行で表示
5. `ft_putstr_fd("the quick brown fox", 1)` → 全文

#### `test_putendl_fd` (4 ケース)

1. `ft_putendl_fd("hello", 1)` → `hello` + 改行
2. `ft_putendl_fd("", 1)` (空文字列) → 改行 1 つだけ
3. `ft_putendl_fd(NULL, 1)` → 何も出ない (改行も出ない)
4. `ft_putendl_fd("the quick brown fox", 1)` → 全文 + 改行

#### `test_putnbr_fd` (5 ケース)

1. `ft_putnbr_fd(0, 1)` → `0`
2. `ft_putnbr_fd(42, 1)` → `42`
3. `ft_putnbr_fd(-42, 1)` → `-42`
4. `ft_putnbr_fd(INT_MAX, 1)` → `2147483647`
5. `ft_putnbr_fd(INT_MIN, 1)` → `-2147483648` (overflow しない実装か?)

### 連結リスト系

#### `test_lstnew` (3 ケース)

1. `ft_lstnew("hello")` → 非NULL、`content == 渡したポインタ`、`next == NULL`
2. `ft_lstnew(NULL)` → 非NULL ノード、`content == NULL`
3. `ft_lstnew((void *)0x1234)` (任意のポインタ) → `content == 0x1234`

#### `test_lstadd_front` (4 ケース)

1. 空 `lst` に `a` を追加 → `lst == a`
2. 既存先頭 (`a`) に `b` を追加 → `lst == b`, `lst->next == a`
3. さらに `c` を追加 → `c → b → a` の順
4. NULL new → `lst` は変わらない

#### `test_lstsize` (2 ケース)

1. `ft_lstsize(NULL)` → 0
2. 1〜5 ノード追加するたびに `ft_lstsize` が一致

#### `test_lstlast` (3 ケース)

1. `ft_lstlast(NULL)` → NULL
2. 1 ノードしかない → 自分自身
3. `a → b → c` の 3 ノード → `c` が返る

#### `test_lstadd_back` (4 ケース)

1. 空 `lst` に `a` → `lst == a`
2. 末尾 (`a`) に `b` → `a → b`
3. さらに `c` を末尾追加 → `a → b → c`
4. NULL new → 不変

#### `test_lstdelone` (3 ケース)

1. 通常 → `del` が 1 回呼ばれ、引数は `n->content`
2. `ft_lstdelone(NULL, spy_del)` → `del` は呼ばれない
3. NULL `del` → 何もしない (ノードが残るので自分で `free`)

#### `test_lstclear` (3 ケース)

1. 3 ノードを clear → `del` が 3 回呼ばれ、`lst == NULL` になる
2. 空リスト → `del` 呼ばれない、`lst == NULL`
3. NULL `del` → `del` 呼ばれず、`lst` 変化なし

#### `test_lstiter` (3 ケース)

1. 3 ノード `a('A') → b('B') → c('C')` で `g_buf` に `"ABC"` が積まれる
2. NULL `lst` → `f` 呼ばれない
3. NULL `f` → `f` 呼ばれず、落ちないこと

#### `test_lstmap` (5 ケース)

1. 通常 → 新リストの 3 ノードが `"HELLO"`, `"WORLD"`, `"FOO"`
2. 原リスト `src` は変更されていない
3. `ft_lstmap(NULL, f, del)` → NULL
4. NULL `f` → NULL
5. NULL `del` → NULL

---

合計 **43 関数 / 219 ケース**。各関数 3〜10 ケース程度の薄さで、libft の主要な仕様と方針をひととおり拾う構成。

---

## 参考にしたテスター

設計を検討するにあたり、以下の既存テスターを clone して参考にした:

- [Tripouille/libftTester](https://github.com/Tripouille/libftTester)
- [alelievr/libft-unit-test](https://github.com/alelievr/libft-unit-test)
- [0x050f/libft-war-machine](https://github.com/0x050f/libft-war-machine)
- [usatie/libft-tester-tokyo](https://github.com/usatie/libft-tester-tokyo)
- [bzalugas/libft-breaker](https://github.com/bzalugas/libft-breaker)
- [jtoty/Libftest](https://github.com/jtoty/Libftest)

本テスターは「libft が書ける人なら全部読める / 自力で改変できる」ことを目標にしているため、これらに比べてカバレッジは控えめ。網羅性の高いテストが欲しい場合は上記が選択肢になる。

---

## コメント

とにかくわかりやすさと個人用に作成しています。できる方から見ればかなり変な設計になっていると思いますがご容赦頂ければ幸いです。
ほぼ自分のmain関数をベースに作成しています。
また、このテスターの作成には一部AIを使用しています。

---

## ライセンス

[MIT License](LICENSE)。著作権表示と LICENSE 本文を含めれば、自由に参考・流用・改変・再配布してよい。

なお、テスト対象である `libft` 本体は別リポジトリの管理下にあり、本リポジトリには含まれない。`libft` 本体のライセンスは各自のリポジトリに従う。

# libft_tester 技術解説

## 想定読者

- テスター本体のコードを読みたい人
- 自分の libft 実装に合わせてテストを **改変** したい人
- 別実装のテストにこのテスターを当てたい人
- テストが落ちたときに「何を疑えばいいか」のヒントが欲しい人

入口ドキュメント (動作確認や基本的な使い方) は [README.md](README.md) を参照。

---

## 目次

- [設計方針 (がっつり)](#設計方針-がっつり)
- [想定する libft 側の方針](#想定する-libft-側の方針)
- [libft 関数への依存マップ (lst 系)](#libft-関数への依存マップ-lst-系)
- [テストの改変ガイドライン](#テストの改変ガイドライン)
- [出力フォーマットの細部](#出力フォーマットの細部)
- [実行結果の扱い](#実行結果の扱い)
- [テストの追加手順](#テストの追加手順)
- [テスト方針一覧](#テスト方針一覧)
- [全テストケース一覧 + 想定ミス](#全テストケース一覧--想定ミス)
- [おまけ: テストケースの小ネタ解説](#おまけ-テストケースの小ネタ解説)
- [既知の限界](#既知の限界)
- [更新履歴](#更新履歴)

---

## 設計方針 (がっつり)

### なぜ 1 関数 1 main か

- 1 つが segfault やループに陥っても、他のテストには影響しない (バイナリが独立しているため)
- `make test_strlen` のように単体ビルドで開発しやすい
- テストランナーやテスト登録マクロを書かなくていい (関数を main に並べるだけ)
- 1 ファイル内のケース順序を入れ替えても他バイナリの結果には影響しない (独立性)

### なぜ共通ヘルパ (`helper.c`) を使わないのか

- `CHECK` マクロや `summary()` のような共通ヘルパを作ると、`#cond` (プリプロセッサ文字列化演算子) や `extern int` のような **libft 課題では出てこない C のテクニック** が必要になる
- 「libft が書ける = テスターも読める」を満たすため、共通化を避けた
- 各テストファイルは `printf` と `if` だけで完結している。冗長だが上から下に読み下せる
- 「自分でテストを足したい」と思った 42 生が、テンプレをコピペして書ける状態を維持

### なぜマクロを使わないのか

- `#define CHECK(x) ...` のような形は便利だが、`__LINE__` / `#x` などプリプロセッサ機能の理解が前提
- 短くなる代わりに「何が展開されるか」を読み解く手間が増える
- ベタ書きの方が `if (pass) { color = green; ... } else { ... }` で意図が明示的

### なぜメモリリーク自動検知をしないのか

- リンカフック (`-Wl,--wrap=malloc`) や `__attribute__((destructor))` を使えば自前で leak 検知も書けるが、いずれも標準 C の世界から外れた仕組みで、初見では何が起きているか追えない
- leak チェックは `valgrind` または `-fsanitize=address` という世間的に標準のツールがあるので、それに任せる
- テスター本体は軽く保つ

### なぜ fd 出力を目視にしたのか

- `pipe` で fd を捕獲して `strcmp` で比較する方法もあるが、`pipe` / `dup` / `close` の組み合わせと `read` のタイミング管理は libft の手の内訓ではない
- 代わりに `fd=1` (stdout) にそのまま出力させ、テスト出力中に `[A]` のように囲んで見せる
- `fflush(stdout)` を挟んでバッファリングのズレを防ぐ程度の細工で済む

### なぜ失敗時に続行するのか

- 1 つのテストファイル内で 5〜10 ケース走らせる構成。最初の失敗で `return` してしまうと、後ろのケースが見えない
- 全部走らせて `grep KO` で拾える方が、修正の見通しが良い
- 失敗の場所を 1 度のテスト実行で全部見渡せる

### なぜ `libc` 並走を使うのか

- `ctype` 系や `strchr` 系は仕様が `libc` と同じはずなので、手で期待値を書く代わりに `libc` 関数の結果と比較する
- 手書き期待値を書く手間が無くなる
- 実装ロケールによる差異 (`ctype` 系の戻り値が `0` / `1` / `8` / `1024` などマジック値) は `!!` 正規化で吸収
- `libc` に対応が無い `ft_substr` / `ft_split` / `ft_itoa` / `ft_lst*` などは手書き期待値を持つ

### なぜ lst 系はスタックノード優先か

- `ft_lstnew` 経由でリストを組むと、**`ft_lstnew` 自体に bug があったときに他の lst テストが連鎖して KO** になる
- 解決策: `t_list nodes[N]` をスタック上に置き、`content` / `next` を手で書き込む
- これで各 lst テストは「テスト対象の関数 + t_list 構造体定義」だけに依存する
- `ft_lstdelone` / `ft_lstclear` は `free` を呼ぶ仕様なので **スタックノードでは使えない** (`free(&stack_var)` は UB)。これだけは `ft_lstnew` を使う

### なぜ不正入力 (NULL ポインタ) を NULL / no-op 前提にしているか

- 多くの libft 実装が「NULL 入力 → NULL 返却 or no-op」方針を採用しているのに合わせている
- subject に明記はないが、評価者から「NULL でも落ちないように」と指摘されるケースが多い
- 別方針 (NULL で segfault が正解) の libft をテストする場合は、該当ケースの削除 or 修正が必要

---

## 想定する libft 側の方針

このテスターは特定の libft 実装方針に合わせている。**異なる実装方針の libft をテストする場合、該当ケースの修正が必要**。

### 不正入力 (NULL ポインタ) は NULL / no-op で安全側

`ft_substr` / `ft_strjoin` / `ft_strtrim` / `ft_split` / `ft_strmapi` / `ft_striteri` / `ft_putstr_fd` / `ft_putendl_fd` 等は **入力が NULL なら NULL を返す or 何もしない**。

`ft_lstmap` / `ft_lstiter` / `ft_lstclear` / `ft_lstdelone` は **関数ポインタ (`del` / `f`) が NULL なら NULL / no-op**。

subject 規定にないものも含むが、本テスターはこの方針前提でケースを書いている。NULL 入力で segfault するのが正解な libft では、関連ケースを削除する必要がある。

### `ft_atoi` は glibc 互換のオーバーフロー処理

オーバーフロー時に `(int)LONG_MAX` (`= -1`) や `(int)LONG_MIN` (`= 0`) のキャスト値を返す。`INT_MAX` / `INT_MIN` でクランプする実装ではない。

テストでは `"9999999999999999999"` を渡して `libc atoi()` と一致するかを見ている。`musl` 等の非 glibc 環境では結果が変わる可能性がある。

### `ft_strlcat` の `dest_size` は size 内で打ち切るループ

`ft_strlen(dest)` ではなく size 内で打ち切るループで `dest_size` を取得する。dst が size 内で NUL 終端されていない場合は `size + src_len` を返す (man 規定通り)。

### `ft_calloc` 関連

`nmemb=0` または `size=0` のとき、`malloc` 経由で 0 バイト確保した「`free` に渡せる値」を返す。`NULL` も `非NULL` も許容 (落ちなければ OK としている)。

### `ft_substr` の `start == strlen(s)` 挙動

subject に明記がないが、本テスターは **空文字列 `""` を返す前提** でケースを書いている。`NULL` を返す実装でテストするとケース 4 (`ft_substr("hello", 5, 3)`) が KO になる。

### `ft_split` で全区切り入力 `",,,"` の挙動

subject に明記がないが、本テスターは **空配列 (NULL 終端だけの `{NULL}`) を返す前提** でケースを書いている。`NULL` を返す実装ではケース 4 (`ft_split(",,,", ',')`) が KO になる。

subject の「The returned array must be NULL terminated」を素直に読むと、空配列でも先頭の NULL 終端は必要なので、`{NULL}` 返しの方が仕様整合性は高い。

### `ft_strchr` / `ft_strrchr` で `c = '\0'` を渡したときの挙動

man 仕様通り、**文字列終端の `'\0'` を指すポインタを返す**。`NULL` を返してしまう実装は man 違反。

### 文字分類関数の高位ビット引数 (`c = 128` / `255`)

ASCII 範囲外の引数で `libc` 並走比較しているケースがある。**42 PC (iMac / Linux クラスタの UTF-8 ロケール) 想定**。Latin-1 系 8-bit ロケールでは libc が non-zero を返すため失敗する可能性がある。

---

## libft 関数への依存マップ (lst 系)

各 lst テストがどの libft 関数に依存しているかの一覧。スタックノード化により、テスト対象以外への依存を最小化している。

| テスト | テスト対象関数 | スタックノード | 他の libft 関数 |
| --- | --- | --- | --- |
| `test_lstnew` | `ft_lstnew` | (使わない) | なし |
| `test_lstadd_front` | `ft_lstadd_front` | ✅ `nodes[3]` | なし |
| `test_lstsize` | `ft_lstsize` | ✅ `nodes[5]` | なし |
| `test_lstlast` | `ft_lstlast` | ✅ `nodes[4]` | なし |
| `test_lstadd_back` | `ft_lstadd_back` | ✅ `nodes[3]` | なし |
| `test_lstdelone` | `ft_lstdelone` | ❌ malloc 必須 | `ft_lstnew` |
| `test_lstclear` | `ft_lstclear` | ❌ malloc 必須 | `ft_lstnew` |
| `test_lstiter` | `ft_lstiter` | ✅ `nodes[3]` | なし |
| `test_lstmap` | `ft_lstmap` | ✅ 原リスト側 `nodes[5]` | なし (新リストは `ft_lstmap` 自身が malloc) |

`test_lstdelone` / `test_lstclear` が malloc 必須な理由: テスト対象の関数が `free(node)` を呼ぶ仕様。スタックノードのアドレスを `free` に渡すと UB (undefined behavior) になるため、`ft_lstnew` 経由で malloc する。

---

## テストの改変ガイドライン

テストを追加・改変するときの指針。

### 1. 副作用ある関数は `printf` 内で再呼び出ししない

NG (古い書き方):
```c
pass = (ft_memmove(buf1, buf2, 5) == buf1);
...
printf("...ft=%p...", ft_memmove(buf1, buf2, 5), ...);  // 2 回目で副作用が増える
```

OK:
```c
r = ft_memmove(buf1, buf2, 5);
pass = (r == buf1);
...
printf("...ft=%p...", r, ...);
```

戻り値を変数 `r` に取って pass と printf の両方で使う。`ft_memmove` / `ft_memcpy` / `ft_memset` 等の **副作用あり関数で必須**。read-only 関数 (ctype / strchr 等) では多少冗長でも害はないが、揃える意味で同じスタイルを推奨。

### 2. alloc 系は NULL チェック先行

NG (古い書き方):
```c
p = ft_calloc(10, sizeof(char));
all_zero = 1;
i = 0;
while (i < 10)
{
    if (p[i] != 0)         // p が NULL なら segfault!
        all_zero = 0;
    i++;
}
pass = (p != NULL && all_zero == 1);
```

OK:
```c
p = ft_calloc(10, sizeof(char));
all_zero = 1;
i = 0;
while (p != NULL && i < 10)
{
    if (p[i] != 0)
        all_zero = 0;
    i++;
}
pass = (p != NULL && all_zero == 1);
```

`ft_xxx` の戻り値を deref する前に `!= NULL` ガード。short-circuit 評価を活用。

### 3. dst バッファの初期状態は明示する

NG (前テストの残骸に依存):
```c
memcpy(dst, "abcde", 5);
r = ft_strlcat(dst, "z", 5);
pass = (r == 6 && dst[0] == 'a' && dst[4] == 'e');
```

OK:
```c
memset(dst, 'X', 16);     /* dst を 'X' で埋めて状態を確定 */
memcpy(dst, "abcde", 5);
r = ft_strlcat(dst, "z", 5);
pass = (r == 6 && dst[0] == 'a' && dst[4] == 'e' && dst[5] == 'X');  /* dst[5] = 'X' のままを確認 */
```

`memset` で dst 全体を埋めてから `memcpy` で必要部分を上書き。テスト順序を入れ替えても結果が変わらない決定的なテストになる。

### 4. 複数箇所で参照するリテラルは変数化

NG (2 つの `""` が別アドレスの可能性):
```c
pass = (ft_strchr("", '\0') != NULL && strchr("", '\0') != NULL);   // 「非 NULL」しか確認できない
```

OK:
```c
/* 関数頭で */
const char *empty = "";

/* 該当ケースで */
pass = (ft_strchr(empty, '\0') == empty && strchr(empty, '\0') == empty);
```

同じ変数を両関数に渡して、戻り値が「**文字列の終端そのもの**」を指していることを確認できる。

### 5. 多段ポインタ deref は中間 NULL ガード

NG:
```c
pass = (dst_list != NULL
        && strcmp((char *)dst_list->next->content, "WORLD") == 0);   // dst_list->next が NULL なら segfault
```

OK:
```c
pass = (dst_list != NULL
        && dst_list->next != NULL
        && dst_list->next->next != NULL
        && strcmp((char *)dst_list->next->content, "WORLD") == 0
        && strcmp((char *)dst_list->next->next->content, "FOO") == 0);
```

リスト構造のテストで、`lst->next->next->...` のような多段 deref をするときは、必ず各段で `!= NULL` をチェック。バグった libft でもテスターがクラッシュせずに KO 判定で済む。

### 6. lst 系は可能ならスタックノード

NG (`ft_lstnew` 経由):
```c
a = ft_lstnew("hello");
b = ft_lstnew("world");
a->next = b;
b->next = NULL;
```

OK (`t_list nodes[N]` でスタック確保):
```c
/* 関数頭で */
t_list	nodes[2];
t_list	*a;
t_list	*b;

/* 該当ケースで */
a = &nodes[0];
b = &nodes[1];
a->content = "hello";
b->content = "world";
b->next = NULL;
a->next = b;
```

`ft_lstnew` が壊れていても他の lst テストが影響を受けない。`free` 不要なのでクリーンアップも楽。

**例外**: テスト対象の関数が `free` を呼ぶ場合 (`ft_lstdelone` / `ft_lstclear`) は malloc が必要。`free(&stack_var)` は UB。

---

## 出力フォーマットの細部

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

### `r=%p` の意味 (mem 系)

`test_memmove` / `test_memcpy` / `test_memset` の各ケースに `r=%p` 列を入れている。これは `ft_xxx` の戻り値 (=dst のアドレス) を毎回表示し、「戻り値が dst を指している」ことを目視できるようにするため。

---

## 実行結果の扱い

### exit code を使った成否判定

各テストバイナリは末尾で:

| 状況 | exit code |
| --- | --- |
| 全ケース OK | 0 |
| KO が 1 つ以上 | 1 |
| fd 出力系 (`test_put*_fd`) | 常に 0 (目視確認のみ、OK/KO 判定なし) |

```bash
./test_strlen
echo $?           # 0 (全 OK) or 1 (KO あり)
```

全テストを一括チェックして失敗したものだけ拾うパターン:

```bash
make
for t in test_*; do
    ./$t >/dev/null 2>&1 || echo "FAIL: $t (exit=$?)"
done
```

`make test` の戻り値は最後に実行されたテストの exit code になる (Makefile 内の `for ... do ./$$t; done` の挙動)。`make test` 全体の成否で判断したい場合は上記のように自前で集計するか、Makefile の `test` ターゲットを `./$$t || exit 1` に書き換える。

### セグフォルトの検出

テストバイナリがクラッシュしたとき、シェルは exit code を `128 + signal番号` の形で返す。よく出るパターン:

| exit code | signal | 意味 |
| --- | --- | --- |
| 139 | SIGSEGV (11) | セグフォルト |
| 138 | SIGBUS (10) | バスエラー (アライメント違反など) |
| 134 | SIGABRT (6) | abort() / assert / ASAN による異常停止 |
| 136 | SIGFPE (8) | 0 除算など |
| 137 | SIGKILL (9) | OOM killer 等で強制終了 |

`exit 1` (KO で正常終了) と区別できるので、libft がクラッシュしたか単にロジックが間違ったかを判定できる:

```bash
make
for t in test_*; do
    ./$t >/dev/null 2>&1
    code=$?
    case $code in
        0)   ;;  # 全 OK、何も出さない
        1)   echo "FAIL (KO): $t" ;;
        139) echo "CRASH (segfault): $t" ;;
        134) echo "CRASH (abort/ASAN): $t" ;;
        138) echo "CRASH (bus error): $t" ;;
        *)   echo "UNKNOWN exit=$code: $t" ;;
    esac
done
```

ASAN 有効ビルド (`make CFLAGS='-Wall -Wextra -Werror -g -fsanitize=address'`) の場合、メモリエラー検出時に ASAN が abort() を呼ぶので exit code 134 になる。「セグフォルトしてないのに 134 が出る」場合は ASAN がエラーをキャッチした合図。

### 結果をテキストファイルに出す

#### 単純なリダイレクト (色付き ANSI エスケープごと保存)

```bash
make test > result.txt 2>&1
```

`stdout` だけでなく `stderr` も含めて保存。ASAN のエラー出力も拾える。`cat result.txt` で再表示すると ANSI 対応ターミナルなら色付きで見える。

#### 色を消してプレーンテキストで保存

ANSI エスケープを `sed` で除去:

```bash
make test 2>&1 | sed 's/\x1b\[[0-9;]*m//g' > result.txt
```

`\x1b` (ESC) で始まる `\[...m` 形式の制御コードをすべて消す。エディタや diff で扱いやすくなる。

#### KO 行だけ抽出

```bash
make test 2>&1 | sed 's/\x1b\[[0-9;]*m//g' | grep KO > ko_only.txt
```

または見出し行も残したい場合:

```bash
make test 2>&1 | sed 's/\x1b\[[0-9;]*m//g' | grep -E '(========= test_|KO)' > ko_with_headers.txt
```

#### テストごとに別ファイルに分ける

```bash
make
mkdir -p logs
for t in test_*; do
    ./$t > logs/$t.txt 2>&1
done
```

`test_strlen` の結果は `logs/test_strlen.txt` に。後で `diff` で差分を取ったり、特定のテストだけ追跡したいときに便利。

#### 過去ログとの diff を取る

```bash
make test 2>&1 | sed 's/\x1b\[[0-9;]*m//g' > result_after.txt
diff result_before.txt result_after.txt
```

libft を直す前後でテスト結果がどう変わったかを diff で確認できる。

---

## テストの追加手順

1. **テストファイルを書く**: `tests/test_<関数名>.c` を新規作成。既存のどれかをコピーして書き換えるのが早い
2. **`Makefile` の `TESTS` リストに 1 行追加**: 関数名 (`test_<関数名>`) を加える
3. **ビルド**: `make test_<関数名>` で個別ビルド + 実行ファイル生成

### 最小テンプレ

```c
#include "libft.h"
#include <stdio.h>
/* 必要に応じて: <string.h> / <ctype.h> / <stdlib.h> / <limits.h> */

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

    /* ANSI escape sequence で色を付ける */
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

**mem 系 (副作用あり、戻り値も確認)** — `r` に戻り値を取って両方で使う:

```c
memset(dst1, 0, 16);
memset(dst2, 0, 16);
r = ft_memcpy(dst1, src, 5);
memcpy(dst2, src, 5);
pass = (memcmp(dst1, dst2, 16) == 0);
if (pass) { color = green; label = "OK"; ok++; }
else      { color = red;   label = "KO"; ko++; }
printf("[%d] memcpy(dst, src, 5)  expect=\"hello\"  ft=\"%.5s\" r=%p  %s%s%s\n",
    1, dst1, r, color, label, reset);
```

**fd 目視 (put*_fd)** — OK / KO 判定なし、stdout に出して目視:

```c
printf("[1] putchar_fd('A', 1)  expect=A  ft=[");
fflush(stdout);
ft_putchar_fd('A', 1);
printf("]  (目視確認)\n");
```

fd 系では `ok` / `ko` カウンタを使わず、サマリーは `printf("%s----- summary: N ケース (目視確認のみ) -----%s\n", cyan, reset);` で出す。

**lst 系 (スタックノード)** — `ft_lstnew` への依存を切る (delone/clear 除く):

```c
/* 関数頭で */
t_list	nodes[3];
t_list	*a;
t_list	*b;
t_list	*c;
t_list	*lst;

/* セットアップ */
a = &nodes[0];
b = &nodes[1];
c = &nodes[2];
a->content = "hello";
b->content = "world";
c->content = "foo";
c->next = NULL;
a->next = b;
b->next = c;
lst = a;
```

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
| `ft_lstdelone`    | spy パターン (呼び出し記録)| 通常 / NULL ノード / NULL del                                    |
| `ft_lstclear`     | spy パターン               | 3 ノード / 空 / NULL del                                         |
| `ft_lstiter`      | spy パターン (`g_buf`)     | 3 ノード / NULL lst / NULL f                                     |
| `ft_lstmap`       | 新リストを `strcmp`        | 通常 / 原リスト不変 / NULL lst / NULL f / NULL del               |

---

## 全テストケース一覧 + 想定ミス

各 `tests/test_*.c` に実装してあるテストケースの全件リスト + 「落ちたら疑うこと」。

「落ちたら疑うこと」は libft 側のよくあるバグパターンを列挙している。完全網羅ではないが、KO が出たときの最初の調査ポイントとして参考に。

### ctype 系

#### `test_isalpha` (7 ケース)

1. `'a'` (小文字) → 非0
    - 落ちたら: `'a'`-`'z'` 範囲チェック忘れ / 境界 `<= 'z'` を `< 'z'`
2. `'Z'` (大文字) → 非0
    - 落ちたら: `'A'`-`'Z'` 範囲チェック忘れ / 境界 `<= 'Z'` を `< 'Z'`
3. `'5'` (数字、アルファベットでない) → 0
    - 落ちたら: `isdigit` と混同 / 数字を alpha 扱い
4. `'!'` (記号) → 0
    - 落ちたら: 「`'A'` 以上ならすべて alpha」のような甘い判定
5. `' '` (空白) → 0
    - 落ちたら: `isspace` と混同 / `'A'` 未満を alpha 扱い
6. `128` (ASCII 範囲外) → 0
    - 落ちたら: 引数を `char` で受けて符号拡張で負値化 → 計算ミス / 上限チェック忘れ
7. `-1` (EOF) → 0
    - 落ちたら: `int` → `unsigned char` キャストで `0xFF` 扱い / EOF 特殊扱い忘れ

#### `test_isdigit` (7 ケース)

1. `'0'` (数字) → 非0
    - 落ちたら: `'0'`-`'9'` 範囲チェック忘れ / 境界 `<= '9'` を `< '9'`
2. `'9'` (数字) → 非0
    - 落ちたら: 上限 `'9'` の境界ミス
3. `'/'` (`'0'` のひとつ前) → 0
    - 落ちたら: 下限 off-by-one (`>= '0' - 1` のようなミス)
4. `':'` (`'9'` のひとつ後) → 0
    - 落ちたら: 上限 off-by-one (`<= ':'`)
5. `'a'` (アルファベット) → 0
    - 落ちたら: `isalpha` と混同 / 範囲が広すぎ
6. `128` (ASCII 範囲外) → 0
    - 落ちたら: 符号拡張ミス / 上限チェック忘れ
7. `-1` (EOF) → 0
    - 落ちたら: 符号拡張ミス / EOF 特殊扱い忘れ

#### `test_isalnum` (9 ケース)

1. `'a'` (英数字) → 非0
    - 落ちたら: alpha 部分の実装漏れ
2. `'Z'` (英数字) → 非0
    - 落ちたら: 大文字判定漏れ
3. `'5'` (英数字) → 非0
    - 落ちたら: digit 部分の実装漏れ (`isalpha` だけ呼んでる)
4. `'!'` (英数字でない) → 0
    - 落ちたら: 記号を含む範囲をうっかり許容
5. `' '` (英数字でない) → 0
    - 落ちたら: 空白を許容してしまっている
6. `'\0'` (NUL、英数字でない) → 0
    - 落ちたら: `'\0'` を特別扱いせず常に 0 になっていない (実装次第で問題なし)
7. `128` (ASCII 範囲外) → 0
    - 落ちたら: 符号拡張 / 上限チェック忘れ
8. `255` (ASCII 範囲外 上限) → 0
    - 落ちたら: `unsigned char` 範囲全体を許容
9. `-1` (EOF) → 0
    - 落ちたら: 符号拡張 / EOF 特殊扱い忘れ

#### `test_isascii` (6 ケース)

1. `0` (ASCII 下限) → 非0
    - 落ちたら: `>= 0` 境界ミス / 0 を除外している
2. `127` (ASCII 上限) → 非0
    - 落ちたら: `<= 127` を `< 127` にしている境界ミス
3. `'A'` (ASCII 範囲内) → 非0
    - 落ちたら: 中間値も判定できていない (まれ)
4. `128` (ASCII 範囲外) → 0
    - 落ちたら: `<= 127` を `<= 128` / 符号扱いの間違い
5. `255` (ASCII 範囲外) → 0
    - 落ちたら: 上限チェック忘れ
6. `-1` (負値) → 0
    - 落ちたら: 負値ガードがない (`c >= 0` 忘れ)

#### `test_isprint` (7 ケース)

1. `' '` (印字可能の下限) → 非0
    - 落ちたら: `>= 32` 境界ミス
2. `'~'` (印字可能の上限) → 非0
    - 落ちたら: `<= 126` を `< 126` にしている境界ミス
3. `'A'` → 非0
    - 落ちたら: 中間値判定漏れ
4. `'\t'` (制御文字) → 0
    - 落ちたら: 制御文字を許容
5. `127` (DEL、制御文字) → 0
    - 落ちたら: `<= 127` を印字可能扱い (DEL を除外できていない)
6. `128` (ASCII 範囲外) → 0
    - 落ちたら: 符号拡張 / 上限チェック忘れ
7. `-1` (EOF) → 0
    - 落ちたら: 符号拡張 / EOF 扱いミス

#### `test_toupper` (5 ケース)

1. `'a'` (小文字) → `'A'`
    - 落ちたら: 変換式 `c - 32` の符号 / `'a'`-`'z'` 範囲チェック忘れ
2. `'z'` (小文字) → `'Z'`
    - 落ちたら: 上限 `'z'` の境界ミス
3. `'A'` (既に大文字) → `'A'`
    - 落ちたら: 範囲ガード忘れて `'A'` を `A - 32` で無意味な値に
4. `'5'` (アルファベット外) → `'5'`
    - 落ちたら: 範囲ガード忘れ / 全部に -32 適用
5. `'!'` (記号) → `'!'`
    - 落ちたら: 範囲ガード忘れ

#### `test_tolower` (5 ケース)

1. `'A'` (大文字) → `'a'`
    - 落ちたら: 変換式 `c + 32` の符号 / `'A'`-`'Z'` 範囲チェック忘れ
2. `'Z'` (大文字) → `'z'`
    - 落ちたら: 上限 `'Z'` の境界ミス
3. `'a'` (既に小文字) → `'a'`
    - 落ちたら: 範囲ガード忘れ
4. `'5'` (アルファベット外) → `'5'`
    - 落ちたら: 範囲ガード忘れ
5. `'@'` (記号) → `'@'`
    - 落ちたら: 範囲ガード忘れ (`'@'` = `'A'` - 1 なので off-by-one が露呈)

### 文字列長・コピー系

#### `test_strlen` (5 ケース)

1. `""` (空文字列) → 0
    - 落ちたら: 初期 `i = 1` にしているなど off-by-one
2. `"a"` (1 文字) → 1
    - 落ちたら: ループ終了条件のミス
3. `"hello"` → 5
    - 落ちたら: ループの基本ロジックが壊れている
4. `"hello, world"` (空白含む) → 12
    - 落ちたら: 空白で打ち切ってしまっている (`isspace` 系と混同)
5. `"ab\0cd"` (途中 NUL) → 2
    - 落ちたら: NUL で止まらず固定サイズで走査している

#### `test_strdup` (4 ケース)

1. `"hello"` → `"hello"`
    - 落ちたら: malloc サイズ計算 (`strlen + 1` を忘れて NUL 切れ) / コピーループのバグ
2. `""` (空文字列) → 非NULL の `""`
    - 落ちたら: 空文字列で `malloc(0)` → NULL を返している / NUL を書き込まない
3. `src="mutable"` を dup 後に `src[0]='X'` → コピー側は `"mutable"` のまま
    - 落ちたら: ポインタを返してるだけで実際に malloc + copy していない
4. `"There are 10 types of people..."` → 同じ文字列
    - 落ちたら: バッファサイズ計算ミスで末尾切れ

#### `test_strlcpy` (5 ケース)

1. `src="hello"`, `size=16` → 戻り 5 / `dst="hello"`
    - 落ちたら: 戻り値が「コピーした長さ」になっていて src の長さでない / NUL 終端忘れ
2. `src="hello"`, `size=3` (切り詰め) → 戻り 5 / `dst="he"`
    - 落ちたら: 切り詰め時の NUL 配置 (`size - 1` の位置) / 戻り値が src の全長になっていない
3. `size=1` (NUL だけ) → 戻り 5 / `dst=""`
    - 落ちたら: size=1 で NUL を書けていない
4. `size=0` (dst 触らない) → 戻り 5 / `dst` 不変
    - 落ちたら: `size=0` ガード忘れ → `dst[0]` 書き換え
5. 空 src → 戻り 0 / `dst=""`
    - 落ちたら: 空 src で NUL 配置忘れ

#### `test_strlcat` (5 ケース)

1. `dst="he"`, `src="llo"`, `size=16` → 戻り 5 / `dst="hello"`
    - 落ちたら: dst の現在長計算 / src 連結 / NUL 終端忘れ
2. `dst="he"`, `src="llo"`, `size=4` (切り詰め) → 戻り 5 / `dst="hel"`
    - 落ちたら: 切り詰め位置 (`size - 1`) / 戻り値 `dst_len + src_len`
3. `dst="he"`, `src="llo"`, `size=0` → 戻り 3 / `dst="he"`
    - 落ちたら: `size=0` ガード忘れ / 戻り値が `size + src_len` でない (man 規定)
4. `dst="abcde"` (size 内で NUL 終端なし), `size=5`, `src="z"` → 戻り 6 / `dst` 不変、`dst[5]='X'` 維持
    - 落ちたら: dst の長さを `strlen(dst)` で取得して size 超え参照 / 戻り値が `size + src_len` でない / dst を超えて書き込んでいる
5. 空 src → 戻り 5 / `dst="hello"`
    - 落ちたら: 空 src で何か変な処理

### メモリ操作系

#### `test_memset` (4 ケース)

1. buf を `'A'` で 5 バイト埋める → libc `memset` と一致
    - 落ちたら: 基本ループのミス
2. 長さ 0 → buf 変化なし
    - 落ちたら: `n=0` ガード忘れ / `while (i <= n)` 境界ミス
3. `c=256+'A'` (高位ビット切り捨て) → `'A'` で埋まる
    - 落ちたら: `(unsigned char)c` キャストせず int で書き込み
4. 戻り値 → buf 自身のアドレス
    - 落ちたら: 戻り値 NULL or 末尾アドレス

#### `test_bzero` (3 ケース)

1. 全 16 バイトを 0 クリア (元 0xAA) → 全 0
    - 落ちたら: 基本ループのミス
2. 先頭 5 バイトだけ 0、6〜15 は 0xAA のまま
    - 落ちたら: 範囲を超えて書き込み / バイト数オーバーラン
3. `n=0` → 何もしない
    - 落ちたら: `n=0` ガード忘れ

#### `test_memcpy` (4 ケース)

1. src の先頭 5 バイトコピー → libc `memcpy` と一致
    - 落ちたら: 基本コピーループのミス
2. NUL 含む 14 バイトコピー
    - 落ちたら: `'\0'` でコピーを打ち切ってしまっている (`strcpy` と混同)
3. `n=0` → 何もコピーしない
    - 落ちたら: `n=0` ガード忘れ
4. 戻り値 → dst 自身のアドレス
    - 落ちたら: 戻り値 NULL or 別アドレス

#### `test_memmove` (5 ケース)

1. 重ならない領域 (`buf+5` に `buf` 先頭 5 バイト)
    - 落ちたら: 基本コピーのミス
2. 前方 overlap → `"0101234789"`
    - 落ちたら: `dst > src` のときに末尾から逆順コピーしていない (`memcpy` と同じ前方ループで壊す)
3. 後方 overlap → `"2345656789"`
    - 落ちたら: `dst < src` のときに前方コピーしていない (逆順だと壊す)
4. `n=0`
    - 落ちたら: `n=0` ガード忘れ
5. 戻り値 → dst 自身
    - 落ちたら: 戻り値 NULL or 別アドレス

### 文字列探索・比較系

#### `test_strchr` (6 ケース)

1. `strchr("hello, world!", 'l')` → s+2
    - 落ちたら: 末尾の `'l'` を返している (`strrchr` と混同) / NUL で止まらず無限ループ
2. `strchr(s, 'h')` → s+0
    - 落ちたら: 先頭から探していない / s+1 から開始
3. `strchr(s, 'z')` (見つからない) → NULL
    - 落ちたら: 末尾の NUL を返している / 終端越えて読み続ける
4. `strchr(s, '\0')` → s+13
    - 落ちたら: `'\0'` を見つける前にループ終了 (man 規定: NUL も検索対象)
5. `strchr("", '\0')` (空文字列で NUL) → empty 自身
    - 落ちたら: 空文字列で NULL を返している / `'\0'` を検索対象にしていない
6. `strchr(s, 256+'h')` (高位ビット) → s+0
    - 落ちたら: `(unsigned char)c` キャストせずに int で比較

#### `test_strrchr` (5 ケース)

1. `strrchr(s, 'l')` → 最後の `'l'` (s+10)
    - 落ちたら: 最初の `'l'` を返している (`strchr` と混同) / 末尾から探索していない
2. `strrchr(s, 'o')` → world の `'o'` (s+8)
    - 落ちたら: hello の `'o'` (s+4) を返している
3. `strrchr(s, 'h')` → s+0
    - 落ちたら: 1 つしかない `'h'` の検出ロジックミス
4. `strrchr(s, 'z')` (見つからない) → NULL
    - 落ちたら: 終端の NUL を返している
5. `strrchr(s, '\0')` → 終端位置 (s+13)
    - 落ちたら: `'\0'` を検索対象にしていない

#### `test_strncmp` (6 ケース)

1. `"hello"` vs `"hello"`, n=5 → 符号 0
    - 落ちたら: 基本ループのミス
2. `"abc"` vs `"abd"`, n=3 → 符号 -1
    - 落ちたら: 大小比較が逆 / 符号化されていない
3. `"abd"` vs `"abc"`, n=3 → 符号 +1
    - 落ちたら: 大小比較が逆
4. `"abc"` vs `"abd"`, n=2 (prefix 一致) → 符号 0
    - 落ちたら: n の制限がない / `< n` でなく `<= n`
5. `""` vs `""`, n=0 → 符号 0
    - 落ちたら: n=0 で読み込んでしまう
6. `"\x80"` vs `"\x01"`, n=1 (unsigned char で比較) → 符号 +1
    - 落ちたら: signed char で比較すると `0x80 = -128 < 0x01 = +1` で符号が逆になる

#### `test_memchr` (6 ケース)

1. `memchr("hello\0world", 'h', 11)` → s+0
    - 落ちたら: 基本探索ロジック
2. `memchr(s, 'w', 11)` (NUL 越え) → s+6
    - 落ちたら: NUL で止まってしまっている (`strchr` と混同)
3. `memchr(s, '\0', 11)` (NUL を探す) → s+5
    - 落ちたら: `'\0'` を特殊扱いして除外
4. `memchr(s, 'z', 11)` (見つからない) → NULL
    - 落ちたら: 末尾を超えて読み続ける
5. `memchr(s, 'h', 0)` (範囲 0) → NULL
    - 落ちたら: n=0 ガード忘れ
6. `memchr(s, 'w', 6)` (範囲外) → NULL
    - 落ちたら: n の制限がない (`'w'` は s+6 にあるので含まないはず)

#### `test_memcmp` (5 ケース)

1. `"hello"` vs `"hello"`, n=5 → 符号 0
    - 落ちたら: 基本ループのミス
2. `"hello"` vs `"hellx"`, n=5 → 符号 -1
    - 落ちたら: 大小比較が逆 / 符号化忘れ
3. `"hello"` vs `"hellx"`, n=4 (prefix 一致) → 符号 0
    - 落ちたら: n の制限ミス
4. `"ab\0cd"` vs `"ab\0ce"`, n=5 (NUL 越え) → 符号 -1
    - 落ちたら: NUL で止まってしまっている (`strncmp` と混同)
5. n=0 → 符号 0
    - 落ちたら: n=0 ガード忘れ

#### `test_strnstr` (6 ケース)

1. `strnstr("hello world foo", "world", 15)` → big+6
    - 落ちたら: 部分一致の検索ロジックミス
2. `strnstr(big, "hello", 15)` → big+0
    - 落ちたら: 先頭から探していない
3. `strnstr(big, "", 15)` (空 needle) → big 自身
    - 落ちたら: 空 needle で NULL を返している (man 仕様: haystack を返す)
4. `strnstr(big, "xyz", 15)` (見つからない) → NULL
    - 落ちたら: 不正な値を返している
5. `strnstr(big, "world", 6)` (n 不足) → NULL
    - 落ちたら: n の制限がない / needle が n を超える位置にあっても見つけてしまう
6. `strnstr(big, "world", 11)` (ちょうど収まる) → big+6
    - 落ちたら: n 境界が `< n` でなく `<= n` などのミス

### 数値変換・確保系

#### `test_atoi` (10 ケース)

1. `"0"` → 0
    - 落ちたら: 基本変換ロジック
2. `"123"` → 123
    - 落ちたら: 桁の加算ミス
3. `"-42"` (負の数) → -42
    - 落ちたら: 符号処理忘れ
4. `"  +42"` (空白 + 符号) → 42
    - 落ちたら: 空白スキップ忘れ / `+` 符号を処理していない
5. `"\t\n\v\f\r 42"` (全 whitespace + 数字) → 42
    - 落ちたら: 一部の whitespace (vt/ff など) を含めていない
6. `"123abc"` (数字以外で打ち切り) → 123
    - 落ちたら: 非数字で停止していない
7. `"+-123"` (二重符号) → 0
    - 落ちたら: 二重符号を許容して -123 を返している
8. `"2147483647"` (INT_MAX) → 2147483647
    - 落ちたら: 中間で overflow (`int` で計算してる)
9. `"-2147483648"` (INT_MIN) → -2147483648
    - 落ちたら: 負の最大値で overflow / INT_MIN を扱えない
10. `"9999999999999999999"` (overflow) → glibc 互換 `(int)LONG_MAX` キャスト値
    - 落ちたら: long のオーバーフロー処理が glibc 互換でない (`INT_MAX` clamp になっている / UB を起こしている)

#### `test_calloc` (5 ケース)

1. `ft_calloc(10, sizeof(char))` → 非NULL、10 バイト全部 0
    - 落ちたら: malloc 後の memset(0) を忘れている / サイズ計算ミス
2. `ft_calloc(5, sizeof(int))` → 非NULL、5 要素全部 0
    - 落ちたら: `nmemb * size` の乗算忘れ
3. `ft_calloc(0, sizeof(int))` (nmemb=0) → 落ちないこと
    - 落ちたら: `malloc(0)` で free できない値を返している
4. `ft_calloc((size_t)-1, 2)` (overflow) → NULL
    - 落ちたら: 乗算オーバーフローのチェック忘れ → 巨大確保失敗で NULL になるか、危険な小さい確保
5. `ft_calloc(2, (size_t)-1)` (overflow) → NULL
    - 落ちたら: 乗算オーバーフローのチェック忘れ

### 文字列加工系

#### `test_substr` (7 ケース)

1. `ft_substr("0xDEADBEEF", 2, 8)` → `"DEADBEEF"`
    - 落ちたら: 基本コピーロジック
2. `ft_substr("0xCAFEBABE", 2, 4)` → `"CAFE"`
    - 落ちたら: start オフセット適用ミス
3. `ft_substr("0xFEEDFACE", 0, 100)` (len 過大) → `"0xFEEDFACE"`
    - 落ちたら: len を `strlen(s) - start` で打ち切っていない → 不定領域を読む
4. `ft_substr("0xBADCAFE", 9, 3)` (start=長さ) → `""`
    - 落ちたら: `start == strlen` のとき NULL を返している / 不定領域を読む
5. `ft_substr("0xBABEFACE", 100, 3)` (start 過大) → `""`
    - 落ちたら: `start > strlen` のときガードがない → 不定領域を読む
6. `ft_substr("0xDEADC0DE", 5, 0)` (len=0) → `""`
    - 落ちたら: len=0 で NUL のみのバッファを返せていない
7. `ft_substr(NULL, 0, 5)` → NULL
    - 落ちたら: NULL ガード忘れ → segfault

#### `test_strjoin` (6 ケース)

1. `ft_strjoin("Hello, ", "World!")` → `"Hello, World!"`
    - 落ちたら: 連結ロジック / バッファサイズ計算
2. `ft_strjoin("", "shell")` (空 s1) → `"shell"`
    - 落ちたら: 空文字列 s1 で何かおかしい処理
3. `ft_strjoin("shell ", "script")` → `"shell script"`
    - 落ちたら: 連結時の境界バイト位置ミス
4. `ft_strjoin("", "")` (両方空) → `""`
    - 落ちたら: `malloc(1)` で NUL バッファを返せていない
5. `ft_strjoin(NULL, "kernel")` → NULL
    - 落ちたら: NULL ガード忘れ → segfault
6. `ft_strjoin("daemon", NULL)` → NULL
    - 落ちたら: NULL ガード忘れ → segfault

#### `test_strtrim` (7 ケース)

1. `ft_strtrim("  /dev/null  ", " ")` → `"/dev/null"`
    - 落ちたら: 前後 trim ロジック
2. `ft_strtrim("###segfault###", "#")` → `"segfault"`
    - 落ちたら: trim 対象が空白限定になっている (`isspace` 使用)
3. `ft_strtrim(" \t\nfork(2)\n\t ", " \t\n")` (複数文字 set) → `"fork(2)"`
    - 落ちたら: set 内の複数文字に対応していない
4. `ft_strtrim(";;;;", ";")` (全部 trim 対象) → `""`
    - 落ちたら: 全 trim で NUL バッファを返せていない / `start > end` で負のサイズ malloc
5. `ft_strtrim("==NULL==", "=")` (内側残す) → `"NULL"`
    - 落ちたら: 内側の文字まで削っている / start/end が逆
6. `ft_strtrim("malloc", "")` (空 set) → `"malloc"`
    - 落ちたら: 空 set で全削除している / 何も返さない
7. `ft_strtrim(NULL, "a")` → NULL
    - 落ちたら: NULL ガード忘れ → segfault

#### `test_split` (6 ケース)

1. `ft_split("ls cd pwd", ' ')` → 3 単語 `{"ls","cd","pwd"}`
    - 落ちたら: 単語数カウントロジック / 各単語のコピー
2. `ft_split("a,,b,,,c", ',')` (連続区切り) → 3 単語 `{"a","b","c"}`
    - 落ちたら: 連続区切りで空単語を含めてしまっている
3. `ft_split(",,bash,,", ',')` (先頭末尾区切り) → 1 単語 `{"bash"}`
    - 落ちたら: 先頭末尾の区切りで空単語を含めている
4. `ft_split(",,,", ',')` (全区切り) → 0 単語、NULL 終端だけ
    - 落ちたら: 単語 0 のときに NULL を返している / `{NULL}` を返せていない
5. `ft_split("", ',')` (空文字列) → 0 単語
    - 落ちたら: 空文字列で `malloc(0)` → NULL / `{NULL}` を返せていない
6. `ft_split(NULL, ',')` → NULL
    - 落ちたら: NULL ガード忘れ → segfault

#### `test_itoa` (6 ケース)

1. `ft_itoa(0)` → `"0"`
    - 落ちたら: 0 を特殊扱いしてない (`while (n != 0)` で抜けて空文字列)
2. `ft_itoa(42)` → `"42"`
    - 落ちたら: 桁順を逆順にしている / NUL 終端忘れ
3. `ft_itoa(-42)` → `"-42"`
    - 落ちたら: 負号付加忘れ / バッファサイズ計算で `-` 分を含めていない
4. `ft_itoa(123456)` → `"123456"`
    - 落ちたら: 桁数カウントが小さい数字に偏っている
5. `ft_itoa(INT_MAX)` → `"2147483647"`
    - 落ちたら: 中間 overflow / 桁数計算で `INT_MAX` を扱えない
6. `ft_itoa(INT_MIN)` → `"-2147483648"`
    - 落ちたら: `-n` で overflow (INT_MIN は `-INT_MIN` が int に収まらない) → 0 を返している / 違う値

#### `test_strmapi` (5 ケース)

1. `ft_strmapi("abc", add_index)` → `"ace"`
    - 落ちたら: index を渡していない / index が常に 0
2. `ft_strmapi("norminette", to_upper_cb)` → `"NORMINETTE"`
    - 落ちたら: 各文字に f を適用していない
3. `ft_strmapi("", add_index)` (空文字列) → 非NULL の `""`
    - 落ちたら: 空文字列で NULL を返している
4. `ft_strmapi(NULL, add_index)` → NULL
    - 落ちたら: NULL s ガード忘れ
5. `ft_strmapi("anything", NULL)` → NULL
    - 落ちたら: NULL f ガード忘れ → segfault

#### `test_striteri` (4 ケース)

1. `ft_striteri("recursion", upper_at)` (in-place 大文字化) → `"RECURSION"`
    - 落ちたら: in-place で書き換えていない / 別バッファに書いている
2. `ft_striteri("abc", add_index_at)` → `"ace"`
    - 落ちたら: index を渡していない
3. `ft_striteri("", upper_at)` (空文字列) → `""`
    - 落ちたら: 空文字列で NUL を上書き
4. `ft_striteri(NULL, upper_at)` → 落ちないこと
    - 落ちたら: NULL ガード忘れ → segfault

### fd 出力系 (目視)

#### `test_putchar_fd` (4 ケース)

1. `ft_putchar_fd('A', 1)` → `A` が表示
    - 落ちたら: write 呼び出しのバグ / fd を捨てている
2. `ft_putchar_fd(' ', 1)` → 空白
    - 落ちたら: 空白が write されない (制御文字扱い)
3. `ft_putchar_fd('\n', 1)` → 改行
    - 落ちたら: 改行を別文字 (`"\\n"`) として出力
4. `ft_putchar_fd('7', 1)` → `7`
    - 落ちたら: 数字キャラと数値を混同して `0x07` を書いている

#### `test_putstr_fd` (5 ケース)

1. `ft_putstr_fd("hello", 1)` → `hello`
    - 落ちたら: 単純ループの実装ミス
2. `ft_putstr_fd("", 1)` (空文字列) → 何も出ない
    - 落ちたら: NUL を 1 文字書き出している
3. `ft_putstr_fd(NULL, 1)` → 何も出ない (落ちない)
    - 落ちたら: NULL ガード忘れ → segfault
4. `ft_putstr_fd("ab\ncd", 1)` (改行透過) → 2 行で表示
    - 落ちたら: 改行で打ち切ってしまう (`putendl` の処理を混ぜている)
5. `ft_putstr_fd("the quick brown fox", 1)` → 全文
    - 落ちたら: 長い文字列でループが終わらない / 途中で止まる

#### `test_putendl_fd` (4 ケース)

1. `ft_putendl_fd("hello", 1)` → `hello` + 改行
    - 落ちたら: 末尾改行を付けていない / 余分な改行
2. `ft_putendl_fd("", 1)` (空文字列) → 改行 1 つだけ
    - 落ちたら: 空文字列で改行を付けない (必ず改行を付ける仕様)
3. `ft_putendl_fd(NULL, 1)` → 何も出ない (改行も出ない)
    - 落ちたら: NULL でも改行だけ出している (本テスター方針)
4. `ft_putendl_fd("the quick brown fox", 1)` → 全文 + 改行
    - 落ちたら: 長い文字列でループが終わらない

#### `test_putnbr_fd` (5 ケース)

1. `ft_putnbr_fd(0, 1)` → `0`
    - 落ちたら: 0 で何も出ない (while 抜け)
2. `ft_putnbr_fd(42, 1)` → `42`
    - 落ちたら: 桁順逆 / 文字キャラへの変換ミス
3. `ft_putnbr_fd(-42, 1)` → `-42`
    - 落ちたら: 負号付加忘れ
4. `ft_putnbr_fd(INT_MAX, 1)` → `2147483647`
    - 落ちたら: 中間 overflow
5. `ft_putnbr_fd(INT_MIN, 1)` → `-2147483648`
    - 落ちたら: `-n` で overflow → 違う値

### 連結リスト系

#### `test_lstnew` (3 ケース)

1. `ft_lstnew("hello")` → 非NULL、`content == 渡したポインタ`、`next == NULL`
    - 落ちたら: malloc 後に `next` を初期化していない (ガベージ) / `content` を別ポインタにしている
2. `ft_lstnew(NULL)` → 非NULL ノード、`content == NULL`
    - 落ちたら: NULL content を拒否している
3. `ft_lstnew((void *)0x1234)` → `content == 0x1234`
    - 落ちたら: content を deref しようとしている

#### `test_lstadd_front` (4 ケース)

1. 空 `lst` に a を追加 → `lst == a`
    - 落ちたら: `lst NULL` の特殊ケース未対応
2. 既存先頭 (a) に b → `b → a`
    - 落ちたら: `new->next = *lst` の更新忘れ
3. さらに c を追加 → `c → b → a`
    - 落ちたら: 複数回呼び出し後のリスト構造維持ミス
4. NULL new → `lst` は変わらない
    - 落ちたら: NULL new ガード忘れ → segfault

#### `test_lstsize` (6 ケース)

1. `ft_lstsize(NULL)` → 0
    - 落ちたら: NULL ガード忘れ → segfault
2-6. 1〜5 ノード追加するたびに `lstsize` が一致
    - 落ちたら: カウンタ初期値 / `next` の追跡漏れ

#### `test_lstlast` (3 ケース)

1. `ft_lstlast(NULL)` → NULL
    - 落ちたら: NULL ガード忘れ → segfault
2. 1 ノードしかない → 自分自身
    - 落ちたら: 必ず `next` を辿ろうとして NULL deref
3. `a → b → c` の 3 ノード → c が返る
    - 落ちたら: 末尾検出ロジックミス (`next != NULL` の境界判定)

#### `test_lstadd_back` (4 ケース)

1. 空 `lst` に a → `lst == a`
    - 落ちたら: `lst NULL` の特殊ケース未対応
2. 末尾 (a) に b → `a → b`
    - 落ちたら: 末尾検出忘れて先頭に挿入
3. さらに c を末尾追加 → `a → b → c`
    - 落ちたら: 複数回呼び出し後の末尾追跡ミス
4. NULL new → 不変
    - 落ちたら: NULL new ガード忘れ → segfault

#### `test_lstdelone` (3 ケース)

1. 通常 → `del` が 1 回呼ばれ、引数は `n->content`
    - 落ちたら: del を呼ばずに `free(n)` だけしている / del の引数が違う
2. `ft_lstdelone(NULL, spy_del)` → del は呼ばれない
    - 落ちたら: NULL ガード忘れ → segfault
3. NULL `del` → 何もしない (ノードが残るので自分で `free`)
    - 落ちたら: NULL del ガード忘れて NULL 関数ポインタ呼び出し → segfault / del スキップしつつ node を free → double-free 危険

#### `test_lstclear` (3 ケース)

1. 3 ノードを clear → `del` が 3 回呼ばれ、`lst == NULL` になる
    - 落ちたら: ループで `next` を辿る前に node を free → use-after-free / del の引数ミス / lst のポインタを NULL に書き戻していない
2. 空リスト → `del` 呼ばれない、`lst == NULL`
    - 落ちたら: NULL lst でループ突入 → segfault
3. NULL `del` → `del` 呼ばれず、`lst` 変化なし
    - 落ちたら: NULL del で関数ポインタ呼び出し → segfault

#### `test_lstiter` (3 ケース)

1. 3 ノード → `g_buf` に `"ABC"` が積まれる
    - 落ちたら: f を全ノードに適用していない / 順序が逆
2. NULL `lst` → f 呼ばれない
    - 落ちたら: NULL ガード忘れ → segfault
3. NULL `f` → f 呼ばれず、落ちない
    - 落ちたら: NULL f で関数ポインタ呼び出し → segfault

#### `test_lstmap` (5 ケース)

1. 通常 → 新リスト `"STDIN"`, `"STDOUT"`, `"STDERR"`
    - 落ちたら: 新ノードを `ft_lstnew` で作らずポインタ共有 / 末尾を NULL 終端していない / 順序ミス
2. 原リスト `src` は変更されていない
    - 落ちたら: 原リストの content を書き換えている / 原リストの next を破壊
3. `ft_lstmap(NULL, f, del)` → NULL
    - 落ちたら: NULL ガード忘れ → segfault
4. NULL `f` → NULL
    - 落ちたら: NULL f を許容して何か返している (leak)
5. NULL `del` → NULL
    - 落ちたら: NULL del を許容して、途中失敗時に解放できない

---

合計 **43 関数 / 219 ケース**。各関数 3〜10 ケース程度の薄さで、libft の主要な仕様と方針をひととおり拾う構成。

---

## おまけ: テストケースの小ネタ解説

テストケースに散りばめた hacker / engineer 文化ネタの解説。「これ何?」となった人向け。

### hex マジックナンバー (test_substr)

未初期化メモリの検出、デバッグ、ファイルフォーマットのシグネチャ等に使われる「読める」16 進数。`int *p = (int *)0xDEADBEEF;` のように埋め込んでおくと、デバッガで見たときに「これは初期化漏れだな」とすぐ分かる、というイディオム。

| パターン | 由来 |
| --- | --- |
| `0xDEADBEEF` | 未初期化メモリ検出の古典。"dead beef" の語呂。IBM RS/6000 系で使われたのが起源とされる |
| `0xCAFEBABE` | Java の `.class` ファイル先頭マジックバイト。可愛い数値の代表格 |
| `0xFEEDFACE` | macOS の Mach-O 実行ファイル (32-bit) の先頭マジック |
| `0xBADCAFE` | Sun の Solaris カーネルが `kmem_alloc` 失敗時に返したパターン |
| `0xBABEFACE` | 一般的なメモリデバッグパターン |
| `0xDEADC0DE` | "dead code" の語呂、デバッグマーカー |

他に `0xC0FFEE`, `0xDECAFBAD`, `0xFACEFEED` などもある。

### Unix 三本柱 (test_lstmap)

`stdin` / `stdout` / `stderr` は Unix の標準入出力。ファイルディスクリプタ `0` / `1` / `2` に対応する。あらゆる Unix プログラムの基礎で、`> /dev/null 2>&1` のような shell イディオムにも頻出。

`kernel` / `daemon` は test_lstmap の単一ノードケースで使用。

### syscall / shell 系 (test_strtrim, test_strjoin)

| 用語 | 意味 |
| --- | --- |
| `/dev/null` | Unix の「捨て先」特殊ファイル。書いたものは消える、読むと EOF |
| `fork(2)` | プロセス複製の syscall。`(2)` は man の section 番号 (= syscall) |
| `segfault` | segmentation fault の略。不正メモリアクセス時の SIGSEGV、C 開発者の最大の敵 |
| `malloc` | C の動的メモリ確保関数 |
| `shell` | OS とユーザの対話インターフェイス (`bash` / `zsh` 等) |
| `kernel` | OS の中核 |
| `daemon` | バックグラウンドで動くプロセス |

### Unix 基本コマンド (test_split)

`ls cd pwd` は shell に入って最初に覚える 3 コマンド (一覧 / 移動 / 現在地)。`bash` は Bourne Again Shell の略、Linux / macOS の標準シェル。

### "Hello, World!" (test_strjoin)

1972 年、Brian Kernighan の B 言語チュートリアルで初登場。1978 年の K&R "The C Programming Language" で広まり、世界共通の「初めて書くプログラム」になった。

### "There are 10 types of people in this world" (test_strdup)

2 進法ジョーク。続きは "those who understand binary, and those who don't" (バイナリを理解する人としない人の 2 種類)。`10` を 2 進法で読むと 10 進の 2 になる、というオチ。

### norminette (test_strmapi)

42 が独自に策定した C コーディング規約 (Norm) のチェッカ。インデント / 関数行数 / 変数宣言位置 等を厳密にチェックする。すべての提出物が pass しないと評価されず、42 学生の天敵にして必修ツール。

### recursion (test_striteri)

CS の基礎概念。Google で「recursion」を検索すると "Did you mean: recursion?" が出る、有名な Easter egg がある。

### 42 (test_itoa)

`ft_itoa(42)` の `42` は 42 Tokyo のシンボルであると同時に、Douglas Adams の "The Hitchhiker's Guide to the Galaxy" で「人生・宇宙・すべての究極の答え」とされた数。42 という学校名そのものがこの引用に由来する。

---

## 既知の限界

- **メモリリークの自動検知はしない**: `valgrind` または `-fsanitize=address` で別途確認する
- **fd 出力は目視確認**: `ft_put*_fd` 系は自動比較せず、テスト出力中に `[A]` のように囲んで見せるだけ
- **ケースは完全網羅ではない**: 各関数 3〜10 ケース程度。実装方針が違う libft では追加・修正が必要
- **マルチスレッド / シグナルは想定外**: libft 課題に含まれないため
- **`mac` (`musl` 等の非 glibc 環境) では `ft_atoi` overflow 等の挙動が違う可能性**: glibc 互換前提のケースがある
- **ctype 高位バイト (128/255) のロケール依存**: 42 PC (macOS / Linux UTF-8 ロケール) 想定。Latin-1 系 8-bit ロケールでは libc が non-zero を返すため失敗する可能性。実害なし

---

## 更新履歴

[Keep a Changelog](https://keepachangelog.com/ja/1.1.0/) 形式。

### [0.2.0] - 2026-05-18

テスト堅牢化、依存関係の最小化、ドキュメント再構成。

#### Added

- `test_calloc` ケース 1: `p == NULL` のときの NULL deref を防ぐ short-circuit ガード
- `test_strchr` 先頭宣言に `const char *empty = "";` を追加 (ケース 5 で利用)
- `test_memmove` / `test_memcpy` / `test_memset` の先頭宣言に `void *r;` を追加
- 各 mem 系テストの printf に `r=%p` 列を追加 (戻り値の目視確認)
- `test_strlcat` ケース 4 に `memset(dst, 'X', 16)` を追加 (dst[5..] 状態の決定化)
- `test_strlcat` ケース 4 の pass 判定に `dst[5] == 'X'` を追加 (書き込み境界の検証)
- `test_lstmap` ケース 1 / 2 に中間 NULL ガード (`dst_list->next != NULL` 等) を追加
- `test_lstsize` / `test_lstadd_front` / `test_lstadd_back` / `test_lstlast` / `test_lstiter` / `test_lstmap` (原リスト側) に `t_list nodes[N]` 配列を追加
- `DESIGN.md`: 設計判断のがっつり版 / 改変ガイドライン / 想定ミス 219 件 / 依存マップ / 実行結果の扱い (exit code / セグフォルト検出 / ログ保存) / 小ネタ解説 等
- テスト入力に hacker / engineer 文化ネタを散りばめ (`test_substr` の hex magic 6 種、`test_lstmap` の stdin/stdout/stderr 等)
- `Makefile`: `make test` の全 pass 時に "ALL CLEAR (43 / 43)" バナーを表示

#### Changed

- `test_strchr` ケース 5: `!= NULL && != NULL` の緩い比較を `== empty && == empty` の厳密比較に変更
- `test_memmove` / `test_memcpy` / `test_memset`: `printf` 内の `ft_xxx(...)` 再呼び出しを廃止し、`r = ft_xxx(...)` で 1 回呼びに統一
- `test_lstsize` / `test_lstadd_front` / `test_lstadd_back` / `test_lstlast` / `test_lstiter` / `test_lstmap` (原リスト側): `ft_lstnew` 経由のノード作成をスタック配列の手動初期化に置き換え
- `test_substr` / `test_strjoin` / `test_strtrim` / `test_split` / `test_strdup` / `test_strmapi` / `test_striteri` / `test_lstmap`: テスト文字列を hacker / engineer 文化系に差し替え (検証ロジックは変更なし)
- `Makefile`: `test` ターゲットを「全テスト走らせる + 全 pass 時のみバナー + 失敗時は exit 1 を返す」形式に変更
- `README.md`: 「とりあえず読む」入口に再構成。詳細は `DESIGN.md` に分離

#### Fixed

- `test_calloc` ケース 1: `ft_calloc(10, sizeof(char))` が NULL を返した場合に segfault する問題を修正
- `test_strchr` ケース 5: 2 つの `""` リテラルが別アドレスになる可能性 / 戻り値が文字列終端を指しているかが確認できない問題を修正
- `test_strlcat` ケース 4: `dst[5..15]` の状態が前のテストケースの残骸に依存していた問題を修正
- `test_lstmap` ケース 1: バグった libft が 1〜2 ノードしか返さない場合に segfault する問題を修正 (KO 判定で済むようになった)

#### Removed

- `test_lstsize` / `test_lstadd_front` / `test_lstadd_back` / `test_lstlast` / `test_lstiter` / `test_lstmap` の `#include <stdlib.h>` (`free` を呼ばなくなったため)
- 上記テストの末尾 `free(...)` 呼び出し群 (スタック化により不要)

### [0.1.0] - 2026-05-17

初回リリース。

- 43 関数 / 219 テストケースの初回実装
- libc 並走 + 手書き期待値のハイブリッド方式
- 色付き OK/KO 表示と末尾サマリー
- 個別バイナリ方式 (1 関数 1 main)
- `make test` の一括実行サポート
- README.md 初版

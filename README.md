# libft_tester

42 Tokyo の Libft 課題 (43 関数) のための自作テスター。`printf` と `if` だけで読み下せる「ベタ書き積み重ね」スタイルで、libft が書ける人なら **テスター本体も自力で改変・追加できる** ことを目指した。

設計判断の詳細・各テストの想定ミス・更新履歴は [DESIGN.md](DESIGN.md) を参照。

---

## 目次

- [特徴](#特徴)
- [動作環境](#動作環境)
- [クイックスタート](#クイックスタート)
- [使い方](#使い方)
- [出力フォーマット](#出力フォーマット)
- [設計方針 (ざっくり)](#設計方針-ざっくり)
- [ディレクトリ構成](#ディレクトリ構成)
- [詳しく知りたい人へ](#詳しく知りたい人へ)
- [参考にしたテスター](#参考にしたテスター)
- [コメント](#コメント)
- [ライセンス](#ライセンス)

---

## 特徴

- **1 関数 1 main、1 バイナリ**: `tests/test_<関数名>.c` をそのままコンパイルして個別実行ファイルにする。1 つが segfault しても他は走る。`make test_strlen` のように単体ビルドも簡単。
- **共通ヘルパなし、マクロなし**: `helper.c` も `#define` も廃して、各テストファイルが `#include "libft.h"` と `#include <stdio.h>` だけで完結。「libft が書ける」レベルの C 知識だけで全コード読める。
- **1 ケース 1 行に圧縮 + 色付き OK / KO**: 各ケースは 1 行で `[N] 入力  expect=...  ft=... libc=...  OK/KO` の形式。OK は緑、KO は赤、テスト名見出しとサマリーはシアン。長い縦スクロールにならない。
- **末尾サマリー + exit code**: 各テストバイナリは末尾で `----- summary: N OK / M KO -----` を出し、KO が 1 つでもあれば `exit 1` を返す。`make test` のフックや CI 連携で使える。
- **`libc` 並走で期待値を生成**: ctype / 文字列比較 / `atoi` など `libc` に対応関数がある場合は並走させて手書き期待値を省く。`libc` に対応が無いもの (`ft_substr` / `ft_split` / `ft_itoa` / `ft_lst*` など) は期待値を手書きで持つ。
- **失敗しても続行**: `KO` を出してから次のケースに進む。途中で止まらないので、全体の赤い場所を一度で見渡せる。
- **メモリリーク検知は外部に任せる**: `valgrind` または `-fsanitize=address` で別途確認する設計。テスター本体は軽く保つ。
- **fd 出力系は目視確認**: `ft_putchar_fd` 等は `fd=1` (stdout) に出して、出力を `[ ]` で囲んで見せる。自動比較しない。
- **lst 系はスタックノード優先**: 多くの lst テストは `t_list nodes[N]` でスタック上にノードを作り、テスト対象以外の libft 関数 (特に `ft_lstnew`) への依存を最小化している。`ft_lstdelone` / `ft_lstclear` は `free` を呼ぶ仕様なので例外的に malloc 必須。

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

### 全部走らせる

```bash
make test
```

43 テストを順に実行。各テストごとに見出し → ケース → サマリーが出る。

### 個別に走らせる

特定の関数だけ集中して開発・デバッグしたいときの基本サイクル:

```bash
# libft 本体を編集
vim ../Libft_git/ft_strlen.c

# 該当テストだけ再ビルド + 実行
make test_strlen && ./test_strlen
```

`Makefile` が `libft.a` の依存を見ているので、libft 側を直すと自動で再ビルドされる。テスター側のコードを直したときも同じコマンドで OK。

シェルのワイルドカードでサブセットも:

```bash
make                       # 全部ビルド
./test_lst*                # lst 系だけ実行
./test_is*                 # ctype 系だけ実行
./test_str*                # str 系だけ実行
```

ループで関数群を回す:

```bash
make
for t in test_is*; do ./$t; done    # ctype 系を全部走らせる
```

### テストを一時的に除外する

`Makefile` の `TESTS` 変数から該当行を **丸ごと削除** する。Makefile は行末 `\` の前にコメントが書けない (`\` がコメントを連結対象にしてしまう) ので、`#` でコメントアウトする方法は使えない。

```makefile
# 修正前 (test_isdigit を含む)
TESTS = \
    test_isalpha \
    test_isdigit \
    test_isalnum \
    ...

# 修正後 (test_isdigit を抜く → 行ごと削除)
TESTS = \
    test_isalpha \
    test_isalnum \
    ...
```

「対象 libft 側で未実装の関数があってビルドが通らない」ような場面で使う。

### 失敗ケースだけ拾う

各テストバイナリは KO がゼロなら `exit 0`、1 つでも KO があれば `exit 1` を返す。失敗だけ眺めたいときは `grep` か exit code を使う:

```bash
# テスト名と KO 行だけ抽出 (シアンの見出し行と赤の KO 行)
make test 2>&1 | grep -E '(========= test_|KO)'

# どのテストが失敗したかを exit code で拾う
make test 2>&1 >/dev/null
for t in test_*; do ./$t >/dev/null || echo "FAIL: $t"; done
```

### valgrind / ASAN で leak チェック

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

色付け (ANSI escape) の仕組み / `!!` 正規化 / 比較系の符号正規化 / `expect=...` の細部などは [DESIGN.md](DESIGN.md#出力フォーマットの細部) を参照。

---

## 設計方針 (ざっくり)

詳細は [DESIGN.md](DESIGN.md#設計方針-がっつり) を参照。判断のサマリーだけ:

- **1 関数 1 main**: 1 バイナリがクラッシュしても他は走る。テストランナーやマクロ登録が不要で `make test_<関数名>` で個別ビルドできる。
- **共通ヘルパなし**: `#cond` (文字列化演算子) や `extern` のような libft 課題では出てこない C テクニックを避けるため。「libft が書ける = テスターも読める」を満たす。
- **libc 並走を優先**: ctype や string 比較は libc と並走させて手書き期待値を省く。手書き期待値は libc に対応関数がないものだけ。
- **失敗時続行**: 1 つのテストファイルで複数ケース走らせるとき、最初の失敗で `return` せず最後まで走らせる。`grep KO` で一覧できて修正の見通しが良い。
- **メモリリーク検知は外部に**: `valgrind` / `-fsanitize=address` という標準ツールに任せて、テスター本体に leak 検知の仕掛けを入れない。
- **fd 出力は目視**: `pipe` / `dup` / `read` の組み合わせは libft の手の内訓を超えるので、`fd=1` に出して `[ ]` で囲んで目視させる。
- **lst 系はスタックノード優先**: `ft_lstnew` への依存を切るために `t_list nodes[N]` でスタック確保。`free` を呼ぶ `ft_lstdelone` / `ft_lstclear` のみ malloc が必要。
- **不正入力 (NULL) は NULL / no-op 前提**: subject 規定外の挙動について特定の実装方針を想定。別実装をテストするときは該当ケースの修正が必要 (詳細は DESIGN.md)。

---

## ディレクトリ構成

```
libft_tester/
├── README.md           ← このファイル (入口)
├── DESIGN.md           ← 技術解説書 (設計判断・想定ミス・更新履歴)
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

## 詳しく知りたい人へ

[DESIGN.md](DESIGN.md) に以下を集約:

- 設計方針 (なぜこの形か) のがっつり版
- 想定する libft 側の方針 (NULL 安全 / glibc 互換など)
- libft 関数への依存マップ (lst 系)
- テストの改変ガイドライン (印 / 注意点)
- 出力フォーマットの細部 (`!!` 正規化、符号正規化など)
- 実行結果の扱い (exit code / セグフォルト検出 / ログ保存)
- テストの追加手順 (テンプレ付き)
- テスト方針一覧 (43 関数の表)
- **全テストケース一覧 + 想定ミス (219 ケース)**: 落ちたときに libft 側の何を疑うべきか
- 既知の限界
- 更新履歴 (Keep a Changelog 風)

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

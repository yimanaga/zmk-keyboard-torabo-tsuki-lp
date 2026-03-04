# ZMK キーマップの JIS 配列対応

このドキュメントでは、macOS の JIS キーボード設定を維持したまま、ZMK ファームウェアで意図した記号が正しく入力されるようにした実装方法を説明する。

## 背景

ZMK は US 配列の HID キーコードを送信する。macOS がキーボードを **JIS** として認識している場合、同じ HID キーコードが JIS 配列として解釈されるため、シンボルキーで意図と異なる文字が入力される。

**例:**
- `&kp LEFT_BRACKET` (HID: LBKT) → US では `[`、JIS では `@`
- `&kp AT_SIGN` (HID: Shift+N2) → US では `@`、JIS では `"`

## 対応方針

1. JIS マッピング定義ファイル `config/keys_jis.h` を新規作成
2. `keymap.keymap` で `#include "keys_jis.h"` してマクロを使用
3. Layer 2（シンボルレイヤー）のバインディングを JIS 互換に変更

## HID キーコード US vs JIS 対応表

macOS JIS モードで、各 HID キーコードが生成する文字:

| HID キーコード | US 配列 | JIS 配列 |
|---------------|---------|----------|
| LBKT | `[` | `@` |
| RBKT | `]` | `[` |
| BSLH | `\` | `]` |
| EQUAL | `=` | `^` |
| SEMI | `;` | `;` / Shift: `+` |
| SQT | `'` | `:` / Shift: `*` |
| MINUS | `-` | `-` / Shift: `=` |
| Shift+N2 | `@` | `"` |
| Shift+N6 | `^` | `&` |
| Shift+N7 | `&` | `'` |
| Shift+N8 | `*` | `(` |
| Shift+N9 | `(` | `)` |
| INT1 (INT_RO) | - | `\` / Shift: `_` |
| INT3 (INT_YEN) | - | `¥` / Shift: `|` |

## 実装ファイル

### config/keys_jis.h

JIS 配列用キーコードマクロを定義するヘッダーファイル。「意図する文字」→「macOS JIS で正しく出力される HID キーコード」のマッピングを提供する。

```c
/*
 * JIS配列用キーコードマクロ (macOS JIS keyboard type)
 *
 * macOS がキーボードを JIS として認識している場合、ZMK が送信する
 * US 配列の HID キーコードが JIS として解釈されるため、
 * シンボルキーの出力が意図と異なる。
 *
 * このヘッダーは「意図する文字」→「macOS JIS で正しく出力される
 * HID キーコード」のマッピングを定義する。
 *
 * 使用例: &kp JIS_LBKT  → macOS JIS で [ が入力される
 */

#pragma once

#include <dt-bindings/zmk/keys.h>

/* 括弧・ブラケット */
#define JIS_LBKT    RBKT         /* [ */
#define JIS_RBKT    BSLH         /* ] */
#define JIS_LBRC    LS(RBKT)     /* { */
#define JIS_RBRC    LS(BSLH)     /* } */
#define JIS_LPAR    LS(N8)       /* ( */
#define JIS_RPAR    LS(N9)       /* ) */

/* 引用符 */
#define JIS_DQT     LS(N2)       /* " */
#define JIS_SQT     LS(N7)       /* ' */

/* 記号 */
#define JIS_AT      LBKT         /* @ */
#define JIS_CARET   EQUAL        /* ^ */
#define JIS_TILDE   LS(EQUAL)    /* ~ */
#define JIS_COLON   SQT          /* : */
#define JIS_STAR    LS(SQT)      /* * */
#define JIS_PLUS    LS(SEMI)     /* + */
#define JIS_AMPS    LS(N6)       /* & */
#define JIS_PIPE    LS(INT3)     /* | */
#define JIS_BSLH    INT1         /* \ */
#define JIS_UNDER   LS(INT1)     /* _ */
#define JIS_EQUAL   LS(MINUS)    /* = */
```

### マクロ一覧

| マクロ | 実体 | 意図する文字 |
|--------|------|-------------|
| `JIS_LBKT` | RBKT | `[` |
| `JIS_RBKT` | BSLH | `]` |
| `JIS_LBRC` | LS(RBKT) | `{` |
| `JIS_RBRC` | LS(BSLH) | `}` |
| `JIS_LPAR` | LS(N8) | `(` |
| `JIS_RPAR` | LS(N9) | `)` |
| `JIS_DQT` | LS(N2) | `"` |
| `JIS_SQT` | LS(N7) | `'` |
| `JIS_AT` | LBKT | `@` |
| `JIS_CARET` | EQUAL | `^` |
| `JIS_TILDE` | LS(EQUAL) | `~` |
| `JIS_COLON` | SQT | `:` |
| `JIS_STAR` | LS(SQT) | `*` |
| `JIS_PLUS` | LS(SEMI) | `+` |
| `JIS_AMPS` | LS(N6) | `&` |
| `JIS_PIPE` | LS(INT3) | `\|` |
| `JIS_BSLH` | INT1 | `\` |
| `JIS_UNDER` | LS(INT1) | `_` |
| `JIS_EQUAL` | LS(MINUS) | `=` |

## keymap.keymap の変更

### インクルード追加

```c
#include "keys_jis.h"
```

### Layer 2 のバインディング（シンボルレイヤー）

Layer 2 では、US 配列のキーコードを JIS 互換マクロに置き換えている。

```dts
layer_2 {
    bindings = <
&none           &none            &none        &none         &none         &none                         &none         &none         &none         &none          &none             &none
&trans          &kp EXCLAMATION  &kp JIS_DQT  &kp JIS_LBKT  &kp JIS_RBKT  &kp LG(INT3)                  &kp N7        &kp N8        &kp N9        &kp JIS_TILDE  &kp JIS_CARET     &kp BACKSPACE
&kp JIS_STAR    &kp DOLLAR       &kp JIS_SQT  &kp JIS_LPAR  &kp JIS_RPAR  &kp JIS_AMPS  &none   &none   &kp NUMBER_4  &kp N5        &kp NUMBER_6  &kp JIS_AT     &kp JIS_COLON     &kp RETURN
&kp LEFT_SHIFT  &kp PERCENT      &kp JIS_AT   &kp JIS_LBRC  &kp JIS_RBRC  &kp JIS_PIPE  &trans  &trans  &kp NUMBER_1  &kp NUMBER_2  &kp NUMBER_3  &kp HASH       &kp LG(INT3)      &kp JIS_UNDER
&trans          &trans           &trans       &none         &none         &trans        &trans  &trans  &trans        &trans        &kp N0        &kp PERIOD     &trans            &trans
    >;
};
```

**JIS マクロを使用しているキー:**
- `JIS_DQT` (`"`), `JIS_LBKT` (`[`), `JIS_RBKT` (`]`)
- `JIS_TILDE` (`~`), `JIS_CARET` (`^`)
- `JIS_STAR` (`*`), `JIS_SQT` (`'`), `JIS_LPAR` (`(`), `JIS_RPAR` (`)`)
- `JIS_AMPS` (`&`), `JIS_AT` (`@`), `JIS_COLON` (`:`)
- `JIS_LBRC` (`{`), `JIS_RBRC` (`}`), `JIS_PIPE` (`|`)
- `JIS_UNDER` (`_`)

**変更していないキー:**
- `EXCLAMATION`, `DOLLAR`, `PERCENT`, `HASH` — Shift+1/3/4/5 は US/JIS で同じ
- `N0`〜`N9`, `NUMBER_1`〜`NUMBER_6` — 数字は US/JIS で同じ
- `LG(INT3)` — Cmd+¥（macOS 固有ショートカット用）

## 変更不要なレイヤー

- **Layer 0（ベース）**: 英字、数字、SEMI/MINUS/FSLH は Unshift 時は US と同じ。Shift 時は JIS 配列通り（`;`+Shift→`+`、`-`+Shift→`=`）のため変更不要
- **Layer 1**: ファンクションキー・矢印・マウスボタンのみ → 変更不要
- **Layer 3**: マウスボタン・矢印・BT/出力切替のみ → 変更不要

## INT1 / INT3 の互換性

ZMK ドキュメントでは `INT1` (`INT_RO`) と `INT3` (`INT_YEN`) の macOS 互換性が「❔」（未テスト）となっている。macOS が JIS キーボードとして認識している場合は、HID の International キーコードを正しく処理する想定。

万が一動作しない場合の代替案:
- `\` (`INT1`) → `NON_US_BACKSLASH` を試す
- `_` (`LS(INT1)`) → 別キーへの割り当てを検討
- `|` (`LS(INT3)`) → 別キーへの割り当てを検討

## 前提条件

- macOS のキーボード設定が **JIS（日本語）** であること  
  （システム設定 → キーボード → キーボードの種類）
- US キーボードとして認識している場合は、この対応は不要（US キーコードのまま正しく入力される）

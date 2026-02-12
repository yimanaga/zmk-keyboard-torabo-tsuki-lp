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

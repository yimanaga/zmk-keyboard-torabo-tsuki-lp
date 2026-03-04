# キーマップカスタマイズ一覧

このドキュメントでは、ZMK の標準設定から変更しているビヘイビア設定やカスタムビヘイビアについて説明する。

## 1. mod-tap (`&mt`) の設定変更

### 変更内容

| 設定項目 | ZMK デフォルト | 本キーマップ |
|---------|--------------|-------------|
| flavor | hold-preferred | **tap-preferred** |
| tapping-term-ms | 200 | 200（変更なし） |

### 設定コード

```dts
&mt {
    flavor = "tap-preferred";
    tapping-term-ms = <200>;
};
```

### 対象キー（Layer 0）

| キー | バインディング | タップ | ホールド |
|------|-------------|--------|---------|
| Z | `&mt LEFT_SHIFT Z` | Z | Left Shift |
| F | `&mt LEFT_ALT F` | F | Left Alt |
| J | `&mt LEFT_ALT J` | J | Left Alt |
| ; | `&mt LEFT_SHIFT SEMI` | ; | Left Shift |
| 英数 | `&mt LEFT_COMMAND LANG2` | LANG2（英数） | Left Command |

### 変更理由

ZMK デフォルトの `hold-preferred` では、キーを押下中に別のキーが押されると即座に hold（修飾キー）として判定される。高速タイピング時に Z キーを押してすぐ次のキーを入力すると、Z ではなく Shift として認識されてしまう問題があった。

`tap-preferred` に変更することで、200ms 以上ホールドした場合のみ修飾キーとして判定し、高速タイピング時は常にタップ（文字入力）として動作するようになる。

### 注意: keymap editor との干渉

この設定は `/ { keymap { } }` の外側に記述している。keymap editor（Web ツール）で keymap を編集すると、`/ { }` の外側にある設定が削除される場合がある。keymap editor 使用後は、この設定が消えていないか確認すること。

## 2. カスタム layer-tap (`&lt_scroll`)

### 概要

S キー専用のカスタム layer-tap ビヘイビア。Layer 3（トラックボールスクロールレイヤー）との組み合わせで、トラックボール操作と高速タイピングの両立を実現する。

### 設定コード

```dts
lt_scroll: layer_tap_scroll {
    compatible = "zmk,behavior-hold-tap";
    #binding-cells = <2>;
    flavor = "tap-preferred";
    tapping-term-ms = <200>;
    hold-while-undecided;
    bindings = <&mo>, <&kp>;
};
```

### 標準 `&lt` との差分

| 設定項目 | 標準 `&lt` | `&lt_scroll` |
|---------|-----------|-------------|
| flavor | tap-preferred | tap-preferred（同じ） |
| tapping-term-ms | 200 | 200（同じ） |
| hold-while-undecided | なし | **あり** |

### 対象キー（Layer 0）

| キー | バインディング | タップ | ホールド |
|------|-------------|--------|---------|
| S | `&lt_scroll 3 S` | S | Layer 3 有効化 |

### `hold-while-undecided` の効果

通常の `&lt`（tap-preferred）では、hold/tap の判定が確定するまでレイヤーが有効化されない。そのため、S を押した直後にトラックボールを動かすと、まだ Layer 0 のまま（カーソル移動モード）で処理され、Layer 3（スクロールモード）に切り替わらない。

`hold-while-undecided` を有効にすると、判定中（undecided）の間もレイヤーが先に有効化される。これにより:

- **S を押した直後にトラックボール操作** → Layer 3 が即座に有効 → スクロールとして動作
- **S を素早く離す / 別キーを押す** → tap-preferred により S が入力される（高速タイピングに影響なし）
- **S を 200ms 以上ホールド** → hold として確定し Layer 3 が維持される

### この設計に至った経緯

1. **標準 `&lt`（tap-preferred）**: S ホールド直後のトラックボール操作で Layer 3 がキャンセルされてしまう
2. **hold-preferred に変更**: スクロールは改善するが、高速タイピング時にサ行（sa, si, su, se, so）の入力で S が認識されなくなる
3. **tap-preferred + hold-while-undecided**: 両方の問題を解決

## 3. トラックボール設定

### 設定コード

```dts
&trackball_listener {
    input-processors = <&zip_xy_transform (INPUT_TRANSFORM_X_INVERT | INPUT_TRANSFORM_Y_INVERT)>;

    scroller {
        layers = <3>;

        input-processors =
            <&zip_xy_transform INPUT_TRANSFORM_X_INVERT>,
            <&zip_xy_scaler 1 20>,
            <&zip_xy_to_scroll_mapper>;
    };
};
```

### 動作モード

| レイヤー | トラックボールの動作 |
|---------|-------------------|
| Layer 0, 1, 2 | マウスカーソル移動（X/Y 軸反転） |
| Layer 3 | スクロール（X 軸反転、Y 軸は Mac の自然スクロール向けにそのまま） |

### スクロールモードの入力プロセッサ

1. `zip_xy_transform INPUT_TRANSFORM_X_INVERT` — X 軸のみ反転（Y 軸は自然スクロール方向を維持）
2. `zip_xy_scaler 1 20` — スクロール速度の調整
3. `zip_xy_to_scroll_mapper` — XY 移動をスクロールイベントに変換

## 4. コンボ: Bluetooth ペアリングクリア

### 設定コード

```dts
combos {
    compatible = "zmk,combos";

    bt_clear {
        bindings = <&bt BT_CLR>;
        key-positions = <29 44>;
        layers = <2>;
    };
};
```

### 動作

Layer 2 で G キー（ポジション 29）と親指キー（ポジション 44）を同時押しすると、Bluetooth ペアリングをクリアする。

## 5. 設定の配置に関する注意

`keymap.keymap` のファイル構成:

```
#include 文
  ↓
&mt 設定              ← / { } の外側
&trackball_listener   ← / { } の外側
  ↓
/ {
    behaviors { }     ← &lt_scroll の定義
    combos { }
    keymap { }        ← keymap editor が管理する範囲
}
```

keymap editor は主に `/ { keymap { } }` 内を書き換えるため、`&mt` や `&trackball_listener` の設定を `/ { }` の外側に配置している。これにより、keymap editor でキーマップを編集しても、これらの設定が上書きされにくくなっている。

ただし、`behaviors { }` 内の `&lt_scroll` は `/ { }` の内側にあるため、keymap editor 使用後に削除されていないか確認が必要。

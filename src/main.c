#include <assert.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>

#include "item.h"
#include "search.h"
#include "util.h"

// main関数
// プログラム使用例: ./knapsack 10 20
//  10個の品物を設定し、キャパ20 でナップサック問題をとく

int main(int argc, char **argv) {
    /* 引数処理: ユーザ入力が正しくない場合は使い方を標準エラーに表示して終了 */
    if (argc != 3) {
        fprintf(stderr, "usage: %s <the number of items (int)> <max capacity (double)>\n", argv[0]);
        exit(1);
    }

    const double W = load_double(argv[2]);
    assert(W >= 0.0);

    Itemset *items;

    if (is_number(argv[1])) {
        // 個数の上限はあらかじめ定めておく
        const int max_items = 100;

        const int n = load_int(argv[1]);
        assert(n <= max_items);  // assert で止める
        assert(n > 0);           // 0以下も抑止する

        printf("max capacity: W = %.f, # of items: %d\n", W, n);

        // 乱数シードを1にして、初期化 (ここは変更可能)
        int seed = 1;
        items = init_itemset(n, seed);
    } else {
        items = load_items_from_binary(argv[1]);
        printf("max capacity: W = %.f, # of items: %zu\n", W, items->number);
    }

    print_itemset(items);

    // ソルバーで解く
    Answer ans = solve(items, W);

    // 表示する
    printf("----\nbest solution:\n");
    printf("value: %4.1f\n", ans.value);
    for (int i = 0; i < get_nitem(items); ++i){
        printf("%d", ans.flags[i]);
    }
    printf("\n");

    free_itemset(items);
    return 0;
}

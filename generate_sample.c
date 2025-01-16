#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    const size_t n_items = 5;
    double values[] = {10.0, 6.0, 12.0, 8.0, 15.0};
    double weights[] = {6.0, 4.0, 8.0, 5.0, 9.0};

    FILE *fp = fopen("sample_items.bin", "wb");
    if (fp == NULL) {
        fprintf(stderr, "ファイルが開けませんでした。\n");
        return 1;
    }

    fwrite(&n_items, sizeof(size_t), 1, fp);

    fwrite(values, sizeof(double), n_items, fp);

    fwrite(weights, sizeof(double), n_items, fp);

    fclose(fp);
    printf("サンプルファイルを生成しました: sample_items.bin\n");
}
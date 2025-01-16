#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "item.h"

// 構造体をポインタで確保するお作法を確認してみよう
Itemset *init_itemset(size_t number, int seed) {
    Itemset *list = (Itemset *)malloc(sizeof(Itemset));

    Item *item = (Item *)malloc(sizeof(Item) * number);

    srand(seed);
    for (int i = 0; i < number; i++) {
        item[i].value = 0.25 * (rand() % 200);
        item[i].weight = 0.25 * (rand() % 200 + 1);
    }
    *list = (Itemset){.number = number, .item = item};
    return list;
}

// itemset の free関数
void free_itemset(Itemset *list) {
    free(list->item);
    free(list);
}

// 表示関数
void print_itemset(const Itemset *list) {
    size_t n = list->number;
    const char *format = "v[%d] = %4.1f, w[%d] = %4.1f\n";
    for (int i = 0; i < n; i++) {
        Item *item = get_item(list, i);
        printf(format, i, get_itemvalue(item), i, get_itemweight(item));
    }
    printf("----\n");
}

Item *get_item(const Itemset *list, size_t index) {
    return &(list->item[index]);
}

size_t get_nitem(const Itemset *list) { return list->number; }

double get_itemweight(const Item *item) { return item->weight; }

double get_itemvalue(const Item *item) { return item->value; }

Itemset *load_items_from_binary(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "ファイル '%s' を開けませんでした。\n", filename);
        exit(1);
    }

    size_t n_items;
    if (fread(&n_items, sizeof(size_t), 1, fp) != 1) {
        fprintf(stderr, "品物数の読み込みに失敗しました。\n");
        fclose(fp);
        exit(1);
    }

    Itemset *items = (Itemset*)malloc(sizeof(Itemset));
    Item *item_array = (Item*)malloc(sizeof(Item) * n_items);
    if (items == NULL || item_array == NULL) {
        fprintf(stderr, "メモリの確保に失敗しました。\n");
        fclose(fp);
        exit(1);
    }

    double *values = (double*)malloc(sizeof(double) * n_items);
    if (fread(values, sizeof(double), n_items, fp) != n_items) {
        fprintf(stderr, "価値の読み込みに失敗しました。\n");
        fclose(fp);
        free(values);
        free(item_array);
        free(items);
        exit(1);
    }

    double *weights = (double*)malloc(sizeof(double) * n_items);
    if (fread(weights, sizeof(double), n_items, fp) != n_items) {
        fprintf(stderr, "重さの読み込みに失敗しました。\n");
        fclose(fp);
        free(weights);
        free(values);
        free(item_array);
        free(items);
        exit(1);
    }

    for (size_t i = 0; i < n_items; i++) {
        item_array[i].value = values[i];
        item_array[i].weight = weights[i];
    }

    items->number = n_items;
    items->item = item_array;

    free(values);
    free(weights);
    fclose(fp);

    return items;
}

int is_number(const char *str) {
    if (str == NULL || *str == '\0') return 0;

    if (*str == '-') str++;

    int has_digit = 0;

    while (*str != '\0') {
        if (!isdigit(*str)) return 0;
        has_digit = 1;
        str++;
    }

    return has_digit;
}
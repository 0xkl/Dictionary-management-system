#ifndef _TARGET_H_
#define _TARGET_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

#define TARGET_TEXT_NAME "dict.txt"
#define TARGET_INDEX_NAME "./dict.dat"
#define TARGET_WORD_MAX_SIZE 60
#define TARGET_WORD_MEANING_COUNT 20
#define TARGET_WORD_MAX_TRANSLATION 100
#define TARGET_WORD_BUFFER 1024

// 单词链表 ---- 单词名称，单词有几种翻译，单词翻译的结果
typedef struct dict{
    char word[TARGET_WORD_MAX_SIZE];    // 要输入的单词，如“#superman”
    uint8_t mean_count;    // 单词解释的个数，如既可以做名词也可以做动词，用@分开
    char trans[TARGET_WORD_MEANING_COUNT][TARGET_WORD_MAX_TRANSLATION];    // 翻译结果
    struct dict *next;
} word_t, *dict_t;

uint32_t ListCount(FILE *fp);    // 词典里面单词的个数，要创建的链表长度
dict_t CreateList(dict_t head, FILE *fp, uint32_t count);    //创建单链表，返回首节点。分配内存。
void SearchList(dict_t head, uint32_t count);    // 查找输入的单词
void DestroyList(dict_t head);    // 释放内存

void WriteIndexFile(dict_t head, const char *filename, uint32_t count);
void ReadIndexFile(dict_t head, const char *filename, uint32_t *count);

void Process(int argc, char **argv);    // 主进程，main函数主要调用接口


#endif /* _TARGET_H_ */

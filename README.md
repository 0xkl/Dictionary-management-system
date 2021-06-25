# 项目分析
## 一、单词查询
给定文本文件 **“dict.txt”** ，该文件用于存储词库。词库的格式为 **"英 - 汉"** ， **“汉 - 英”** 双语词典，每个单词和其解释的格式固定，如下所示：

    #单词
    Trans:解释1@解释2@...解释n

每个新单词都有 *“#”* 开头，解释之间使用 *“@”* 隔开。一个词肯能有多个解释，解释均存储在一行里，行首固定以 **“Trans:”** 开头。
例子：
    
    #hello
    Trans:int.喂；哈罗，你好@vi.说（或大声所）“喂”@n.“喂”的打招呼声

编写程序将词库文件读取到内存中，接受用户输入的单词，在字典中查找单词，并且将解释输出到屏幕上。用户可以反复输入，直到用户输入 **“exit”** 字典程序退出。

程序执行格式如下：
    
    ./app -text

_-text 表示使用文本词库进行单词查找。_

## 二、建立索引，并且使用索引进行单词查询
建立二进制索引，索引格式如下图所示。将文本文件 **“dice.txt”** 文件转换为上图所示索引文件 **“dict.dat”** ，使用索引文件实现单词查找。程序执行格式如下：

    ./app -index

_-index 表示使用文本词库 dict.txt 建立二进制索引词库 dict.dat。_
_-bin 表示使用二进制索引词库进行单词查找。_


# 项目实现
## 一、文本文件单词查询

### 1、单词结构

    #单词
    Trans:解释1@解释2@…解释n

在dict.txt文件中，单词占一行，以“#”开头；解释以“Trans:”开头，内容以“@”分隔。结构我采用链表。具体结构定义如下：
```c
// 单词链表 ---- 单词名称，翻译的个数，单词翻译的结果

typedef struct dict

{
  char word[TARGET_WORD_MAX_SIZE]; // 要输入的单词，如"#superstar"

  uint8_t mean_count;  // 单词解释的个数，如既可以做名词也可以做动词，

  char trans[TARGET_WORD_MEANING_COUNT][TARGET_WORD_MAX_TRANSLATION]; // 翻译结果，用@分开（strtok分割函数）

  struct dict *next;

} word_t, *dict_t;
```
### 2、接口定义

#### 2.1、文件中单词的总数
要建立单链表，就要知道单链表的长度，因此，要知道文件中单词的总个数，定义如下接口：
```c
uint32_t ListCount(FILE *fp); // 词典里面单词的个数，即是要创建链表的长度
```

#### 2.2、创建单链表
从文件中一项一项读出单词及其解释，填充到单词结构体中，创建单链表就是分配内存并连接节点的过程，定义接口如下（count是单词的总数）：
```c
dict_t CreateList(dict_t head, FILE *fp, uint32_t count); // 创建单链表，返回首节点。分配内存。
```

#### 2.3、查找单词
从链表中匹配要查找的单词，找到了就输出单词解释，定义接口如下：
```c
void SearchList(dict_t head, uint32_t count); // 查找输入的单词
```

#### 2.4、释放内存
创建链表分配的内存在结束时释放，定义接口如下：
```c
void DestroyList(dict_t head); // 释放内存

```


## 二、建立索引文件dict.dat

### 1、索引结构

包含如下内容：索引头单词个数（4字节），单词1的单词长度（4字节），单词1的内容（单词长度个字节），单词1的解释个数（4字节），解释1的长度（4字节），解释1的内容（解释1的长度），解释2的长度（4字节），解释2的内容（解释2的长度）...按照这个格式将dict.txt文件的内容写到dict.dat的索引文件中。结构和上面文本查询的结构一样。

### 2、接口定义

将链表的节点按上面的格式一个一个写到索引文件dict.dat中，用fwrite函数写，定义接口如下：
```c
// 链表头结点head，文件名，链表长度

void WriteIndexFile(dict_t head, const char *filename, uint32_t count);

```

## 三、索引文件查找单词

上面建立了索引文件，并按协议的格式将文本文件的内容写到了索引文件中，通过索引查找单词就是从索引文件读出要查找的单词，用fread函数读，接口定义如下：
```c
void ReadIndexFile(dict_t head, const char *filename, uint32_t *count);
```

## 三、编译
```makefile
CC = gcc
CFLAGS = -lm -Wall -g

app: main.c process.o find_word_from_index.o find_word_from_text.o
    $(CC) $(CFLAGS) main.c process.o find_word_from_index.o find_word_from_text.o -o app

process.o: process.c
    $(CC) $(CFLAGS) -c process.c

find_word_from_index.o: find_word_from_index.c
    $(CC) $(CFLAGS) -c find_word_from_index.c

find_word_from_text.o: find_word_from_text.c
    $(CC) $(CFLAGS) -c find_word_from_text.c

claen:
    rm *.o app
```
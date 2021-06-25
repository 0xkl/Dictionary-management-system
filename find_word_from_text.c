#include "target.h"

static char file_exist;

/*
 * ListCount:
 * - 作用：单词个数既是链表的长度
 **/
uint32_t ListCount(FILE *fp){
    uint32_t count = 0;
    char buffer[100];

    while(fgets(buffer, sizeof(buffer), fp)){
        if('#' == buffer[0]){
            ++count;
        }
    }
    rewind(fp);    // 使用文件指针指向文件头

    return count;
}

/*
 * CreateList:
 * - 作用：创建链表，返回头节点
 **/
dict_t CreateList(dict_t head, FILE *fp, uint32_t count){
    dict_t new, pointer;
    char buf[TARGET_WORD_BUFFER];
    uint8_t word_size, trans_size, mean_count = 1, *str;
    uint32_t i, j = 0;

    head = (dict_t)malloc(sizeof(word_t));     //分配节点空间
    if(NULL == head){
        fprintf(stderr, "malloc failure!\n");
        exit(1);
    }
    printf("head success!\n");

    if(count > 0){
        memset(buf, 0, sizeof(buf));
        fgets(buf, sizeof(buf), fp);
        word_size = strlen(buf);
        buf[word_size - 1] = '\0';
        strcpy(head->word, buf);

        memset(buf, 0, sizeof(buf));
        fgets(buf, sizeof(buf), fp);
        trans_size = strlen(buf);
        buf[trans_size - 1] = '\0';

        str = strtok(buf, "@");
        strcpy(head->trans[j++], str + 6);

        while(str = strtok(NULL, "@")){
            strcpy(head->trans[j++], str);
            ++mean_count;
        }
        head->mean_count = mean_count;

        head->next = NULL;    // 到这里位置填充了首节点，并将收节点的下一个节点指向空
        pointer = head;

        /* 将后面（count-1）个依次链接到首节点后面*/
        for(i = 0; i < count - 1; ++i){
            mean_count = 1;
            new = (dict_t)malloc(sizeof(word_t));

            memset(buf, 0, sizeof(buf));
            fgets(buf, sizeof(buf), fp);
            word_size = strlen(buf);
            buf[word_size - 1] = '\0';
            strcpy(head->word, buf);

            memset(buf, 0, sizeof(buf));
            fgets(buf, sizeof(buf), fp);
            trans_size = strlen(buf);
            buf[trans_size - 1] = '\0';

            for(j = 0; j < count;){
                 str = strtok(buf, "@");
                 strcpy(head->trans[j++], str + 6);

                 while(str = strtok(NULL, "@")){
                       strcpy(head->trans[j++], str);
                       ++mean_count;
                   }
               }
            head->mean_count = mean_count;
            head->next = NULL;    

            pointer->next = new;
            pointer = new;
        }
    }
    rewind(fp);

    return head;
}

void PrintList(dict_t head){
    dict_t pointer;
    pointer = head;

    while(pointer != NULL){
        printf("pointer->word = %s, pointer->mean_count = %d\n", pointer->word, pointer->mean_count);
	pointer = pointer->next;
    }
}
/*
 * SearchList:
 * - 作用： 从列表中查找单词
 */
void SearchList(dict_t head, uint32_t count){
    dict_t pointer;
    char str[TARGET_WORD_MAX_SIZE];
    uint32_t i;
//    char new_word;

    while(1){
        file_exist = 0;
        pointer = head;
        printf("Please input a word:");
        fgets(str, TARGET_WORD_MAX_SIZE, stdin);
        str[strlen(str) - 1] = '\0';

        if(strcmp(str, "exit") == 0){
            exit(1);
        }

        while(pointer != NULL){
            if((strcmp(pointer->word, str)) == 0){
                for(i = 0; i < pointer->mean_count; ++i){
                    file_exist = 1;
                    fprintf(stdout, "Trans%d: %s\n", i + 1, pointer->trans[i]);
                }
                break;
            }

            pointer = pointer->next;
        }
        if(file_exist == 0){
            // 这里判断了单词不存在，可以选择添加，也可以选择退出；
//            printf("没找到此单词\n");
//            printf("是否将此单词添加为新的单词？(Y/N):");
//            scanf("%c", &new_word);
//            if(new_word == 'Y' || new_word == 'y'){
//                AddWordToText(head, TARGET_WORD_CUSTOM_TEXT);
//            }
            exit(1);
        }
    }
}

/*
 *
 *
 */

void DestroyList(dict_t head){
    dict_t pointer;

    while(pointer != NULL){
        pointer = head;
        head = head->next;

        free(pointer);
    }
}

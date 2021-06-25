#include "target.h"

static const char params[][15] = {"-text", "-index", "-bin", "-test1 -f", "-test2 -f"};

void Process(int argc, char **argv){
    FILE *fp;
    dict_t head;
    uint32_t count;

    if((fp = fopen(TARGET_TEXT_NAME, "r")) == NULL){
        fprintf(stderr, "open file failure!\n");
        exit(1);
    }
    count = ListCount(fp);

    printf("count: %d\n",count);
    printf("open sucess!\n");

    if((strcmp(argv[1], params[0])) == 0){
        head = CreateList(head, fp, count);
        SearchList(head, count);
        fclose(fp);
        DestroyList(head);
    }

    if(strcmp(argv[1], params[1]) == 0){
        head = CreateList(head, fp, count);
        fclose(fp);
        WriteIndexFile(head, TARGET_INDEX_NAME, count);
        DestroyList(head);
    }

    if(strcmp(argv[1], params[2]) == 0){
        head == CreateList(head, fp, count);
        ReadIndexFile(head, TARGET_INDEX_NAME, &count);
        SearchList(head, count);
        fclose(fp);
        DestroyList(head);
    }
}

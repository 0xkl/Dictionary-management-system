#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define SIZE 100
/*创建一个词库类型*/
typedef struct Words{
	char English[32];	//英文单词
	char Mean[16];	//中文意思
	struct Words *Next_English;	//指向下一个单词
}WORDS;

WORDS* English_Words;	//单词链表-链表头
char * Wordsinfo = "dict.txt";	//词库存储文件
int input = 0;	//选择功能菜单
char tempInput[10];	//临时选择功能菜单的范围
char tempEnglish[SIZE];	//临时输入英文单词
char tempMean[SIZE];	//临时输入中文意思

void InitFromTxt(void);		//初始化读入信息
void deleteInfo(char * English);	//删除一条信息
void reWrite(void);		//重写
void insert2txt(WORDS * eng);	//插入文本
int addInfo(char * English, char * Mean);	//增加信息
void modify(char * English, char * Mean);	//修改学生信息
bool checkAdd(char * English);		//判断英语输入
void showInfo(void);			//打印所有信息	
WORDS * findInfo(char *English);	//查找
int checkInput(char * input, char begin, int end);	//判断输入是否在范围ie内，用来判断
void view(void);		//界面

int main()
{
	InitFromTxt();	//初始化
	view();		//进入菜单
	return 0;
}
//初始化
void InitFromTxt(void)
{
	English_Words = (WORDS *)malloc(sizeof(WORDS));		//给链表头分配内存空间
	English_Words->Next_English = NULL;		//指向设为空
	FILE * filePointer;		//定义一个文件变量
	filePointer = fopen(Wordsinfo, "r");		//使用写入的形式打开文件
	//当没有这个文件的时候，提示用户
	if(filePointer == NULL)
	{
		printf("[IniFromTxt]打开失败！\n");
		return;
	}
	WORDS* e = English_Words;	//当前节点
	while(1)
	{
		WORDS* eng = (WORDS *)malloc(sizeof(WORDS));		//从读入单词列表信息，存进eng中
		if(fscanf(filePointer, "%s %s", &(eng->English), &(eng->Mean)) == EOF)
		{
			//如果读入为EOF， 也就是说为读入为空，那么刚刚新建的eng就不要了，释放分配的内存
			free(eng);
			break;
		}
		eng->Next_English = NULL;	//初始化下一个数据

		e->Next_English = eng;		//将当前节点和新建的节点链接起来
		e = eng;	//调整当前节点为新节点
	}
	fclose(filePointer);	//关闭文件
}
//删除一条信息
void deleteInfo(char * English)
{
	WORDS * eng = English_Words->Next_English;		//当前节点
	WORDS * Parnet_node = English_Words;			//当前父节点
	int isFound = 0;		//是否找到单词
	while(eng != NULL)
	{
		if(strcmp(eng->English, English) == 0)		//判断单词是否相同
		{
			Parnet_node->Next_English = eng->Next_English;
			free(eng);
			reWrite();	//重写进文件
			isFound = 1;
			break;
		}
		Parnet_node = Parnet_node->Next_English;
		eng = eng->Next_English;
	}
	if(isFound)
	{
		printf("[deleteInfo]删除成功！\n");
	}
	else
	{
		printf("[deleteInfo]没有找到记录!\n");
	}
}
//重写一条信息
void reWrite(void)
{
	WORDS * eng = English_Words->Next_English;
	FILE * filePointer = fopen(Wordsinfo, "w");
	fclose(filePointer);
	if(eng == NULL)		//代表没有这个单词，直接返回
	{
		return;
	}
	while(eng != NULL)
	{
		insert2txt(eng);
		eng = eng->Next_English;
	}
}
//插入一条信息
void insert2txt(WORDS * eng)
{
	FILE * filePointer;
	filePointer = fopen(Wordsinfo, "a");
	if(filePointer == NULL)
	{
		printf("[icnser2txt]文件写入不成功！\n");
		return;
	}
	fprintf(filePointer, "%s %s\n", eng->English, eng->Mean);
	fclose(filePointer);
}
//增加单词
int addInfo(char * English, char *Mean)
{
	if(checkAdd(English) == false)
	{
		printf("[addInfo]英文单词重复，不能添加!!\n");
		return 0;
	}
	WORDS * newinfo = (WORDS*)malloc(sizeof(WORDS));	//新建一个新节点
	newinfo->Next_English = NULL;
	//给新节点赋值
	strcpy(newinfo->English, English);
	strcpy(newinfo->Mean, Mean);
	//调整Pointer到最后一个节点
	WORDS * Pointer = English_Words;
	while(Pointer->Next_English != NULL)
	{
		Pointer = Pointer->Next_English;
	}
	//Pointer链接新节点
	Pointer->Next_English = newinfo;
	//插入新信息
	insert2txt(newinfo);
	return 1;
}

//修改学生信息
void modify(char * English, char * Mean)
{
	//修改信息是先删除在添加
	deleteInfo(English);
	if(addInfo(English, Mean))		//使用addInfo进行添加
	{
		printf("[modify]修改成功!\n");
	}
	else
	{
		printf("[modify]修改失败!\n");
	}
}

//判断单词是否重复
bool checkAdd(char * English)
{
	WORDS * eng = English_Words->Next_English;
	while(eng != NULL)
	{
		if(strcmp(eng->English, English) == 0)
			return false;
		eng = eng->Next_English;
	}
	return true;
}

//打印所有信息
void showInfo(void)
{
	WORDS * eng = English_Words->Next_English;
	while(eng != NULL)
	{
		printf("[showInfo]>>>  %s  %s\n",eng->English, eng->Mean);
		eng = eng->Next_English;
	}
	printf("\n");
}

//查找
WORDS * findInfo(char * English)
{
	WORDS * eng = English_Words->Next_English;
	while(eng != NULL)
	{
		if(strcmp(eng->English, English) == 0)
			return eng;
		eng = eng->Next_English;
	}
	return NULL;
}

//判断输入是否在范围内，用来判断
int checkInput(char * input, char begin, int end)
{
	//如果不是一个字符，直接返回
	if(strlen(input) != 1)
	{
		return -1;
	}
	//判断输入是不是在范围内
	if(input[0] < '0' + begin || input[0] > '0' + end)
	{
		return -1;
	}
	return input[0] - '0';
}

void view(void)
{
	system("clear");
	printf("---------------------\n");
	printf("1、增加单词信息\n");
	printf("2、删除单词信息\n");
	printf("3、改变单词信息\n");
	printf("4、查询单词信息\n");
	printf("5、展示单词信息\n");
	printf("6、调出功能菜单\n");
	printf("7、退出\n");
	printf("---------------------\n");
	while(1)
	{	
		printf(">>>");
		scanf("%s", tempInput);
		input = checkInput(tempInput, 1 , 7);
		switch(input)
		{

			case 1:
				printf("[addInfo]请输入单词，中文意思(例如，ceche 高速缓存)数据以空格隔开:");
				scanf("%s	%s", tempEnglish, tempMean);
				addInfo(tempEnglish, tempMean);
				break;
			case 2:
				printf("[deleteInfo]请输入单词:");
				scanf("%s", tempEnglish);
				deleteInfo(tempEnglish);
				break;
			case 3:
				printf("[modify]请输入单词:");
				scanf("%s %s", tempEnglish, tempMean);
				modify(tempEnglish, tempMean);
				break;
			case 4:
				printf("[findInfo]请输入单词:");
				scanf("%s", tempEnglish);
				WORDS * eng = findInfo(tempEnglish);
				if(eng != NULL)
				{
					printf("%s\t\t%s\n",eng->English, eng->Mean);
				}
				else
				{
					printf("[findInfo]单词不存在！按回车键返回\n");
					getchar();	getchar();
				}
				break;
			case 5:
				showInfo();
				printf("[showInfo]>>>----------END----------<<<\n");
				break;
			case 6:
				printf("---------------------\n");
				printf("1、增加单词信息\n");
				printf("2、删除单词信息\n");
				printf("3、改变单词信息\n");
				printf("4、查询单词信息\n");
				printf("5、展示单词信息\n");
				printf("6、调出功能菜单\n");
				printf("7、退出\n");
				printf("---------------------\n");
				break;
			case 7:
				return ;
				break;
			default:
				printf("[!]输入错误！按回车建返回");
				getchar();	getchar();
				break;
		}
	}
}

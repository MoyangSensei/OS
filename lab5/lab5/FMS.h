//
//  FMS.h
//  lab5
//
//  Created by jfy on 2019/6/16.
//  Copyright © 2019 jfy. All rights reserved.
//

#ifndef FMS_h
#define FMS_h

#include <string>
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

#define FILEBLOCK 1
#define DATABLOCK 4
#define FILEDESSIZE 100
#define L 1129
#define B 32
#define KEEPSIZE  105

std::string TIPSNAME[10]={"dir","create","delete","open","close","read","write","help","exit","clean"};
std::string TIPSINFO[10]={"显示文件目录","创建未存在的文件","删除已存在的文件","打开已存在的文件","关闭已打开的文件","读取已存在的文件","写入已存在的文件","显示提示信息","退出文件管理系统（自动保存文件）","清空所有存储内容"
};

typedef struct
{
    char name[10];
    short int length;
    short int allo[10];
} filedes;

typedef struct
{
    char Buffer[B];
    char *p;
    int id;
    int index;
    filedes dsc;
} OpenFileTable;

char ldisk[L][B];
char map[8] = {127,64,32,16,8,4,2,1};
char temp_block[B];
int noofblock;

OpenFileTable openFileTable;
filedes t;

#endif /* FMS_h */

//
//  main.cpp
//  lab5
//
//  Created by jfy on 2019/6/6.
//  Copyright © 2019 jfy. All rights reserved.
//

#include "FMS.h"
#include "SystemFunction.h"
#include <iostream>
using namespace std;

int main(){
    InitFMS();
    load();
    printTips();
    int opCounter=0;
    for(;;){
        cout<<endl;
        cout<<"["<<++opCounter<<"]：";
        string op="";
        cin>>op;
        
        if(!op.compare("exit")){
            savedata();
            return 0;
        }
        else if(!op.compare("create")){
            printf("File Name: ");
            char filename1[10];
            scanf("%s",filename1);
            if(!create(filename1)){
                exit(0);
            }
            getchar();
        }
        else if(!op.compare("delete")){
            printf("File Name: ");
            char filename[10];
            scanf("%s",filename);
            getchar();
            destroy(filename);
        }
        else if(!op.compare("open")){
            printf("File Name: ");
            char filename[10];
            scanf("%s",filename);
            getchar();
            if(!open(filename)){
                cout<<"ERROR: 请输入正确的文件名！"<<endl;
                continue;
            }
            printf("接下来的指令需为读写指令，请输入:");
            cin>>op;
            if(!op.compare("read")){
                getchar();
                char temp[B];
                read(NULL,temp,B);
                printf("%s\n",temp);
                close(filename);
            }
            else if(!op.compare("write")){
                getchar();
                char str[100];
                printf("请输入写入内容：\n");
                scanf("%s",str);
                write(NULL,str,strlen(str)+1);
                close(filename);
                getchar();
            }
        }
        else if(!op.compare("dir")){
            directory();
            printf("\n");
            
        }
        else if(!op.compare("clean")){
            InitFMS();
        }
        else if(!op.compare("help")){
            printTips();
        }
        else{
            cout<<"ERROR: 请正确输入指令！"<<endl;
        }
    }
    return 0;
}

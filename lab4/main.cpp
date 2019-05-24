//
//  main.cpp
//  yemian
//
//  Created by jfy on 2019/5/16.
//  Copyright © 2019 jfy. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include "Stack.h"
using namespace std;

#define MAX 1024
//页面序列总长度、工作集的起始位置p、物理块个数
int pageNum=0,pageStart=0,blockNum=0;
//物理块状态数组
int **blockSta;
//页面序列数组
int *page;
//页面序列指针
int pPage;
//缺页中断数组
char *interrupt;
//缺页数量
int lakePage;
//虚拟内存N
int memory;
//工作集中包含的页数e
int everyWorkSet;
//工作集移动率m
int moveRate;

//使用位
bool *useBit;
//修改位
bool *modifiedBit;
//修改页面序列
bool *modifiedPage;

//空闲物理块数量
int freeBlock;
//空闲物理块状态数组
int **freeBlockSta;

int a[]={5, 5, 3, 1, 2, 1, 4, 4, 4, 1, 4, 3, 6, 5, 3, 2, 6, 4, 2, 2, 6, 3, 5, 7, 7, 7, 6, 4, 5, 6, 6, 4};

//输出信息：页面序列、各物理块的内容、缺页中断、缺页率
void printInfo(int way){
    if(way==1){
        cout<<"最佳置换算法:"<<endl;
    }else if(way==2){
        cout<<"先进先出置换算法:"<<endl;
    }else if(way==3){
        cout<<"最近最久未使用置换算法:"<<endl;
    }else if(way==4){
        cout<<"改进型CLOCK置换算法:"<<endl;
    }else{
        cout<<"页面缓冲算法:"<<endl;
    }
    cout<<"页面序列:  ";
    for(int i=0; i<pageNum; i++){
        cout<<page[i]<<" ";
    }
    cout<<endl;
    if(way==4){
        cout<<"修改页面:  ";
        for(int i=0; i<pageNum; i++){
            if(modifiedPage[i]){
                cout<<"["<<page[i]<<"] ";
            }else{
                cout<<page[i]<<" ";
            }
        }
    }
    if(way==4){
        cout<<endl;
    }
    for(int i=0; i<blockNum; i++){
        cout<<"物理块["<<i+1<<"]: ";
        for(int j=0; j<pageNum; j++){
            cout<<blockSta[i][j]<<" ";
        }
        cout<<endl;
    }
    if(way==5){
        for(int i=0; i<freeBlock; i++){
            cout<<"空闲块["<<i+1<<"]: ";
            for(int j=0; j<pageNum; j++){
                cout<<freeBlockSta[i][j]<<" ";
            }
            cout<<endl;
        }
    }
    cout<<"缺页中断:  ";
    for(int i=0; i<pageNum; i++){
        cout<<interrupt[i]<<" ";
    }
    //缺页率 = 缺页数量(lakePage) / 页面序列总长度(pageNum)
    cout<<endl<<"缺页率: "<<(double)lakePage/pageNum<<endl<<endl;
}

//将物理块填充满
//该函数适用于FIFO、OPT
//该函数不适用于LRU：虽然是同样的填充方法，但是LRU要从头开始维护一个特殊的栈，别的方法不用考虑
void fillFront(){
    pPage=0;
    int a=0;
    int b[blockNum];
    for(int i=0;a<blockNum;i++){
        if(i==0){
            blockSta[0][0]=page[pPage];
            b[a]=page[pPage];
            a+=1;
        }else{
            for(int j=0;j<blockNum;j++){
                blockSta[j][pPage]=blockSta[j][pPage-1];
            }
            bool ifFindCommon=false;
            for(int j=0;j<a;j++){
                if(page[pPage]==b[j]){
                    ifFindCommon=true;
                    break;
                }
            }
            if(!ifFindCommon){
                blockSta[a][pPage]=page[pPage];
                b[a]=page[pPage];
                a+=1;
            }
        }
        pPage+=1;
    }
}

void getRandomModifiedPage(){
    srand((unsigned)time(NULL));
    //设置修改页面数量占页面总数量的比重
    //此处默认为0.3
    double modifiedRate=0.3;
    int modifiedPageNum=ceil(pageNum*modifiedRate);
    int *a;
    a=(int*)malloc(sizeof(int)*modifiedPageNum);
    int flag=0,t=0;
    //随机编号为0到pageNum-1的页面序列编号中生成modifiedPageNum个修改页面
    for(;;){
        flag=0;
        if(t==modifiedPageNum){
            break;
        }
        int RandNum = (rand()%(pageNum-0))+0;
        for(int i = 0; i < t; i++){
            if(a[i] == RandNum)
                flag = 1;
        }
        if(flag != 1){
            a[t] = RandNum;
            modifiedPage[a[t]]=true;
            t++;
        }
    }
}

void PBA(){
    //缺页数量初始化
    lakePage=0;
    //页面序列指针初始化
    pPage=0;
    //缺页中断记录初始化
    for(int i=0;i<pageNum;i++){
        interrupt[i]='N';
    }
    //将物理块填充满
    fillFront();
    //设置替换块指针：此处的更替指针的原理与FIFO是相同的
    int pReplaceBlock=0;
    //使用位初始化（此处用作缺页数列的填充）
    for(int i=0;i<blockNum;i++){
        useBit[i]=false;
    }
    //将物理块填充过程中的缺页补全，替换块指针不动（填充完后又回到第一个位置）
    //从第1页到第pPage页共产生blockNum个缺页中断
    for(int i=0;i<pPage;i++){
        for(int j=0;j<blockNum;j++){
            if(page[i]==blockSta[j][pPage-1]){
                if(!useBit[j]){
                    useBit[j]=true;
                    interrupt[i]='Y';
                    lakePage+=1;
                }
                break;
            }
        }
    }
    
    //从能填充满物理块的那一个页面的下一个页面起开始遍历
    for(int i=pPage;i<pageNum;i++){
        //
        for(int j=0;j<freeBlock;j++){
            freeBlockSta[j][i]=freeBlockSta[j][i-1];
        }
        //寻找所有的物理块内是否存储了当前所查找的页面i
        bool findPage = false;
        for(int j=0; j<blockNum; j++){
            if(page[i]==blockSta[j][i-1]){
                findPage=true;
                break;
            }
        }
        //将前一个页面所对应的物理块状态复制到当前页面所对应的物理块状态
        for(int j=0;j<blockNum;j++){
            blockSta[j][i]=blockSta[j][i-1];
        }
        //物理块内已存在相同页面
        if(findPage){
            //上一页面的物理块状态就是当前页面的物理块状态
            //上一页面的物理块状态已复制，直接进行下一页面即可
            continue;
        }
        //物理块内不存在相同页面
        else{
            //是否在缓冲物理块内
            int inFreeBLockLocation=-1;
            for(int j=0;j<freeBlock;j++){
                if(page[i]==freeBlockSta[j][i]){
                    inFreeBLockLocation=j;
                    break;
                }
            }
            //在缓冲物理块内
            if(inFreeBLockLocation!=-1){
                //产生缺页
                lakePage+=1;
                interrupt[i]='Y';
                //将两块的内容交换
                int temp=blockSta[pReplaceBlock][i];
                blockSta[pReplaceBlock][i]=freeBlockSta[inFreeBLockLocation][i];
                freeBlockSta[inFreeBLockLocation][i]=temp;
                //将替换指针后移
                pReplaceBlock=(pReplaceBlock+1)%blockNum;
                //将这一块的内容放到最后
                for(int j=inFreeBLockLocation;j<freeBlock-1;j++){
                    freeBlockSta[j][i]=freeBlockSta[j+1][i];
                }
                freeBlockSta[freeBlock-1][i]=temp;
            }
            else{
                int spaceFreeBlock=-1;
                for(int j=0;j<freeBlock;j++){
                    if(freeBlockSta[j][i]==0){
                        spaceFreeBlock=j;
                        break;
                    }
                }
                //有空白的缓冲物理块：
                //当前物理块填入空白物理块
                //当前页面填入当前物理块
                if(spaceFreeBlock!=-1){
                    //产生缺页
                    lakePage+=1;
                    interrupt[i]='Y';
                    //将替换指针所指向的物理块进行替换
                    freeBlockSta[spaceFreeBlock][i]=blockSta[pReplaceBlock][i];
                    blockSta[pReplaceBlock][i]=page[i];
                    //将替换指针后移
                    pReplaceBlock=(pReplaceBlock+1)%blockNum;
                }
                //不存在空白的缓冲物理块
                //将空闲物理块的最后一个位置的内容替换成当前的页面，缺页操作同上
                else{
                    //产生缺页
                    lakePage+=1;
                    interrupt[i]='Y';
                    //temp保留当前物理块内容
                    //当前工作物理块填入当前页面作为新内容
                    int temp=blockSta[pReplaceBlock][i];
                    blockSta[pReplaceBlock][i]=page[i];
                    //抛弃空闲物理块第一块的内容，temp放入最后一块的内容
                    for(int j=0;j<freeBlock-1;j++){
                        freeBlockSta[j][i]=freeBlockSta[j+1][i];
                    }
                    freeBlockSta[freeBlock-1][i]=temp;
                    //将替换指针后移
                    pReplaceBlock=(pReplaceBlock+1)%blockNum;
                }
            }
        }
    }
}

void CLOCK_better(){
    //缺页数量初始化
    lakePage=0;
    //页面序列指针初始化
    pPage=0;
    //缺页中断记录初始化
    for(int i=0;i<pageNum;i++){
        interrupt[i]='N';
    }
    //使用位初始化（此处用作缺页数列的填充）
    for(int i=0;i<blockNum;i++){
        useBit[i]=false;
    }
    //随机生成修改页序列
    getRandomModifiedPage();
    //将物理块填充满
    fillFront();
    //设置替换块指针
    int pReplaceBlock=0;
    //将物理块填充过程中的使用位和修改位、缺页补全，替换块指针不动（填充完后又回到第一个位置）
    //从第1页到第pPage页共产生blockNum个缺页中断
    //所有的useBit全部为1
    for(int i=0;i<pPage;i++){
        for(int j=0;j<blockNum;j++){
            if(page[i]==blockSta[j][pPage-1]){
                if(!useBit[j]){
                    useBit[j]=true;
                    interrupt[i]='Y';
                    lakePage+=1;
                }
                if(modifiedPage[i]){
                    modifiedBit[j]=true;
                }
                break;
            }
        }
    }
    //从能填充满物理块的那一个页面的下一个页面起开始遍历
    for(int i=pPage;i<pageNum;i++){
        //寻找所有的物理块内是否存储了当前所查找的页面i
        bool findPage = false;
        for(int j=0; j<blockNum; j++){
            if(page[i]==blockSta[j][i-1]){
                findPage=true;
                break;
            }
        }
        //将前一个页面所对应的物理块状态复制到当前页面所对应的物理块状态
        for(int j=0;j<blockNum;j++){
            blockSta[j][i]=blockSta[j][i-1];
        }
        //物理块内已存在相同页面
        if(findPage){
            //上一页面的物理块状态就是当前页面的物理块状态
            //上一页面的物理块状态已复制，直接进行下一页面即可
            continue;
        }
        //物理块内不存在相同页面
        else{
            //产生缺页
            lakePage+=1;
            interrupt[i]='Y';
            bool ifFindReplaceBlock=false;
            ////////////////////////////////////////////////////////////////////
            //第一次寻找：find(0,0)
            for(int j=0;j<blockNum;j++){
                if(!useBit[pReplaceBlock]&&!modifiedPage[pReplaceBlock]){
                    ifFindReplaceBlock=true;
                    break;
                }else{
                    pReplaceBlock=(pReplaceBlock+1)%blockNum;
                }
            }
            if(ifFindReplaceBlock){
                blockSta[pReplaceBlock][i]=page[i];
                if(modifiedPage[i]){
                    modifiedBit[pReplaceBlock]=true;
                }
                pReplaceBlock=(pReplaceBlock+1)%blockNum;
                continue;
            }
            ////////////////////////////////////////////////////////////////////
            //第二次寻找：find(0,1)
            for(int j=0;j<blockNum;j++){
                if(!useBit[pReplaceBlock]&&modifiedPage[pReplaceBlock]){
                    ifFindReplaceBlock=true;
                    break;
                }else{
                    pReplaceBlock=(pReplaceBlock+1)%blockNum;
                }
            }
            if(ifFindReplaceBlock){
                blockSta[pReplaceBlock][i]=page[i];
                if(modifiedPage[i]){
                    modifiedBit[pReplaceBlock]=true;
                }
                pReplaceBlock=(pReplaceBlock+1)%blockNum;
                continue;
            }
            ////////////////////////////////////////////////////////////////////
            //将集合中所有页面的使用位设置成0
            for(int j=0;j<blockNum;j++){
                useBit[j]=false;
            }
            ////////////////////////////////////////////////////////////////////
            //第三次寻找（使用位已修改）：find(0,0)
            for(int j=0;j<blockNum;j++){
                if(!useBit[pReplaceBlock]&&!modifiedPage[pReplaceBlock]){
                    ifFindReplaceBlock=true;
                    break;
                }else{
                    pReplaceBlock=(pReplaceBlock+1)%blockNum;
                }
            }
            if(ifFindReplaceBlock){
                blockSta[pReplaceBlock][i]=page[i];
                if(modifiedPage[i]){
                    modifiedBit[pReplaceBlock]=true;
                }
                useBit[pReplaceBlock]=true;
                pReplaceBlock=(pReplaceBlock+1)%blockNum;
                continue;
            }
            ////////////////////////////////////////////////////////////////////
            //第四次寻找（使用位已修改）：find(0,1)
            //如果进行到此处，一定可以找到结果，否则证明代码逻辑出现错误
            for(int j=0;j<blockNum;j++){
                if(!useBit[pReplaceBlock]&&modifiedPage[pReplaceBlock]){
                    ifFindReplaceBlock=true;
                    break;
                }else{
                    pReplaceBlock=(pReplaceBlock+1)%blockNum;
                }
            }
            if(ifFindReplaceBlock){
                blockSta[pReplaceBlock][i]=page[i];
                if(modifiedPage[i]){
                    modifiedBit[pReplaceBlock]=true;
                }
                useBit[pReplaceBlock]=true;
                pReplaceBlock=(pReplaceBlock+1)%blockNum;
                continue;
            }
        }
    }
}

void LRU(){
    //缺页数量初始化
    lakePage=0;
    //页面序列指针初始化
    pPage=0;
    //缺页中断记录初始化
    for(int i=0;i<pageNum;i++){
        interrupt[i]='N';
    }
    //自写栈的定义
    int stack[MAX];
    int top=-1;
    //初始化自写栈
    for(int i=0;i<blockNum;i++){
        stack[i]=-1;
    }
    //从页面序列的第一个开始遍历
    for(int i=0;i<pageNum;i++){
        //读数后top自动+1
        top++;
        //栈中无元素：直接插入元素
        if(top==0){
            stack[top]=page[i];
            lakePage+=1;
            interrupt[i]='Y';
        }
        //栈中元素个数小于物理块个数：不重复则直接插入、重复则更新元素在栈中位置
        else if(top<blockNum){
            //栈中不存在待插入元素：新元素从尾部插入
            if(judge(stack,blockNum,page[i])==-1){
                stack[top]=page[i];
                lakePage+=1;
                interrupt[i]='Y';
            }
            //栈中存在待插入元素：重复元素的位置置后，表示最近使用过
            else{
                move(stack,top,judge(stack,blockNum,page[i]));
                top--;
            }
        }
        //栈中元素个数大于物理块个数：重复则更新元素在栈中位置、不重复就淘汰并替换元素
        else{
            //栈中不存在待插入元素：新元素栈法插入（淘汰替换旧元素）
            //栈法插入：第一个元素出，后面元素前移，新元素从尾部入
            //产生缺页
            if(judge(stack,blockNum,page[i])==-1){
                insert(stack,blockNum,page[i]);
                lakePage+=1;
                interrupt[i]='Y';
                top--;
            }
            //栈中存在待插入元素：重复元素的位置置后，表示最近使用过
            else{
                move(stack,blockNum,judge(stack,blockNum,page[i]));
                top--;
            }
        }
        //更新物理块的变化情况
        for(int j=0;j<blockNum;j++){
            if(stack[j]!=-1){
                blockSta[j][i]=stack[j];
            }
        }
    }
}

void FIFO(){
    //缺页数量初始化
    lakePage=0;
    //页面序列指针初始化
    pPage=0;
    //缺页中断记录初始化
    for(int i=0;i<pageNum;i++){
        interrupt[i]='N';
    }
    //将物理块填充满
    fillFront();
    //使用位初始化（此处用作缺页数列的填充）
    for(int i=0;i<blockNum;i++){
        useBit[i]=false;
    }
    //将物理块填充过程中的缺页补全
    //从第1页到第pPage页共产生blockNum个缺页中断
    for(int i=0;i<pPage;i++){
        for(int j=0;j<blockNum;j++){
            if(page[i]==blockSta[j][pPage-1]){
                if(!useBit[j]){
                    useBit[j]=true;
                    interrupt[i]='Y';
                    lakePage+=1;
                }
                break;
            }
        }
    }
    //设置替换块指针
    int pReplaceBlock=0;
    //从能填充满物理块的那一个页面的下一个页面起开始遍历
    for(int i=pPage;i<pageNum;i++){
        //寻找所有的物理块内是否存储了当前所查找的页面i
        bool findPage = false;
        for(int j=0; j<blockNum; j++){
            if(page[i]==blockSta[j][i-1]){
                findPage=true;
                break;
            }
        }
        //将前一个页面所对应的物理块状态复制到当前页面所对应的物理块状态
        for(int j=0;j<blockNum;j++){
            blockSta[j][i]=blockSta[j][i-1];
        }
        //物理块内已存在相同页面
        if(findPage){
            //上一页面的物理块状态就是当前页面的物理块状态
            //上一页面的物理块状态已复制，直接进行下一页面即可
            continue;
        }
        //物理块内不存在相同页面
        else{
            //产生缺页
            lakePage+=1;
            interrupt[i]='Y';
            //将替换指针所指向的物理块进行替换
            blockSta[pReplaceBlock][i]=page[i];
            //将替换指针后移
            pReplaceBlock=(pReplaceBlock+1)%blockNum;
        }
    }
}

void OPT(){
    //缺页数量初始化
    lakePage=0;
    //页面序列指针初始化
    pPage=0;
    //缺页中断记录初始化
    for(int i=0;i<pageNum;i++){
        interrupt[i]='N';
    }
    //将物理块填充满
    fillFront();
    //使用位初始化（此处用作缺页数列的填充）
    for(int i=0;i<blockNum;i++){
        useBit[i]=false;
    }
    //将物理块填充过程中的缺页补全，替换块指针不动（填充完后又回到第一个位置）
    //从第1页到第pPage页共产生blockNum个缺页中断
    for(int i=0;i<pPage;i++){
        for(int j=0;j<blockNum;j++){
            if(page[i]==blockSta[j][pPage-1]){
                if(!useBit[j]){
                    useBit[j]=true;
                    interrupt[i]='Y';
                    lakePage+=1;
                }
                break;
            }
        }
    }
    //从能填充满物理块的那一个页面的下一个页面起开始遍历
    for(int i=pPage;i<pageNum;i++){
        //寻找所有的物理块内是否存储了当前所查找的页面i
        bool findPage = false;
        for(int j=0; j<blockNum; j++){
            if(page[i]==blockSta[j][i-1]){
                findPage=true;
                break;
            }
        }
        //将前一个页面所对应的物理块状态复制到当前页面所对应的物理块状态
        for(int j=0;j<blockNum;j++){
            blockSta[j][i]=blockSta[j][i-1];
        }
        //物理块内已存在相同页面
        if(findPage){
            //上一页面的物理块状态就是当前页面的物理块状态
            //上一页面的物理块状态已复制，直接进行下一页面即可
            continue;
        }
        //物理块内不存在相同页面
        else{
            //产生缺页
            lakePage+=1;
            interrupt[i]='Y';
            //nearPage：记录最近要被调用的位置
            int *nearPage;
            //ifFindNearPage：记录从该页面的下一个页面起到序列结束，是否有当前页面的调用
            bool *ifFindNearPage;
            nearPage=(int*)malloc(blockNum*sizeof(int));
            ifFindNearPage=(bool*)malloc(blockNum*sizeof(bool));
            //向着页面序列结束的方向寻找当前物理块每一个内容最近要被调用的位置
            for(int j=0;j<blockNum;j++){
                nearPage[j]=-1;
                ifFindNearPage[j]=false;
                for(int k=i+1;k<pageNum;k++){
                    if(blockSta[j][i]==page[k]){
                        nearPage[j]=k;
                        //找到调用：记录调用位置
                        ifFindNearPage[j]=true;
                        break;
                    }
                }
                //直到序列结束都未找到调用：调用位置设为序列长度+1
                if(!ifFindNearPage[j]){
                    nearPage[j]=pageNum+1;
                }
            }
            //找到最远调用的物理块
            int farPageNum=0,farPage=nearPage[0];
            for(int j=1;j<blockNum;j++){
                if(farPage<nearPage[j]){
                    farPage=nearPage[j];
                    farPageNum=j;
                }
            }
            //替换最远调用物理块中的内容为当前页面
            blockSta[farPageNum][i]=page[i];
        }
    }
}

//符合局部访问特性的随机生成算法
void getRandomPage(){
    srand((unsigned)time(NULL));
    double t=rand()%10/10.0;
    for(int i=0;i<(int)(pageNum/moveRate);i++){
        for(int j=i*moveRate;j<(i+1)*moveRate;j++){
            page[j]=(pageStart+rand()%everyWorkSet)%memory;
        }
        double r=rand()%10/10.0;
        if(r<t){
            pageStart=rand()%memory;
        }else{
            pageStart=(pageStart+1)%memory;
        }
    }
    //序列的最后几个内容的长度不足以构成工作集移动的一步，单另填充，规则不变
    for(int i=(int)(pageNum/moveRate)*moveRate;i<pageNum;i++){
        page[i]=(pageStart+rand()%everyWorkSet)%memory;
    }
    cout<<"生成的随机序列为: ";
    for(int i=0;i<pageNum;i++){
        cout<<page[i]<<" ";
    }
    cout<<endl<<endl;
}

void init(){
    //键入信息
    cout<<"输入页面序列长度、页面起始位置、物理块数、每个工作集包含的页面数、工作集移动率:";
    cin>>pageNum>>pageStart>>blockNum>>everyWorkSet>>moveRate;
    cout<<"PBA算法中的缓冲物理块数量(需小于工作物理块数):";
    while(1){
        cin>>freeBlock;
        if(freeBlock<blockNum){
            break;
        }else{
            cout<<"缓冲物理块数量有误，请重新输入:";
        }
    }
    //按照输入信息初始化容器的大小和内容
    page=(int*)malloc(pageNum*sizeof(int));
    modifiedPage=(bool*)malloc(sizeof(bool)*pageNum);
    interrupt=(char*)malloc(pageNum*sizeof(char));
    blockSta=(int**)malloc(blockNum*sizeof(int));
    useBit=(bool*)malloc(sizeof(bool)*blockNum);
    modifiedBit=(bool*)malloc(sizeof(bool)*blockNum);
    for(int i=0;i<blockNum;i++){
        blockSta[i]=(int*)malloc(pageNum*sizeof(int));
        useBit[i]=false;
        modifiedBit[i]=false;
    }
    for(int i=0;i<pageNum;i++){
        page[i]=-1;
        interrupt[i]='N';
        modifiedPage[i]=false;
        for(int j=0;j<blockNum;j++){
            blockSta[j][i]=0;
        }
    }
    freeBlockSta=(int**)malloc(sizeof(int)*freeBlock);
    for(int i=0;i<freeBlock;i++){
        freeBlockSta[i]=(int*)malloc(sizeof(int)*pageNum);
    }
    for(int i=0;i<freeBlock;i++){
        for(int j=0;j<pageNum;j++){
            freeBlockSta[i][j]=0;
        }
    }
    //虚拟内存大小设置为64(K)
    memory=64;
    //页面序列指针
    pPage=0;
    cout<<endl;
}

int main(){
    //初始化
    init();
    //符合局部访问特性的随机生成算法
    getRandomPage();
    //page=a;
    //最佳替换算法
    OPT();
    printInfo(1);
    //先进先出替换算法
    FIFO();
    printInfo(2);
    //最近最久未使用置换算法
    LRU();
    printInfo(3);
    //改进型CLOCK置换算法
    CLOCK_better();
    printInfo(4);
    //PBA
    PBA();
    printInfo(5);
    return 0;
}

//5 5 3 1 2 1 4 4 4 1 4 3 6 5 3 2 6 4 2 2 6 3 5 7 7 7 6 4 5 6 6 4
//32 1 3 5 10
//2

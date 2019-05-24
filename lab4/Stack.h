//
//  Stack.h
//  yemian
//
//  Created by jfy on 2019/5/17.
//  Copyright © 2019 jfy. All rights reserved.
//

#ifndef Stack_h
#define Stack_h
//判断数组中是否已有x，若有返回其下标值，没有则返回-1
int judge(int a[],int n,int x){
    int i;
    for(i=0;i<n;i++)
        if(x==a[i])
            return i;
    return -1;
}
//栈法插入(第一个元素出，后面元素前移，新元素从尾部入)
void insert(int a[],int n,int x){
    int i;
    for(i=0;i<n-1;i++)
        a[i]=a[i+1];
    a[n-1]=x;
}
//移动下标为i的元素到尾部
void move(int a[],int n,int i){
    int j;
    int m=a[i];
    for(j=i;j<n-1;j++)
        a[j]=a[j+1];
    a[n-1]=m;
}


#endif /* Stack_h */

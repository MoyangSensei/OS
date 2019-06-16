//
//  AuxiliaryFunction.h
//  lab5
//
//  Created by jfy on 2019/6/16.
//  Copyright © 2019 jfy. All rights reserved.
//

#ifndef AuxiliaryFunction_h
#define AuxiliaryFunction_h
#include "FMS.h"
#include <iostream>
using namespace std;

bool read_block(int i,char *p)
{
    memcpy(p,ldisk[i],B);
    return true;
}

bool write_block(int i,char *p)
{
    memcpy(ldisk[i],p,B);
    return true;
}

void savedata(){
    FILE *fp = fopen("/study/专业课作业/大三下操作系统/lab5/data.txt","w");
    if(fp==NULL){
        exit(0);
    }
    fwrite(&ldisk[0][0],1,L*B,fp);
    fclose(fp);
}

void load(){
    FILE *fp = fopen("/study/专业课作业/大三下操作系统/lab5/data.txt","r");
    if(fp==NULL){
        exit(0);
    }
    fread(&ldisk[0][0],1,L*B,fp);
    fclose(fp);
}

void printTips(){
    cout<<"----------------------------------------"<<endl;
    for(int i=0;i<10;i++){
        cout<<TIPSNAME[i];
        for(int j=0;j<10-TIPSNAME[i].length();j++){
            cout<<" ";
        }
        cout<<TIPSINFO[i];
        cout<<endl;
    }
    cout<<"----------------------------------------"<<endl;
    cout<<endl;
}

#endif /* AuxiliaryFunction_h */

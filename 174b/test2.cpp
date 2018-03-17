//
//  tester.cpp
//  174b
//
//  Created by Xiaofeng Xu on 26/02/2018.
//  Copyright © 2018 Xiaofeng Xu. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include<string>
#include<iostream>
//#include"console.h"
//#include "origin_reader.h"
//#include "command_checker.h"
//#include "index_reader.h"
//#include "database.h"
//#include<vector>
//#include"sorter.h"
//#include"writer.h"
#include "external_sorter.h"
using namespace std;

int main(void) {

    int page_num = 200;
    string filename = "external.txt";
    external_sorter ext(page_num,2000);//page_num, page_size
    writer wt("txt",1000);
    ext.pass_0_sort(filename,&wt);
    ext.merge_sorting("p0sorted.txt");
    cout<<"end of pass"<<endl;
    return 0;
    

    
};

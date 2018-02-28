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
#include"console.h"
#include "origin_reader.h"
#include "command_checker.h"
#include "index_reader.h"
#include<vector>
#include"sorter.h"
using namespace std;

int main(void) {
    //testing origin_reader
    
    string filename = "test.txt";
    string space = " ";
    if(space.compare(" ")==0){
        cout<<"equal space!"<<endl;
    }
    cout<<"start class origin read"<<endl;
    origin_reader original_reader(filename);
    
    original_reader.print_pairs();
    sorter sorter;
    
    sorter.sortvec( &original_reader );
    
    original_reader.print_pairs();
    //testing index_reader
    /*
    string filename2 = "pairs.txt";
    cout<<"start class index_reader"<<endl;
    index_reader index_reader_o(filename2);
    index_reader_o.print_pairs();
    */
    
        
    return 0;
};

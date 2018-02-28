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
#include"writer.h"
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
    
    cout<<"Before sorting"<<endl<<endl;
    original_reader.print_pairs();
    sorter sorter;
    
    sorter.sortvec( &original_reader );
    cout<<"After sorting"<<endl<<endl;
    original_reader.print_pairs();
    original_reader.duplicate_eliminate();
    cout<<"After duplicate elimination: "<<endl<<endl;
    original_reader.print_pairs();
    
    //test writer
    cout<<"start writing origin_index"<<endl;
    writer writer("origin_index.txt",1000);
    writer.write( &original_reader );
    cout<<"Finished writing the inverted index file"<<endl<<endl;
    //testing index_reader
    
    string filename2 = "origin_index.txt";
    cout<<"start class index_reader"<<endl;
    index_reader index_reader_o2(filename2);
    index_reader_o2.print_pairs();
    cout<<"index reader tested!"<<endl;
    
    cout<<"end!!!!"<<endl;
    return 0;
};

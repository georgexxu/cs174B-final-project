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
#include "database.h"
#include<vector>
#include"sorter.h"
#include"writer.h"
using namespace std;

int main(void) {
    
    
    //testing origin_reader

    string a("zz");
    string b("abcdefg");
    if(a>b){
        cout<<"zz is greater than adbcdefg"<<endl;
    }else{
        cout<<"zz is equal or smaller than"<<endl;
    }

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
    
    //test writer for index_reader
    cout<<"start writing index"<<endl;
    class writer writer2("final_index.txt",1000);
    writer2.write( &index_reader_o2 );
    cout<<"Finished writing the index file"<<endl<<endl;

    
    writer2.write_from_index1("origin_index.txt");
    //testing the tree
    cout<<"test the tree start here"<<endl;
    database dbs;
    
    dbs.load("final_index.txt");
    char key[16] ={"so"};
    
    cout<<"search the keyword so"<<endl;
    dbs.search(key);
    char key2[16] = {"spend"};
    cout<<"range search between so and spend"<<endl;
    dbs.range_search(key,key2);
    
    //test delete
    dbs.delete_key_doc("about", "120", &writer2);
    
    
    
    //update index2 after delete
    writer2.write_from_index1("origin_index.txt");
    //load again
    dbs.load("final_index.txt");
    
    
    //test insert
    dbs.insert_key_doc("allocate", "DOC11", &writer2);
    
    
    cout<<"end!!!!"<<endl;
    return 0;
};

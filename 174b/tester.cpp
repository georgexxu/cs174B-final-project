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
    
    
//    
//    writer wt("txt",1000);
//    wt.write_from_index1("temperary_name.txt");
//    return 0;
//
    

    int page_num = 1000;
    
    
    string filename = "test.txt";

    cout<<"start class origin read"<<endl;
    origin_reader original_reader(filename);
    
    cout<<"Before sorting"<<endl<<endl;
    original_reader.print_pairs();
    sorter sorter;
    
    sorter.sortvec( &original_reader );
    cout<<"After sorting"<<endl<<endl;
  //  original_reader.print_pairs();
    original_reader.duplicate_eliminate();
    cout<<"After duplicate elimination: "<<endl<<endl;
   // original_reader.print_pairs();
    
    //test writer
    cout<<"start writing origin_index"<<endl;
    writer writer("origin_index.txt",page_num);
    writer.write( &original_reader );
    cout<<"Finished writing the inverted index file"<<endl<<endl;
    //testing index_reader
    
    class writer writer2("xt",page_num);
    writer2.write_from_index1("origin_index.txt");
    cout<<"Finished writing the final index file"<<endl<<endl;

    
    writer2.write_from_index1("origin_index.txt");
    //testing the tree
    cout<<"test the tree start here"<<endl;
    
    database dbs;
    //dbs.test_load(&index_reader_o2);
    dbs.merge("sec_file.txt", &writer2);

    dbs.load("final_index.txt");
    char key[16] ={"heard"};
    cout<<"search the keyword feng"<<endl;
    dbs.search(key);
    char key2[16] = {"higher"};
    cout<<"range search between heard and higher"<<endl;
    dbs.range_search(key,key2);
    cout<<"Please give me the range search range"<<endl;
    

    dbs.merge("sec_file.txt", &writer2);
    database dbs3;
    writer2.write_from_index1("origin_index.txt");
    dbs.load("final_index.txt");
    //    return 0;
    
    
    //test delete
//    dbs.delete_key_doc("could", "1", &writer2);
    

//    //update index2 after delete
//    writer2.write_from_index1("origin_index.txt");
//    //load again
//    dbs.load("final_index.txt");
    
    
    //test insert
    cout<<"test insert"<<endl;
    dbs.insert_key_doc("couple", "DOC1", &writer2, "origin_index.txt");
    //test delete doc
    dbs.delete_document("DOC6", &writer2, "origin_index.txt");
    
    cout<<"end!!!!"<<endl;
    return 0;
};

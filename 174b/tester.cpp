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
    dbs.search(key,&writer2);
    char key2[16] = {"higher"};
    cout<<"range search between heard and higher"<<endl;
    dbs.range_search(key,key2);
    cout<<"Please give me the range search range"<<endl;
    

    dbs.merge("sec_file.txt", &writer2);
    database dbs3;
    writer2.write_from_index1("origin_index.txt");
    dbs.load("final_index.txt");
<<<<<<< HEAD
    cout<<"testing search"<<endl;
    char but[16] ={"but"};
    char chinatown[16]={"chinatown"};
    dbs.search(but,&writer2);
    dbs.search(chinatown, &writer2);
    int counter =dbs.count(but, &writer2);
    cout<<"counting this but key:  "<<counter<<endl;//error when directly cout, dont know why
    //cout<<"counting this but key:  "<<dbs.count(but, &writer2)<<endl;
    cout<<"print path:"<<endl;
    dbs.printpath(but);
    cout<<"print page 4 content:"<<endl;
    dbs.page(4, &writer2);
=======
    //    return 0;
    
    
    //test delete
//    dbs.delete_key_doc("could", "1", &writer2);
    
>>>>>>> b28092998a6ab98dc0ee0ad6c98aab4068e5cc1c

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

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
    
    string weird("122222222   ");
    int num = stoi(weird);
    cout<<"num "<<num<<endl;

    



    int page_num = 1000;
    int proceed;
    cout<<"enter an int to proceed"<<endl;
    cin>>proceed;
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
    cout<<"enter an int to proceed"<<endl;
    cin>>proceed;
    //test writer
    cout<<"start writing origin_index"<<endl;
    writer writer("origin_index.txt",page_num);
    writer.write( &original_reader );
    cout<<"Finished writing the inverted index file"<<endl<<endl;
    //testing index_reader
    
//    cout<<"enter an int to proceed"<<endl;
//    cin>>proceed;
//    string filename2 = "origin_index.txt";
//    cout<<"start class index_reader"<<endl;
//    index_reader index_reader_o2(filename2);
//    index_reader_o2.print_pairs();
//    cout<<"index reader tested!"<<endl;
//    
    cout<<"enter an int to proceed"<<endl;
    cin>>proceed;
//    //test writer for index_reader
//    cout<<"start writing index"<<endl;
    class writer writer2("xt",1000);
    writer2.write_from_index1("origin_index.txt");
    cout<<"Finished writing the final index file"<<endl<<endl;

    //testing the tree
    cout<<"test the tree start here"<<endl;
    cout<<"enter an int to proceed"<<endl;
    cin>>proceed;
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
    
    cout<<"parseline + merging part  "<<endl;
    cout<<"enter an int to proceed"<<endl;
    cin>>proceed;


    fstream myfile;
    myfile.open("origin_index.txt");
    string word;
    string line;
    std::getline(myfile, line);
    database dbs1;
    std::vector<std::pair<string, string> > holder;
    while(myfile){
        std::getline(myfile, line);
        dbs1.parse_line(holder, line);
    }
    for( std::vector<std::pair <string, string> >::iterator it = holder.begin(); it != holder.end(); ++it ){
        cout<<" "<<it->first<<" "<<it->second<<endl;
    }
    
    dbs.merge("sec_file.txt", &writer2);
    database dbs3;
    writer2.write_from_index1("origin_index.txt");
    dbs.load("final_index.txt");
    //    return 0;
    

    
    

    return 0;
};

//============================================================================
// Name        : 174b.cpp
// Author      : george
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include<string>
#include<iostream>
//#include"console.h"
#include"bpt.h"
#include"time.h"
#include"database.h"
#include "command_checker.h"
#include<vector>
#include "sorter.h"
#include "predefined.h"
using namespace std;

int main(void) {
    
	std::string quit("quit");
	std::string build("build_index");
	std::string load("load");
	std::string merge("merge");
	std::string insert("insert");
	std::string delete_key("delete");
    std::string count("count");
    std::string search("search");
    std::string printpath("printpath");
    std::string page_i("page");
    std::string range("range");
    std::string search_test("search_test");
    command_checker command_check;
    database dbs;
    
    int page_num;
    cout<<"Please set the number of pages for the buffer! Input an integer. "<<endl;
    cin>>page_num;//modify??
	//console console1(8000);
    //sample code for using vector, pair, etc
    typedef std::pair <string, string> stringPair;
    typedef std::pair <int, string> int_stringPair;

    std::string command;
	while(std::getline(cin,command)){
	    //timer start
        std::clock_t start;
	    double duration;
		start = std::clock();
        //check command
        vector<string> parsed_command;//make it a list, no need
        
        if(command_check.check(command)){
            parsed_command= command_check.parse_command(command);//more in parse_command
            cout<<"size of the vector"<<parsed_command.size()<<endl;
            for (std::vector<string>::iterator it = parsed_command.begin() ; it != parsed_command.end(); ++it)
                //std::cout<<*it<<" "<<"end ";
            cout<<endl; //testing parse_command
        }
        else{
            cout<<"there might be some typo in your command"<<endl;
        }
        string command_1;
        if(!parsed_command.empty())
             command_1= parsed_command.front();
        
		if(command_1.compare(quit)==0){
			cout<<"Quiting ..."<<endl;
			duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
			cout<<"duration is "<<duration<<endl;
			break;
		}else if(command_1.compare(build)==0){
			cout<<"starting to build the index file"<<endl;
            //put reader and writer in console, call console.build(....).
            //console1.build("text.txt","origin_index",1000);
            int page_size=1000;
            string text_file = "yelp_final_dataset";
            string index_file = "origin_index.txt";
            cout<<"start class origin read"<<endl;
            origin_reader original_reader(text_file);
            cout<<"Before sorting"<<endl<<endl;
            //original_reader.print_pairs();
            sorter sorter;
            sorter.sortvec( &original_reader );
            cout<<"After sorting"<<endl<<endl;
            original_reader.duplicate_eliminate();
            //original_reader.print_pairs();
            writer writer(index_file,page_size);
            writer.write( &original_reader );// 写出index1文件
            original_reader.clear();//empty the vector
            cout<<"Finished writing the inverted index file"<<endl<<endl;
			duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
			cout<<"duration is "<<duration<<endl;
		}else if(command_1.compare(load)==0){
			cout<<"loading..."<<endl;
            class writer writer2("xt",1000);
            writer2.write_from_index1("origin_index.txt");
            cout<<"Finished writing the final index file"<<endl;
            //dbs.test_load(&index_reader_o2);
            dbs.load("final_index.txt");
			duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
			cout<<"duration is "<<duration<<endl;
        }else if(command_1.compare(merge)==0){
            cout<<"merging..."<<endl;
            class writer writer2("xt",1000);//page_size is 1000
            dbs.merge("sec_file.txt", &writer2);
            duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
            cout<<"duration is "<<duration<<endl;
        }
        else if(command_1.compare(insert)==0){// to do
			cout<<"inserting..."<<endl;
            class writer writer2("xt",1000);//page_size is 1000
            dbs.insert_key_doc("couple", "DOC1", &writer2, "origin_index.txt");
            writer2.write_from_index1("origin_index.txt");
            dbs.load("final_index.txt");
            //load the b+ index again
            
			duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
			cout<<"duration is "<<duration<<endl;
        }else if(command_1.compare(delete_key)==0){//to do
            cout<<"deleting <key,doc> pair..."<<endl;
            class writer writer2("xt",1000);//page_size is 1000
            dbs.delete_document("DOC1", &writer2, "origin_index.txt");
            writer2.write_from_index1("origin_index.txt");
            dbs.load("final_index.txt");
            duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
            cout<<"duration is "<<duration<<endl;
            
        }else if(command_1.compare(count)==0){
            cout<<"counting occurence of the key..."<<endl;
            class writer writer2("xt",1000);//page_size is 1000
            char key[16]={"but"};
            int counter = dbs.count(key, &writer2);
            cout<<"the number of occurrence: "<<counter<<endl;
            duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
            cout<<"duration is "<<duration<<endl;
        }else if(command_1.compare(search)==0){
            cout<<"searching the a key or multiple keys..."<<endl;
            class writer writer2("xt",1000);//page_size is 1000
            char key[16]={"but"};
            dbs.search(key, &writer2);
            duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
            cout<<"duration is "<<duration<<endl;//damn search multiple here!! for loop
            // dont forget to convert from string to char*
            
        }else if(command_1.compare(printpath)==0){
            cout<<"tracing the path of the key... "<<endl;
            class writer writer2("xt",1000);//page_size is 1000
            char key[16]={"but"};
            dbs.printpath(key);
            duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
            cout<<"duration is "<<duration<<endl;
            
        }else if(command_1.compare(page_i)==0){
            cout<<"looking at content at the specified page..."<<endl;
            class writer writer2("xt",1000);//page_size is 1000
            dbs.page(4, &writer2);
            duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
            cout<<"duration is "<<duration<<endl;
        }else if(command_1.compare(range)==0){
            cout<<"range query..."<<endl;
            char key1[16]={"but"};
            char key2[16]={"difference"};
            dbs.range_search(key1, key2);
            duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
            cout<<"duration is "<<duration<<endl;
        }
        

	}


}

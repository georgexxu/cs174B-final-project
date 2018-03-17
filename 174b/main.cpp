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
#include "external_sorter.h";
using namespace std;

int main(void) {
    //    string weird = "  12key";
    //    if(std::isspace(weird.at(0))){
    //        cout<<weird<<endl;
    //    }
    //    return 0;
    ////
    std::string quit("quit");
    std::string build("build_index");
    std::string load("load");
    std::string merge("merge");
    std::string insert("insert");
    std::string delete_doc("delete");
    std::string count("count");
    std::string search("search");
    std::string printpath("printpath");
    std::string page_i("page");
    std::string range("range");
    std::string search_test("search_test");
    std::string rollback("rollback");
    std::string external("external_sort");
    command_checker command_check;
    database dbs;
    logg log("log.txt");
    
    int page_num = 8096;//by default
    int page_size = 8096;//by default
    string g_index_file;
    cout<<endl;cout<<endl;
    cout<<"====================================================================="<<endl;
    cout<<"Welcome to use the database designed by:"<<endl<<"George, Xiaofeng Xu"<<endl<<"Aden, Yuting Pang"<<endl<<endl;
    
    //    string num;
    //    cin>>page_num;//modify?? yes
    //console console1(8000);
    //sample code for using vector, pair, etc
    typedef std::pair <string, string> stringPair;
    typedef std::pair <int, string> int_stringPair;
    std::string command;
    int first_time = 1;
    cout<<"Please set the number of pages for the buffer! Input an integer. "<<endl;
    while(std::getline(cin,command)){
        //timer start
        cout<<"Now, please type in your command! Always build before load!"<<endl;
        std::clock_t start;
        double duration;
        start = std::clock();
        //check command
        vector<string> parsed_command;//make it a list, no need
        string command_1;
        if(first_time){
            if(std::isdigit(command.at(0))){
                page_num = stoi(command);
                first_time = 0;
                continue;
            }
            cout<<"please provide an integer"<<endl;
            continue;
        }else{//not first input
            //build the UI
            cout<<endl<<endl;
            //            cout<<"Please Enter the command and press ENTER. Please pay attention to the format: "<<endl;
            //            cout<<"1. Build Index: "<<endl<<"\t"<<"build_index data_file_name index_file_name page_size_in_integer"<<endl;
            
            
            if(command_check.check(command)){
                //cout<<"start parsing"<<endl;
                parsed_command= command_check.parse_command(command);//more in parse_command
                //cout<<"size of the vector"<<parsed_command.size()<<endl;
                //                for (std::vector<string>::iterator it = parsed_command.begin() ; it != parsed_command.end(); ++it)
                //                    std::cout<<*it<<" "<<"end ";
                //cout<<endl; //testing parse_command
            }else{
                cout<<"there might be some typo in your command"<<endl;
            }
            if(!parsed_command.empty())
                command_1= parsed_command.front();
            
            if(command_1.compare(quit)==0){
                cout<<"Quiting ..."<<endl;
                duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
                cout<<"duration is "<<duration<<endl;
                break;
            }else if(command_1.compare(build)==0){
                //put reader and writer in console, call console.build(....).
                //console1.build("text.txt","origin_index",1000);
                if(parsed_command.size()>=4){//build_index text index -p size
                    cout<<"starting to build the index file"<<endl;
                    string text_file = "yelp_final_dataset";
                    string index_file = "origin_index.txt";
                    
                    std::vector<string>::iterator it = parsed_command.begin();
                    text_file = parsed_command.at(1);
                    index_file =parsed_command.at(2);
                    //g_index_file = index_file;
                    if(parsed_command.size()>=5)//empty space cause problem
                        page_size = stoi(parsed_command.at(4));//potential fault
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
                }else{
                    cout<<"No enought arguments"<<endl;
                }
                
            }else if(command_1.compare(load)==0){
                if(parsed_command.size()>=2){
                    cout<<"loading..."<<endl;
                    string index1 = parsed_command.at(1);
                    g_index_file =index1;
                    cout<<index1<<endl;
                    class writer writer2("xt",page_size);
                    writer2.write_from_index1(index1);
                    cout<<"Finished writing the final index file"<<endl;
                    //dbs.test_load(&index_reader_o2);
                    dbs.load("final_index.txt");
                    duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
                    cout<<"duration is "<<duration<<endl;
                }
            }else if(command_1.compare(merge)==0){
                if(parsed_command.size()>=2){
                    cout<<"merging..."<<endl;
                    string sec_index_f = parsed_command.at(1);
                    class writer writer2("xt",page_size);//page_size is 1000
                    dbs.merge(g_index_file,sec_index_f, &writer2);
                    writer2.write_from_index1(g_index_file);
                    dbs.load("final_index.txt");
                    duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
                    cout<<"duration is "<<duration<<endl;
                }
            }
            else if(command_1.compare(insert)==0){// to do
                if(parsed_command.size()>=3){
                    cout<<"inserting..."<<endl;
                    class writer writer2("xt",page_size);//page_size is 1000
                    string keyword =parsed_command.at(1);
                    string doc_name =parsed_command.at(2);
                    dbs.insert_key_doc(keyword, doc_name, &writer2, g_index_file,&log);
                    log.write_insert_log(keyword, doc_name);
                    writer2.write_from_index1(g_index_file);
                    dbs.load("final_index.txt");
                    //load the b+ index again
                    duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
                    cout<<"duration is "<<duration<<endl;
                }else{
                    cout<<"error, please insert a <keyword,docname> pair"<<endl;
                }
            }else if(command_1.compare(delete_doc)==0){//to do
                if(parsed_command.size()>=2){
                    cout<<"deleting all pair in this doc..."<<endl;
                    class writer writer2("xt",page_size);//page_size is 1000
                    string doc_name = parsed_command.at(1);
                    dbs.delete_document(doc_name, &writer2, g_index_file,&log);
                    
                    writer2.write_from_index1(g_index_file);
                    dbs.load("final_index.txt");
                    duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
                    cout<<"duration is "<<duration<<endl;
                }
            }else if(command_1.compare(count)==0){
                if(parsed_command.size()>=2){
                    cout<<"counting occurence of the key..."<<endl;
                    class writer writer2("xt",page_size);//page_size is 1000
                    string keyword = parsed_command.at(1);
                    keyword = keyword.substr(0,15);
                    char key[16];
                    std::strcpy(key,keyword.c_str());
                    int counter = dbs.count(g_index_file,key, &writer2);
                    cout<<"the number of occurrence: "<<counter<<endl;
                    duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
                    cout<<"duration is "<<duration<<endl;
                }
            }else if(command_1.compare(search)==0){
                if(parsed_command.size()>=2){
                    cout<<"searching the a key or multiple keys..."<<endl;
                    for(int i = 0;i<parsed_command.size()-1;i++){
                        class writer writer2("xt",page_size);//page_size is 1000
                        string keyword =parsed_command.at(i+1);
                        keyword=keyword.substr(0,15);
                        char key[16];
                        std::strcpy(key,keyword.c_str());
                        dbs.search(g_index_file,key, &writer2);
                    }
                    duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
                    cout<<"duration is "<<duration<<endl;//damn search multiple here!! for loop
                    // dont forget to convert from string to char*
                }
            }else if(command_1.compare(printpath)==0){
                if(parsed_command.size()>=2){
                    cout<<"tracing the path of the key... "<<endl;
                    class writer writer2("xt",page_size);//page_size is 1000
                    string keyword = parsed_command.at(1);
                    keyword = keyword.substr(0,15);
                    char key[16];
                    std::strcpy(key,keyword.c_str());
                    dbs.printpath(key);
                    duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
                    cout<<"duration is "<<duration<<endl;
                }
            }else if(command_1.compare(page_i)==0){
                string num = parsed_command.at(1);
                if(std::isdigit(num.at(0))){
                    int i = stoi(num);
                    cout<<"looking at content at the specified page..."<<endl;
                    class writer writer2("xt",page_size);//page_size is 8096 by default
                    dbs.page(g_index_file,i, &writer2);
                    duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
                    cout<<"duration is "<<duration<<endl;
                }else{
                    cout<<"invalid page number"<<endl;
                }
            }else if(command_1.compare(range)==0){
                if(parsed_command.size()>=2){
                    cout<<"range query..."<<endl;
                    string keyword1 = parsed_command.at(1);
                    string keyword2 = parsed_command.at(2);
                    keyword1 = keyword1.substr(0,15);
                    keyword2 = keyword2.substr(0,15);
                    char key1[16];
                    char key2[16];
                    std::strcpy(key1,keyword1.c_str());
                    std::strcpy(key2,keyword2.c_str());
                    dbs.range_search(key1, key2);
                    duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
                    cout<<"duration is "<<duration<<endl;
                }
            }else if(command_1.compare(rollback)==0){
                string num = parsed_command.at(1);
                //                cout<<"num:"<<num;
                if(std::isdigit(num.at(0))){
                    cout<<"rollback..."<<endl;
                    int n = stoi(num);
                    class writer writer2("xt",page_size);
                    dbs.rollback(n, &writer2, g_index_file, &log);
                    duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
                    cout<<"duration is "<<duration<<endl;
                }else{
                    cout<<"invalid page number"<<endl;
                }
                
            }else if(command_1.compare(external)){
                cout<<"start external sorting..."<<endl;
                //int page_num = 200;
                string filename = "external.txt";
                external_sorter ext(200,2000);//page_num, page_size
                writer wt("txt",1000);
                ext.pass_0_sort(filename,&wt);
                ext.merge_sorting("p0sorted.txt");
                duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
                remove("temp2.txt");
                cout<<"duration is "<<duration<<endl;
            }
            
        }
    }
    
    
}

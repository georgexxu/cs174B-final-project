/*
 * database.h
 *
 *  Created on: Feb 23, 2018
 *      Author: georgexu
 */

#ifndef DATABASE_H_
#define DATABASE_H_
#include "bpt.h"
#include "writer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index_reader.h"
#include "predefined.h"
#include <fstream>
#include <ostream>
#include<algorithm>
#include "log.h"
using bpt::bplus_tree;

class database{
    
private:
    bplus_tree tree;
    
public:
    database():tree("test.db",true){
        
    };
    
    void test_load(index_reader* p_index_reader){
        for( std::vector<std::pair <string, int> >::iterator it = p_index_reader->string_int_pairs.begin(); it != p_index_reader->string_int_pairs.end(); ++it )
        {
            char key[16];
            const string string1(it->first);
            std::strcpy(key,string1.c_str());
            tree.insert(key, it->second);
        }
        //manually insert some keys
        char key1[16]={"feng"};
        tree.insert(key1, 2);
        char key2[16]={"yuting"};
        tree.insert(key2, 4);
    };//insert to bplus tree
    
    //    void load(string filename,writer * p_writer){//load directly from the disk
    //        //open file
    //        //insert entries to the tree
    //        //close file
    //        fstream myfile;
    //        myfile.open(filename);
    //        if(!myfile){
    //            cout<<"Cannot open input file...loading failed"<<endl;
    //            return;
    //        }
    //        cerr<<"readingfile starts"<<endl;
    //        int line_counter = 1;
    //        while(myfile){
    //            string line = "";
    //            string keyword = "";
    //            string page_num = "";
    //            char delim = ' ';
    //            char key[16];
    //            int count_even_odd = 1; //1 is odd
    //            std::getline(myfile,line); // get the line
    //            cerr<<"after getting aline "<<endl;
    //            auto i = 0;
    //            cerr<<"getting line: "<<endl;
    //            auto pos = line.find(delim);
    //            while(pos!=string::npos){
    //                if(count_even_odd ==1){//keyword part
    //                    keyword=line.substr(i,pos-i);
    //                    cout<<"keyword: "<<keyword<<"  ";
    //                    keyword.erase(std::remove(keyword.begin(), keyword.end(), '\\'), keyword.end());
    //                    std::strcpy(key,keyword.c_str());
    //                    i = ++pos;//keep reading the line
    //                    pos = line.find(delim,pos);
    //                    count_even_odd = 0;
    //                }else{// page number part
    //                    page_num=line.substr(i,pos-i);
    //                    cout<<"page_num: "<<page_num<<endl;
    //                    int page_number = stoi(page_num);///potential danger
    //                    tree.insert(key, page_number);
    //                    if(pos > (p_writer->page_size - 30)){
    //                        break;
    //                    }
    //                    i = ++pos;//keep reading the line
    //                    pos = line.find(delim,pos);
    //                    count_even_odd = 1;
    //                }
    //
    //            }
    //            line_counter++;
    //        }
    //        myfile.close();
    //    }
    
    
    void load(string filename){//load index2 directly from the disk
        //open file
        //insert entries to the tree
        //close file
        
        tree.init_from_empty();
        fstream myfile;
        myfile.open(filename);
        if(!myfile){
            cout<<"Cannot open input file...loading failed"<<endl;
            return;
        }
        string keyword;
        string num;
        char key[16];
        while(myfile){
            
            myfile>>keyword;
            keyword.erase(std::remove(keyword.begin(), keyword.end(), '\\'), keyword.end());
            keyword = keyword.substr(0,15);
            std::strcpy(key,keyword.c_str());
            myfile>>num;
            try {
                
                int page_num = stoi(num);
                //cout<<"keyword: "<<keyword<<" leng: "<<keyword.length()<<" page: "<<num<<endl;
                tree.insert(key, page_num);
            }catch(const char* msg){
                cerr << msg << endl;
            }
            
        }
        
    }
    
    void parse_line(std::vector<std::pair <string, string> > &string_str_pairs1,string line1){
        char delim = ' ';
        auto i1 = 0;
        auto pos1 = line1.find(delim);
        string keyword("");
        string doc("");
        string page_num("");
        int count_even_odd1 = 1; //1 is odd
        //cout<<"  pos: "<<pos<<endl;
        while(pos1!=string::npos){
            if(count_even_odd1 ==1){//keyword part
                keyword=line1.substr(i1,pos1-i1);
                //cout<<"keyword: "<<keyword<<endl;
                i1 = ++pos1;//keep reading the line
                pos1 = line1.find(delim,pos1);
                count_even_odd1 = 0;
            }else{
                doc=line1.substr(i1,pos1-i1);
                //cout<<"doc: "<<doc<<endl;
                stringPair s_s_pair(keyword,doc);
                string_str_pairs1.push_back(s_s_pair);
                i1 = ++pos1;//keep reading the line
                pos1 = line1.find(delim,pos1);
                count_even_odd1 = 1;
            }
        }
        
    }
    
    void merge(string g_index_file,string second_file,writer *p_writer){
        //merge the current index file with the second index file
        int page_size = p_writer->page_size;
        string current_name = g_index_file;//should be global to some extent
        fstream current_file;
        fstream sec_file;
        ofstream o_file;
        current_file.open(current_name);
        sec_file.open(second_file);
        o_file.open("temperary_name.txt");
        
        if(!sec_file){
            cout<<"Cannot find the file you want to merge, please try again"<<endl;
            return;
        }
        //1 . sort the two sorted file, i.e current and sec_file
        std::vector<std::pair <string, string> >string_str_pairs1;
        std::vector<std::pair <string, string> >string_str_pairs2;
        std::pair <string, string> smallerpair;
        while(sec_file){
            string line1 = "";
            string line2 = "";
            if(string_str_pairs1.empty()){
                std::getline(sec_file,line1); // get the line
                parse_line(string_str_pairs1, line1);
            }
            if(string_str_pairs2.empty()){
                if(current_file){
                    std::getline(current_file,line2); // get the line
                    parse_line(string_str_pairs2, line2);
                }else{//when current is empty,用完所有sec
                    if(string_str_pairs1.empty()){
                        while(sec_file){
                            std::getline(sec_file,line1); // get the line
                            parse_line(string_str_pairs1, line1);
                            for( std::vector<std::pair <string, string> >::iterator it = string_str_pairs1.begin(); it != string_str_pairs1.end(); ++it ){
                                long position = o_file.tellp();
                                if (position % page_size > page_size - 100){
                                    //outputfile<<"\n";
                                    o_file.seekp((position/page_size+1)*page_size-1);
                                    o_file<<"\n";
                                }
                                o_file<<it->first<<" "<<it->second<<" ";
                            }
                            string_str_pairs1.clear();
                        }
                    }else{
                        while(!string_str_pairs1.empty()){
                            long position = o_file.tellp();
                            if (position % page_size > page_size - 100){
                                //outputfile<<"\n";
                                o_file.seekp((position/page_size+1)*page_size-1);
                                o_file<<"\n";
                                
                            }
                            o_file<<string_str_pairs1.begin()->first<<" "<<string_str_pairs1.begin()->second<<" ";
                            string_str_pairs1.erase(string_str_pairs1.begin());
                        }
                    }
                }
            }
            //compare
            while(!string_str_pairs1.empty()&&!string_str_pairs2.empty()){
                std::pair <string, string> front1 = string_str_pairs1.front();
                std::pair <string, string> front2 =string_str_pairs2.front();
                if(front1.first<=front2.first){
                    smallerpair = front1;
                    string_str_pairs1.erase(string_str_pairs1.begin());
                }else{
                    smallerpair = front2;
                    string_str_pairs2.erase(string_str_pairs2.begin());
                }
                long position = o_file.tellp();
                if (position % page_size > page_size - 100){
                    //outputfile<<"\n";
                    o_file.seekp((position/page_size+1)*page_size-1);
                    o_file<<"\n";
                }
                o_file<<smallerpair.first<<" "<<smallerpair.second<<" ";
            }
            
        }
        if(current_file){//if still not empty
            while(!string_str_pairs1.empty()){
                long position = o_file.tellp();
                if (position % page_size > page_size - 100){
                    //outputfile<<"\n";
                    o_file.seekp((position/page_size+1)*page_size-1);
                    o_file<<"\n";
                    
                }
                o_file<<string_str_pairs1.begin()->first<<" "<<string_str_pairs1.begin()->second<<" ";
                string_str_pairs1.erase(string_str_pairs1.begin());
                
            }
            while(!string_str_pairs2.empty()){
                long position = o_file.tellp();
                if (position % page_size > page_size - 100){
                    //outputfile<<"\n";
                    o_file.seekp((position/page_size+1)*page_size-1);
                    o_file<<"\n";
                }
                o_file<<string_str_pairs2.begin()->first<<" "<<string_str_pairs2.begin()->second<<" ";
                string_str_pairs2.erase(string_str_pairs2.begin());
            }
            while(current_file){
                string line2("");
                std::getline(current_file,line2); // get the line
                parse_line(string_str_pairs2, line2);
                while(!string_str_pairs2.empty()){
                    
                    long position = o_file.tellp();
                    if (position % page_size > page_size - 100){
                        //outputfile<<"\n";
                        o_file.seekp((position/page_size+1)*page_size-1);
                        o_file<<"\n";
                        
                    }
                    o_file<<string_str_pairs2.begin()->first<<" "<<string_str_pairs2.begin()->second<<" ";
                    string_str_pairs2.erase(string_str_pairs2.begin());
                }
            }
            
        }
        current_file.close();
        sec_file.close();
        o_file.close();
        char filename_char[30];
        std::strcpy(filename_char,g_index_file.c_str());
        remove(filename_char);
        rename("temperary_name.txt",filename_char);
        //2. update index2
        //3. load again
        //load("final_index.txt",p_writer);//should clear before loading
    }
    
    void search(char * key,writer *p_writer){//use the tree search
        bpt::value_t value;
        string keyword(key);
        if (tree.search(key, &value) != 0)
            printf("Key %s not found\n", key);
        else{
            printf("%d\n", value);
            int page_num = value-1;
            fstream myfile;
            string current_file="origin_index.txt";
            myfile.open(current_file);
            myfile.seekp(page_num*p_writer->page_size);
            long pos = myfile.tellp();
            //cout<<"current pos: "<<pos<<endl;
            std::vector<std::pair <string, string> >string_str_pairs1;
            while(myfile){
                string line = "";
                std::getline(myfile,line);
                parse_line(string_str_pairs1, line);
                for(std::vector<std::pair <string, string> >::iterator it = string_str_pairs1.begin(); it != string_str_pairs1.end(); ++it ){
                    //cout<<it->first<<" ";
                    if(it->first==keyword){
                        cout<<it->second<<" ";
                    }
                    if(it==(string_str_pairs1.end()-1)){
                        if(it->first!=keyword){
                            cout<<"finished searching"<<endl;
                            return;
                        }
                    }
                }
            }
        }
        //todo, read from disk
        //****also from this number search all the docnames and cout them
    };
    
    int count(char * key,writer *p_writer){//use the tree search
        bpt::value_t value;
        string keyword(key);
        int counter = 0;
        if (tree.search(key, &value) != 0){
            printf("Key %s not found\n", key);
            return counter;
        }
        else{
            printf("%d\n", value);
            int page_num = value-1;
            fstream myfile;
            string current_file="origin_index.txt";
            myfile.open(current_file);
            myfile.seekp(page_num*p_writer->page_size);
            long pos = myfile.tellp();
            //cout<<"current pos: "<<pos<<endl;
            std::vector<std::pair <string, string> >string_str_pairs1;
            while(myfile){
                string line = "";
                std::getline(myfile,line);
                parse_line(string_str_pairs1, line);
                for(std::vector<std::pair <string, string> >::iterator it = string_str_pairs1.begin(); it != string_str_pairs1.end(); ++it ){
                    //cout<<it->first<<" ";
                    if(it->first==keyword){
                        cout<<it->second<<" ";
                        counter++;
                        cout<<"count: "<<counter<<endl;
                    }
                    if(it==(string_str_pairs1.end()-1)){
                        if(it->first!=keyword){
                            cout<<"finished counting"<<endl;
                            return counter;
                        }
                    }
                }
            }
            return counter;
        }
        
    };
    
    void printpath(char* keyword){
        bpt::value_t value;
        if (tree.search(keyword, &value) != 0)
            printf("Key %s not found\n", keyword);
        else{
            //printf("%d\n", value);
        }
        cout<<"The keyword can be found in page "<<value<<endl;
    }
    void page(int i,writer * p_writer){//display things in page i
        if(i>0){
            string line;
            int page_num = i-1;
            fstream myfile;
            string current_file="origin_index.txt";
            myfile.open(current_file);
            myfile.seekp(page_num*p_writer->page_size);
            long pos = myfile.tellp();
            cout<<"current pos: "<<pos<<endl;
            std::getline(myfile,line);
            //std::vector<std::pair <string, string> >string_str_pairs1;
            cout<<"The content in page "<<i<<" is: "<<endl;
            cout<<line;
            
        }else{
            cout<<"error, invalid input, page number must be greater than 1"<<endl;
        }
        
    }
    
    
    void range_search(char *key1,char * key2){
        bpt::key_t start(key1);
        bpt::value_t values[512];
        bool next = true;
        while (next) {
            int ret = tree.search_range(
                                        &start, key2, values, 512, &next);
            if (ret < 0)
                break;
            //            for (int i = 0; i < ret; i++)
            //                printf("%d\n", values[i]);
        }//todo
        //cout all the keyword inbetween,later
    }
    
    
    void insert_key_doc(string key,string doc, writer* p_writer, string filename, logg* p_log){
        ifstream myfile;
        ofstream outputfile;
        string line;
        string pre_line;
        char key_char[16];
        std::strcpy(key_char,key.c_str());
        unsigned long page_pos=0;
        //        long start_pos = -1; //set as 1 first
        
        myfile.open(filename);  //index 1 file name
        outputfile.open("new_origin_index.txt");
        if(!myfile){
            cout<<"Cannot open input file"<<endl;
        }
        //seek for the insert page
        //        page_pos = -1;
        //        tree.search(key_char, &page_pos);
        //        cout<<page_pos<<endl;
        //
        //        if(page_pos==0){
        //            while (getline(myfile, line)){
        //                page_pos+=1;
        //            }
        //        }
        //
        //        cout<<page_pos<<endl;
        //
        
        //        while (getline(myfile, line)){
        ////                cout <<"scan through " << line << endl;
        //            if(key <= line.substr(0,line.find(" "))){
        ////                   cout<<"find start pos before "<<line<<endl;
        //                break;
        //            }else{
        //                page_pos= page_pos/p_writer->get_page_size();
        //            }
        //
        while (myfile>>line){
            if(key <= line ){
                break;
            }
            page_pos=  1+myfile.tellg()/p_writer->get_page_size();
            myfile.ignore(p_writer->get_page_size()+1,'\n');
        }
        //        page_pos = myfile.tellg();
        //        page_pos =page_pos-1;
        //        page_pos =page_pos/p_writer->get_page_size();
        //
        //            myfile.seekg(start_pos);
        //            cout<<"set to "<<start_pos<<endl;
        
        
        
        //            cout<<page_pos<<endl;
        
        
        // start to write the new file
        //        cout<<"start update the origin_index"<<endl;/////
        long c_pos=-1;
        myfile.close();
        myfile.open(filename);
        
        //copy the previous lines
        for(int i=0; i<page_pos-1; i++){
            getline(myfile,line);
            c_pos = myfile.tellg();
            //            cout<<line<<endl;
            outputfile<<line;
            outputfile<<"\n";
        }
        
        std::vector<std::pair <string, string> >string_str_pairs;
        stringPair s_s_pair(key,doc);
        string_str_pairs.push_back(s_s_pair);
        
        getline(myfile,line);
        //        cout<<endl<<line;
        parse_line(string_str_pairs, line);
        //        cout<<endl<<line;
        sort(string_str_pairs.begin(), string_str_pairs.end());
        for( std::vector<std::pair <string, string> >::iterator it = string_str_pairs.begin(); it != string_str_pairs.end(); it++ ){
            long position = outputfile.tellp();
            if (position % p_writer->get_page_size() > (p_writer->get_page_size() - 100)){
                //outputfile<<"\n";
                outputfile.seekp((position/p_writer->get_page_size()+1)*p_writer->get_page_size()-1);
                outputfile<<"\n";
            }
            //            cout<<endl<<it->first<<" "<<it->second<<" ";
            outputfile<<it->first<<" "<<it->second<<" ";
        }
        string_str_pairs.clear();
        
        while(getline(myfile,line)){
            parse_line(string_str_pairs, line);
            //            cout<<endl<<line;
            for( std::vector<std::pair <string, string> >::iterator it = string_str_pairs.begin(); it != string_str_pairs.end(); it++ ){
                long position = outputfile.tellp();
                if (position % p_writer->get_page_size() > (p_writer->get_page_size() - 100)){
                    //outputfile<<"\n";
                    outputfile.seekp((position/p_writer->get_page_size()+1)*p_writer->get_page_size()-1);
                    outputfile<<"\n";
                }
                //                cout<<endl<<it->first<<" "<<it->second<<" ";
                outputfile<<it->first<<" "<<it->second<<" ";
            }
            string_str_pairs.clear();
        }
        
        
        myfile.close();
        outputfile.close();
        //        cout<<"finish writing new origin index"<<endl;/////
        char filename_char[30];
        std::strcpy(filename_char,filename.c_str());
        remove(filename_char);
        rename("new_origin_index.txt", filename_char);
        p_writer->write_from_index1(filename);
        load("final_index.txt");
        
        
    }
    
    void delete_key_doc(string key, string doc, writer* p_writer, string filename, logg* p_log){
        ifstream myfile;
        ofstream outputfile;
        string line;
        
        myfile.open(filename);  //index 1 file name
        outputfile.open("new_origin_index.txt");
        
        if(!myfile){
            cout<<"Cannot open input file"<<endl;
        }
        
        std::vector<std::pair <string, string> >string_str_pairs;
        
        while(getline(myfile,line)){
            parse_line(string_str_pairs, line);
            //            cout<<endl<<line;
            for( std::vector<std::pair <string, string> >::iterator it = string_str_pairs.begin(); it != string_str_pairs.end(); ){
                
                //                cout<<endl<<"scan through "<<it->first<<" "<<it->second<<" ";
                if(it->second == doc and it->first == key){
                    //                    cout<<endl<<"delete "<<it->first<<" "<<it->second<<" ";
                    it = string_str_pairs.erase(it);
                }
                else
                    it++;
            }
            for( std::vector<std::pair <string, string> >::iterator it = string_str_pairs.begin(); it != string_str_pairs.end();it++ ){
                long position = outputfile.tellp();
                if (position % p_writer->get_page_size() > (p_writer->get_page_size() - 100)){
                    //outputfile<<"\n";
                    outputfile.seekp((position/p_writer->get_page_size()+1)*p_writer->get_page_size()-1);
                    outputfile<<"\n";
                }
                //                cout<<endl<<it->first<<" "<<it->second<<" ";
                outputfile<<it->first<<" "<<it->second<<" ";
            }
            string_str_pairs.clear();
        }
        
        myfile.close();
        outputfile.close();
        //        cout<<"finish writing new origin index"<<endl;/////
        
        
        char filename_char[30];
        std::strcpy(filename_char,filename.c_str());
        remove(filename_char);
        rename("new_origin_index.txt", filename_char);
        
        p_writer->write_from_index1(filename);
        load("final_index.txt");
        
    }
    
    void delete_document(string doc,writer* p_writer, string filename, logg* p_log){
        ifstream myfile;
        ofstream outputfile;
        string line;
        
        myfile.open(filename);  //index 1 file name
        outputfile.open("new_origin_index.txt");
        
        if(!myfile){
            cout<<"Cannot open input file"<<endl;
        }
        
        std::vector<std::pair <string, string> >string_str_pairs;
        unsigned long count = 0;
        while(getline(myfile,line)){
            parse_line(string_str_pairs, line);
            //            cout<<endl<<line;
            for( std::vector<std::pair <string, string> >::iterator it = string_str_pairs.begin(); it != string_str_pairs.end(); ){
                
                //                cout<<endl<<"scan through "<<it->first<<" "<<it->second<<" ";
                if(it->second == doc){
                    //                    cout<<endl<<"delete "<<it->first<<" "<<it->second<<" ";
                    it = string_str_pairs.erase(it);
                    count+=1;
                    p_log->write_delete_log(it->first,it->second);
                }
                else
                    it++;
            }
            for( std::vector<std::pair <string, string> >::iterator it = string_str_pairs.begin(); it != string_str_pairs.end();it++ ){
                long position = outputfile.tellp();
                if (position % p_writer->get_page_size() > (p_writer->get_page_size() - 100)){
                    //outputfile<<"\n";
                    outputfile.seekp((position/p_writer->get_page_size()+1)*p_writer->get_page_size()-1);
                    outputfile<<"\n";
                }
                //                cout<<endl<<it->first<<" "<<it->second<<" ";
                outputfile<<it->first<<" "<<it->second<<" ";
            }
            string_str_pairs.clear();
        }
        
        myfile.close();
        outputfile.close();
        cout<<endl<<"deleted "<<count<<" words in total"<<endl;
        cout<<"finish writing new origin index"<<endl;/////
        
        char filename_char[30];
        std::strcpy(filename_char,filename.c_str());
        remove(filename_char);
        rename("new_origin_index.txt", filename_char);
        
        p_writer->write_from_index1(filename);
        load("final_index.txt");
    }
    
    void rollback(int n, writer* p_writer, string filename, logg* p_log){
        ifstream myfile;
        string line;
        
        myfile.open(p_log->get_logname());  //logfilename
        vector<string> alllog;  //all the log records
        vector<string> logs;    //logs that need to be done
        while(getline(myfile, line)){
            alllog.push_back(line);
        }
        
        while(logs.size()<n and !alllog.empty()){
            line = alllog.back();
            if (line.find("*")==string::npos){
                logs.push_back(line);
            }
            alllog.pop_back();
        }
        
        vector<string> orders;
        unsigned long insert_count = 0;
        unsigned long delete_count = 0;
        while (!logs.empty()){
            line = logs.back();
            //                        cout<<"before parse: "<<line<<endl;
            string delimiter = " ";
            size_t pos = 0;
            string token;
            while ((pos = line.find(delimiter)) != std::string::npos) {
                token = line.substr(0, pos);
                //                std::cout << token << std::endl;
                orders.push_back(token);
                line.erase(0, pos + delimiter.length());
            }
            //            cout<<line<<endl;
            orders.push_back(line);
            //execute rollback here start
            if (orders.front() == "insert"){
                orders.erase(orders.begin());
                string key,doc;
                key = orders.front();
                doc = orders.back();
                delete_key_doc(key,doc,p_writer,filename,p_log);
                delete_count+=1;
            }
            else if (orders.front() == "delete"){
                orders.erase(orders.begin());
                string key,doc;
                key = orders.front();
                doc = orders.back();
                insert_key_doc(key,doc,p_writer,filename,p_log);
                insert_count+=1;
            }
            logs.pop_back();
            orders.clear();
        }
        p_log->write_rollback_log(n);
        cout<<endl<<"finish rollback "<<n<<" operation(s)"<<endl;
        cout<<"Including "<<insert_count<<" insertions and "<<delete_count<<" deletions"<<endl;
        myfile.close();
    }
    
    
    
    void search_test(char * key,writer *p_writer){//use the tree search
        bpt::value_t value;
        string keyword(key);
        if (tree.search(key, &value) != 0)
            printf("Key %s not found\n", key);
        else{
            printf("%d\n", value);
        }
    }
    
};






#endif /* DATABASE_H_ */

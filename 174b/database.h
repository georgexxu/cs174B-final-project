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
                    //
                    int page_num = stoi(num);
                    cout<<"keyword: "<<keyword<<" leng: "<<keyword.length()<<" page: "<<num<<endl;
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
    
    void merge(string second_file,writer *p_writer){
        //merge the current index file with the second index file
        int page_size = p_writer->page_size;
        string current_name("origin_index.txt");//should be global to some extent
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
        remove("origin_index.txt");
        rename("temperary_name.txt","origin_index.txt");
        //2. update index2
        //3. load again
        //load("final_index.txt",p_writer);//should clear before loading
        
        
    }
    
    void search(char * key){//use the tree search
        bpt::value_t value;
        if (tree.search(key, &value) != 0)
            printf("Key %s not found\n", key);
        else
            printf("%d\n", value);
        //todo, read from disk
        //****also from this number search all the docnames and cout them
    };
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
    void insert_key_doc(string key,string doc){
        
        
    //location: 3
        //
    }
    void delete_key_doc(string key, string doc){
        
    }
    int count(string keyword);
    
    int printpath(string keyword);
    
    void range (char* k1, char*  k2);//: Range query. Print all of the keywords between keyword1 and keyword2where keyword1 < keyword2


};






#endif /* DATABASE_H_ */

//
//  index_reader.hpp
//  174b
//
//  Created by Xiaofeng Xu on 24/02/2018.
//  Copyright © 2018 Xiaofeng Xu. All rights reserved.
//

#ifndef index_reader_h
#define index_reader_h

#include <stdio.h>
#include<string>
#include<iostream>
#include<fstream>
#include <stdio.h>
#include "origin_reader.h"

using namespace std;

typedef pair<int,int> int_int_pair;
typedef pair<string,int> string_int_pair;
class index_reader{//after sorting, get the <key,block number> pair and write to an index file
    friend class writer;
    friend class database;
private:
    //int page_size; this is determinded previously
    string filename;//sorted <int,string> pairs
    fstream myfile; 
   // std::vector<std::pair <int, int> >int_int_pairs;
    std::vector<std::pair <string, int> >string_int_pairs;
public:
    index_reader(origin_reader * p_origin_reader){//construtor
        //this->page_size = page_size;
    }
    index_reader(string filename){
        this->filename=filename;
        read_index_file1();
    }
//    void read_index_file1(){
//     //read it byte by byte, check number, assuming that writing separate pages by \n
//        myfile.open(filename);
//        if(!myfile){
//            cout<<"Cannot open input file"<<endl;
//        }
//        int line_counter = 1;
//        while(myfile){
//            string line = "";
//            string keyword = "";
//            char delim = ' ';
//            int count_even_odd = 1; //1 is odd
//            std::getline(myfile,line); // get the line
//            auto i = 0;
//            auto pos = line.find(delim);
//        
//            //cout<<"  pos: "<<pos<<endl;
//            while(pos!=string::npos){
//                if(count_even_odd ==1){//keyword part
//                    if(keyword.compare(line.substr(i,pos-i))!=0){
//                        keyword=line.substr(i,pos-i);
//                        if(keyword.length()!=0){
//                       // cout<<"keyword: "<<keyword<<endl;
//                        string_int_pair s_i_pair(keyword,line_counter);
//                        string_int_pairs.push_back(s_i_pair);
//                        }
//                    }
//                    i = ++pos;//keep reading the line
//                    pos = line.find(delim,pos);
//                    count_even_odd = 0;
//                }else{
//                    i = ++pos;//keep reading the line
//                    pos = line.find(delim,pos);
//                    count_even_odd = 1;
//                }
//            }
//            line_counter++;
//        }
//    }
    
    void print_pairs(){
        for (std::vector<pair<string,int> >::iterator it=string_int_pairs.begin();it != string_int_pairs.end();++it) {
            cout<<"<"<<it->first<<","<<it->second<<"> ";
        }
        cout<<endl;
    }
    void read_index_file1(){
        //read it byte by byte, check number, assuming that writing separate pages by \n
        if(!string_int_pairs.empty()){
            string_int_pairs.clear();
        }
        myfile.open(filename);
        if(!myfile){
            cout<<"Cannot open input file"<<endl;
        }
        int line_counter = 1;
        while(myfile){
            string line = "";
            string keyword = "";
            char delim = ' ';
            int count_even_odd = 1; //1 is odd
            std::getline(myfile,line); // get the line
            auto i = 0;
            auto pos = line.find(delim);
            
            //cout<<"  pos: "<<pos<<endl;
            while(pos!=string::npos){
                if(count_even_odd ==1){//keyword part
                    if(keyword.compare(line.substr(i,pos-i))!=0){
                        keyword=line.substr(i,pos-i);
                        if(keyword.length()!=0){
                            // cout<<"keyword: "<<keyword<<endl;
                            string_int_pair s_i_pair(keyword,line_counter);
                            string_int_pairs.push_back(s_i_pair);
                        }
                    }
                    i = ++pos;//keep reading the line
                    pos = line.find(delim,pos);
                    count_even_odd = 0;
                }else{
                    i = ++pos;//keep reading the line
                    pos = line.find(delim,pos);
                    count_even_odd = 1;
                }
            }
            line_counter++;
        }
    }
    
    
};

#endif /* index_reader_hpp */

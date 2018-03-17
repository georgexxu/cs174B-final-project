//
//  reader.hpp
//  174b
//
//  Created by Xiaofeng Xu on 24/02/2018.
//  Copyright © 2018 Xiaofeng Xu. All rights reserved.
//

#ifndef origin_reader_h
#define origin_reader_h

#include<string>
#include<iostream>
#include<fstream>
#include <stdio.h>
#include<algorithm>
#include<vector>
using namespace std;

typedef std::pair <string, string> stringPair;
typedef std::pair <int, string> int_stringPair;
class origin_reader{
    friend class sorter;
    friend class converter;
    friend class index_reader;
    friend class writer;
private:
    string filename;
    ifstream myfile;
    std::vector<std::pair <string, string> > string_pairs;

public:
    origin_reader(string filename){
       //count time
        std::clock_t start;
        double duration;
        start = std::clock();
        this->filename=filename;
        read_original_file();
       //time end
        duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
        cout<<"duration is "<<duration<<endl;
        
    }//initilize the vector
       // converter::converter converter;
    void read_original_file(){
        myfile.open(filename);
        if(!myfile){
            cout<<"Cannot open input file"<<endl;
        }
        int line_counter = 0;
        while(myfile){
            line_counter++;
//            if(line_counter>100000){
//                return;
//            }
            //cout<<"inside origin reader, reading my file"<<endl;
            string doc_name = "";
            string line = "";
            std::getline(myfile,line); // get the line
            //cout<<"gettingl line: "<<line<<endl;
            char delim = ' ';
            string item;
            auto i = 0;
            auto pos = line.find(delim);
            //cout<<"  pos: "<<pos<<endl;
            while(pos!=string::npos){
                if(i==0) {
                    doc_name = line.substr(i,pos-i);
                    //cout<<"doc name: "<<doc_name<<endl;
                }else{
                    item = line.substr(i,pos-i);
                    if(item.length()!=1 and item.length()!=0 and !(item.length()>16)and item.length()!=2){//if it is not a single character
                        stringPair s_pair(doc_name,item);
                        string_pairs.push_back(s_pair);
                    }
                }
                i = ++pos;
                pos = line.find(delim,pos);
                if(pos==string::npos){//if it is not a single character
                    item = line.substr(i,line.length());
                    if(item.length()!=1){
                        stringPair s_pair(doc_name,item);
                        string_pairs.push_back(s_pair);
                    }
                }
                    
            }
            //reference http://ysonggit.github.io/coding/2014/12/16/split-a-string-using-c.html
            
        }
        myfile.close();
        
    }//line by line read
    void duplicate_eliminate(){
        std::unique(string_pairs.begin(),string_pairs.end());
    }//needed for same key appearing twice in a line, this can be done easily later when things are sorted
    //convert

    void print_pairs(){

//        std::vector<pair<string,string> >::iterator it = string_pairs.end();
//        it--;
//        cout<<"<"<<it->first<<","<<it->second<<"> ";

        for (std::vector<pair<string,string> >::iterator it=string_pairs.begin();it != string_pairs.end();++it) {
            cout<<"<"<<it->first<<","<<it->second<<"> ";
        }
        cout<<endl;
    }
    void clear(){
        string_pairs.clear();
    }
    
};




#endif /* reader_hpp */

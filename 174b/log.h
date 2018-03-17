//
//  log.hpp
//  174b
//
//  Created by Pang Yuting on 3/13/18.
//  Copyright © 2018 Xiaofeng Xu. All rights reserved.
//

#ifndef log_hpp
#define log_hpp

#include<string>
#include<iostream>
#include<fstream>
#include <stdio.h>
#include <vector>


using namespace std;

class database;

class logg{
private:
    string filename;    //log file name
    ifstream myfile;
    
public:
    logg(string name){
        this->filename = name;
//         ofstream flush;
//         flush.open(filename, std::ofstream::out | std::ofstream::trunc);
//         flush.close();
    }
    
    string get_logname(){
        return filename;
    }
    void write_insert_log(string key, string doc){
        ofstream outputfile(filename, std::ios_base::app | std::ios_base::out);
        outputfile<<"insert"<<" "<<key<<" "<<doc<<"\n";
        outputfile.close();
    }
    
    void write_delete_log(string key, string doc){
        ofstream outputfile(filename, std::ios_base::app | std::ios_base::out);
        outputfile<<"delete"<<" "<<key<<" "<<doc<<"\n";
        outputfile.close();
    }
    
    void write_rollback_log(int n){
        ofstream outputfile("templog.txt");
        vector<string> buffer;
        string lines;
        int counter=0;
        myfile.open(filename);
        while (getline(myfile,lines)){
            buffer.push_back(lines);
        }
        vector<string> writebuffer;
        while(counter<n){
            lines = buffer.back();
            if(lines.find("*")!=string::npos){
                writebuffer.push_back(lines);
                buffer.pop_back();
            }else if(lines.find("*")==string::npos){
                writebuffer.push_back(lines+" "+"*");
                buffer.pop_back();
                counter+=1;
            }
        }
        while(!buffer.empty()){
            writebuffer.push_back(buffer.back());
            buffer.pop_back();
        }
        
        //        while(buffer.size() > (n+counter) ){
        //            outputfile<<buffer.front()<<"\n";
        //            buffer.erase(buffer.begin());
        //        }
        //        for(int i=0;i<n;i++){
        //            outputfile<<buffer.front()<<" "<<"*"<<"\n";
        //            buffer.erase(buffer.begin());
        //        }
        while(!writebuffer.empty() ){
            outputfile<<writebuffer.back()<<"\n";
            writebuffer.pop_back();
        }
        outputfile.close();
        remove("log.txt");
        rename("templog.txt", "log.txt");
        myfile.close();
        outputfile.close();
        
    }
    
};



#endif /* log_hpp */

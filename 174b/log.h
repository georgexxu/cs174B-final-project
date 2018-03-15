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



using namespace std;

class database;

class log{
private:
    string filename;    //log file name
    ifstream myfile;
    
public:
    log(){
        this->filename = "log.txt";
        ofstream flush;
        flush.open(filename, std::ofstream::out | std::ofstream::trunc);
        flush.close();
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
    
    
};



#endif /* log_hpp */

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

//#include "database.h"

using namespace std;



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
    
    void rollback(int n, database* p_dbs, writer* p_writer, string indexname){
        myfile.open(filename);
        string line;
        vector<string> logs;
        int line_counter = 0;
        while(getline(myfile, line)){
            if(line_counter == n) break;
            logs.push_back(line);
            line_counter++;
            cout<<"pushed "<<line<<endl;
        }
        
        vector<string> orders;
        
        while (!logs.empty()){
            line = logs.back();
//            cout<<"before parse: "<<line<<endl;
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
                p_dbs->delete_key_doc(key,doc,p_writer,indexname);
            }
            else if (orders.front() == "delete"){
                orders.erase(orders.begin());
                string key,doc;
                key = orders.front();
                doc = orders.back();
                            }
            
            logs.pop_back();
            orders.clear();
        }
    }
};



#endif /* log_hpp */

//
//  command_checker.h
//  174b
//
//  Created by Xiaofeng Xu on 24/02/2018.
//  Copyright © 2018 Xiaofeng Xu. All rights reserved.
//
#include<string>
#include<iostream>
#include<vector>
#include <sstream>

using namespace std;

#ifndef command_checker_h
#define command_checker_h


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



class command_checker{
private:
    int num;
    
public:
    command_checker(int num = 0){
        this->num = num;
    }//constructor
    
    bool check(string command){// to be implemented
        
        string cmd = command.substr(0, command.find(' '));
        cout<<"checking"<<endl;
        if (cmd.compare("load")==0 or cmd.compare("merge")==0
            or cmd.compare("insert")==0 or cmd.compare("delete")==0 or cmd.compare("count")==0 or cmd.compare("search")==0
            or cmd.compare("printpath")==0 or cmd.compare("range")==0 or cmd.compare("page")==0){
            
            this->num = 1;
            cout<<"checked"<<endl;
            return true;
        }
        else
            return false;
        
        
    }
    vector<string> parse_command(string command){//parse the command into different part for furthere processing
        
        std::vector<string> v;
        
        istringstream iss(command);
        
        do
        {
            string subs;
            iss >> subs;
            v.push_back(subs);
            
        } while (iss);
        cout<<"finished parse"<<endl;
        return v;
        
        
}//return a list of string
    int getnum(){
        return num;
    }
    //parse a command
    
};





#endif /* command_checker_h */

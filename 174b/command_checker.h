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
        if (cmd.compare("quit") or cmd.compare("build_index") or cmd.compare("load") or cmd.compare("merge")
            or cmd.compare("insert") or cmd.compare("delete") or cmd.compare("count") or cmd.compare("search")
            or cmd.compare("printpath") or cmd.compare("range") or cmd.compare("page")){
            
            this->num = 1;
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
        return v;
        
        
}//return a list of string
    int getnum(){
        return num;
    }
    //parse a command
    
};





#endif /* command_checker_h */

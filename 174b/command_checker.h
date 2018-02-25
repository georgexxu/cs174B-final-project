//
//  command_checker.h
//  174b
//
//  Created by Xiaofeng Xu on 24/02/2018.
//  Copyright © 2018 Xiaofeng Xu. All rights reserved.
//
#include<string>
#include<iostream>
using namespace std;

#ifndef command_checker_h
#define command_checker_h

class command_checker{
private:
    int num;
public:
    command_checker(int num = 0){
        this->num = num;
    }//constructor
    bool check(string command){// to be implemented
        
        cout<<"given command is"<<command<<endl;
        cout<<"checking command"<<endl;
        this->num = 1;
        return true;
    }
    string parse_command(string command){//parse the command into different part for furthere processing
        return command;
    }//return a list of string
    int getnum(){
        return num;
    }
    //parse a command
    
};





#endif /* command_checker_h */

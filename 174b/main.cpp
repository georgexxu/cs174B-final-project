//============================================================================
// Name        : 174b.cpp
// Author      : george
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include<string>
#include<iostream>
#include"console.h"
#include"bptree.h"
#include"time.h"
#include"database.h"
#include "command_checker.h"
#include<vector>
using namespace std;

int main(void) {
    
	std::string command;
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
    command_checker command_check;
    
	console console1(1);
    //sample code for using vector, pair, etc
    cout<<"testing string pairs and int string pairs"<<endl;
    typedef std::pair <string, string> stringPair;
    typedef std::pair <int, string> int_stringPair;
    stringPair string1("hello","xiaofeng");
    int_stringPair string2(1,"yuting");
    int_stringPair string3(2,"george");
    cout<<string1.first<<"  "<<string1.second<<endl;
    cout<<string2.first<<"  "<<string2.second<<endl;
    vector<std::pair <int,string> > pairs;
    pairs.push_back(string2);
    pairs.push_back(string3);
    
//    for (std::vector<int>::iterator it = myvector.begin() ; it != myvector.end(); ++it)
//        std::cout << ' ' << *it;
//    std::cout << '\n';
    
   //end of sample

	while(std::getline(cin,command)){
	    //timer start
        std::clock_t start;
	    double duration;
		start = std::clock();
        //check command
        vector<string> parsed_command;//make it a list
        
        if(command_check.check(command)){
            
            parsed_command= command_check.parse_command(command);//more in parse_command
                        

        }
        else{
            cout<<"there might be some typo in your command"<<endl;
        }
        
		if(command.compare(quit)==0){
			cout<<"this is us is the end"<<endl;
			duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
			cout<<"duration is "<<duration<<endl;
			break;
		}else if(command.compare(build)==0){
			cout<<"starting to build the index file"<<endl;
			duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
			cout<<"duration is "<<duration<<endl;
		}else if(command.compare(load)==0){
			cout<<console1.getpage();
			console1.load();
			cout<<"loading"<<endl;
			duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
			cout<<"duration is "<<duration<<endl;
		}else if(command.compare(insert)==0){
			cout<<"inserting"<<endl;
			duration = (std::clock()-start)/(double)CLOCKS_PER_SEC;
			cout<<"duration is "<<duration<<endl;
		}
	}


}

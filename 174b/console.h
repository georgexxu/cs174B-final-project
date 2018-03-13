/*

 * console.h
 *
 *  Created on: Feb 22, 2018
 *      Author: georgexu
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "database.h"
//#include "time.h"
#include<iostream>
using namespace std;
class console{

private:
    int num_of_pages;
    database dbs;

public:
    console(int page,int page_num):dbs(){
        num_of_pages = page;
    }
    
    void load (string filename){
        dbs.load(filename);
    }
    
    void merge();//: Merge the current index file with the second index file, and update the index file on the disk.
    void page(int i);//: Displays the contents of the ith page, directly access the disk
    void insert_doc();//use the database to search for the key, then insert at the specified location
    //call database.load at the end
    void delete_doc();//similar to the above
    int getpage(){
        return num_of_pages;
    }
    int count(string keyword){
        //Counts a keyword by printing the number of documents that contain this keyword
        return dbs.count(keyword);
        return 0;
    }
    void search_multiple_keys(string s[]){
//        for(int i =0; i < sizeof(s);i++){
//            dbs.search(s[i])
//        }
    }
    void printpath(){
        
    }
    void rollback(int n){
        
    }
};




#endif /* CONSOLE_H_ */

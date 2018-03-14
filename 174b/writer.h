//
//  writer.hpp
//  174b
//
//  Created by Xiaofeng Xu on 24/02/2018.
//  Copyright © 2018 Xiaofeng Xu. All rights reserved.
//

#ifndef writer_h
#define writer_h

#include<string>
#include<iostream>
#include<fstream>
#include <stdio.h>
#include "origin_reader.h"
#include "index_reader.h"
using namespace std;

class writer{
    friend class database;
private:
    int page_size; //use this to determine block size ,in bytes
    string filename;//name of the outputfile
    ofstream outputfile;
public:
    int get_page_size(){
        return page_size;
    }
    
    writer(string filename, int page_size){
        this->page_size = page_size;
        this->filename = filename;
        //initialize the fstream
    }

    void write(origin_reader* p_origin_reader){
        
        
        outputfile.open(filename);
        
        
        for( std::vector<std::pair <string, string> >::iterator it = p_origin_reader->string_pairs.begin(); it != p_origin_reader->string_pairs.end(); ++it )
        {
            long position = outputfile.tellp();
            //              cout<<"position before write"<<position<<endl;
            if (position % page_size > page_size - 100){
                //outputfile<<"\n";
                //                cout<<"new page"<<endl;
                outputfile.seekp((position/page_size+1)*page_size-1);
                outputfile<<"\n";
            }
            outputfile<< it->second <<" "<<it->first <<" ";
        }
        
        outputfile.close();
        
    }
    void write(index_reader* p_index_reader){
        
        outputfile.open(filename);
        for( std::vector<std::pair <string, int> >::iterator it = p_index_reader->string_int_pairs.begin(); it != p_index_reader->string_int_pairs.end(); ++it )
        {
            long position = outputfile.tellp();
            //              cout<<"position before write"<<position<<endl;
            if (position % page_size > page_size - 100){
                //outputfile<<"\n";
                //                cout<<"new page"<<endl;
                outputfile<<"\n";
                outputfile.seekp((position/page_size+1)*page_size);
                
            }
            outputfile<< it->first <<" "<<it->second <<" ";
        }
        
        outputfile.close();
    }
    
    void write_from_index1(string f_name){
        //read it byte by byte, check number, assuming that writing separate pages by \n
        ifstream myfile;
        myfile.open(f_name);

        outputfile.open("final_index.txt");
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
                            long position = outputfile.tellp();
                            //              cout<<"position before write"<<position<<endl;
                            if (position % page_size > page_size - 100){

//                                outputfile.seekp((position/page_size+1)*page_size-1);
//                                outputfile<<"\n";
                                for(int i =0;i<(page_size-(position % page_size)-1);i++){
                                        outputfile<<" ";
                                        if(i == (page_size-(position % page_size))-2)
                                        outputfile<<"\n";
                                }
                            }
                            outputfile<<keyword<<" "<<line_counter<<" ";
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
        myfile.close();
        outputfile.close();
    }
    
};

#endif /* writer_hpp */

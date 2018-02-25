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
private:
    int page_size; //use this to determine block size ,i.e number of lines
    string filename;//name of the outputfile
    ofstream outputfile;
public:
    writer(string filename, int page_size){
        this->page_size = page_size;
        this->filename = filename;
        //initialize the fstream
    }
    void write(origin_reader* p_origin_reader){
        
    }
    void write(index_reader* p_index_reader){
        
    }
    //void wrtie(vector)
    
};

#endif /* writer_hpp */

//
//  reader.hpp
//  174b
//
//  Created by Xiaofeng Xu on 24/02/2018.
//  Copyright © 2018 Xiaofeng Xu. All rights reserved.
//

#ifndef origin_reader_h
#define origin_reader_h

#include<string>
#include<iostream>
#include<fstream>
#include <stdio.h>
#include<algorithm>
#include<vector>
#include "converter.h"
using namespace std;

typedef std::pair <string, string> stringPair;
typedef std::pair <int, string> int_stringPair;
class origin_reader{
    friend class sorter;
    friend class converter;
    friend class index_reader;
    friend class writer;
private:
    string filename;
    ifstream myfile;
    std::vector<std::pair <string, string> > string_pairs;
    std::vector<std::pair <int, string> >int_string_pairs;
    converter converter;

public:
    origin_reader(string filename){//initilize the vector
        converter::converter converter;
        
        
    }
    void read_original_file(){//line by line read
        
    }
    void merge_each_line(){//needed for same key appearing twice in one doc
        
    }
    //convert
    
};





#endif /* reader_hpp */

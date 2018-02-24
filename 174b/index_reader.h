//
//  index_reader.hpp
//  174b
//
//  Created by Xiaofeng Xu on 24/02/2018.
//  Copyright © 2018 Xiaofeng Xu. All rights reserved.
//

#ifndef index_reader_h
#define index_reader_h

#include <stdio.h>
#include<string>
#include<iostream>
#include<fstream>
#include <stdio.h>
#include "origin_reader.h"

using namespace std;

typedef pair<int,int> int_int_pair;
class index_reader{//after sorting, get the <key,block number> pair and write to an index file
    friend class writer;
private:
    string filename;
    std::vector<std::pair <int, int> >int_string_pairs;
public:
    index_reader(){//construtor
        
    }
    
    void read_index_file1(origin_reader * p_origin_reader){
        
    }
    
    
};

#endif /* index_reader_hpp */

//
//  sorter.h
//  174b
//
//  Created by Xiaofeng Xu on 24/02/2018.
//  Copyright © 2018 Xiaofeng Xu. All rights reserved.
//

#ifndef sorter_h
#define sorter_h

#include<string>
#include <stdio.h>
#include<algorithm>
#include "origin_reader.h"
using namespace std;

class sorter{
private:
public:
    sorter();
    void sortvec(origin_reader * p_origin_reader){
        sort(p_origin_reader->int_string_pairs.begin(),p_origin_reader->int_string_pairs.end());
    }
    //void sort(vector)
};


#endif /* sorter_h */

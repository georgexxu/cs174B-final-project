//
//  sorter.h
//  174b
//
//  Created by Xiaofeng Xu on 24/02/2018.
//  Copyright © 2018 Xiaofeng Xu. All rights reserved.
//

#ifndef sorter_h
#define sorter_h

#include<iostream>
#include<string>
#include <stdio.h>
#include<algorithm>
#include "origin_reader.h"
using namespace std;

bool sortbysec(const pair<string,string> &a,
               const pair<string,string> &b)
{
    return (a.second < b.second);
}

class sorter{
friend class origin_reader;
public:
    
    
    
    void sortvec(origin_reader* p_origin_reader){
        
        
        
        sort(p_origin_reader->string_pairs.begin(),p_origin_reader->string_pairs.end(),sortbysec);
        cout<<"sorted"<<endl;
        
    }
    //void sort(vector)
};


#endif /* sorter_h */

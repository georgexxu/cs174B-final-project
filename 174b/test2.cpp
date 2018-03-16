//
//  tester.cpp
//  174b
//
//  Created by Xiaofeng Xu on 26/02/2018.
//  Copyright © 2018 Xiaofeng Xu. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include<string>
#include<iostream>
//#include"console.h"
//#include "origin_reader.h"
//#include "command_checker.h"
//#include "index_reader.h"
//#include "database.h"
//#include<vector>
//#include"sorter.h"
//#include"writer.h"
#include "external_sorter.h"
using namespace std;

int main(void) {
    int n_units = 10;
    int k_way = 3;
    int loop = ceil(static_cast<double>(n_units)/(k_way+0.0));
    cout<<"loop: "<<loop<<endl;
    return 0;
    int a ;
    int page_num = 1000;
    external_sorter ext(page_num,200);//page_num, page_size
    //testing origin_reader
    std::vector<std::pair <string, string> > string_str_pairs1;
    string line = "  ";
    writer wt("txt",1000);
    string filename = "yelp_final_dataset";
    //string line = "   ";
    if(!(line=="")){
        ext.parse_line(string_str_pairs1, line);
    }
    cout<<ext.quadratic_solver(page_num)<<endl;
    //  cout<<string_str_pairs1;
    //cout<<ext.count_pairs("yelp_final_dataset")<<endl;
    cout<<ext.get_k_way()<<" ";
    cout<<ext.num_chunks_in_a_unit(4)<<" ";
    cout<<ext.test_get_last_unit_chunks(8024, 4)<<endl;

    
    ext.pass_0_sort("yelp_final_dataset",&wt);
    cout<<ext.get_num_chunks("p0sorted.txt")<<endl;
    cout<<"pass 0 sort end"<<endl;
    ext.merge_sorting("p0sorted.txt");
    cout<<"end of pass"<<endl;
    
    return 0;
    

    
};

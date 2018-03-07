/*
 * database.h
 *
 *  Created on: Feb 23, 2018
 *      Author: georgexu
 */

#ifndef DATABASE_H_
#define DATABASE_H_
#include "bpt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index_reader.h"
#include "predefined.h"
using bpt::bplus_tree;

class database{

private:
    bplus_tree tree;

public:
    database():tree("test.db",true){
        
    };
    
    void test_load(index_reader* p_index_reader){
        for( std::vector<std::pair <string, int> >::iterator it = p_index_reader->string_int_pairs.begin(); it != p_index_reader->string_int_pairs.end(); ++it )
        {
            char key[16];
            const string string1(it->first);
            std::strcpy(key,string1.c_str());
            tree.insert(key, it->second);
        }
        //manually insert some keys
        char key1[16]={"feng"};
        tree.insert(key1, 2);
        char key2[16]={"yuting"};
        tree.insert(key2, 4);
    };//insert to bplus tree
    
    void load(string filename){//load directly from the disk
        
    }
    
    void search(char * key){//use the tree search
        bpt::value_t value;
        if (tree.search(key, &value) != 0)
            printf("Key %s not found\n", key);
        else
            printf("%d\n", value);
        //todo
        //****also from this number search all the docnames and cout them
    };
    void range_search(char *key1,char * key2){
        bpt::key_t start(key1);
        bpt::value_t values[512];
        bool next = true;
        while (next) {
            int ret = tree.search_range(
                                            &start, key2, values, 512, &next);
            if (ret < 0)
                break;
//            for (int i = 0; i < ret; i++)
//                printf("%d\n", values[i]);
        }//todo
        //cout all the keyword inbetween,later
    }
    void insert_key_doc(string key,string doc){
        
    }
    void delete_key_doc(string key, string doc){
        
    }
    int count(string keyword);
    
    int printpath(string keyword);
    
    void range (char* k1, char*  k2);//: Range query. Print all of the keywords between keyword1 and keyword2where keyword1 < keyword2


};






#endif /* DATABASE_H_ */

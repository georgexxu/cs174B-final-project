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
#include "writer.h"

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
    
    void insert_key_doc(string key,string doc, writer* p_writer){
        ifstream myfile;
        ofstream outputfile;
        string line;
        int page_pos = -1; //set as 1 first

        myfile.open("origin_index.txt");
        outputfile.open("new_origin_index.txt");
        
        if(!myfile){
            cout<<"Cannot open input file"<<endl;
        }
        
        //seek for the insert page
        while (myfile >> line)
        {
            cout << line << endl;
            myfile.ignore(1000, '\n');
        }
        
        
        myfile.close();
        outputfile.close();
        
    }
    void delete_key_doc(string key, string doc, writer* p_writer){
        ifstream myfile;
        ofstream outputfile;
        string line;
        bpt::value_t page_pos;
        char key_char[16];
        std::string word1;
        std::string word2;
        std::strcpy(key_char,key.c_str());
        long f_exact_pos = -1;
        long b_exact_pos = -1;

        //seek for the insert page from tree
            //int page_pos = 1; //set as 1 first
        if ( tree.search(key_char, &page_pos) != 0)
            printf("Key %s not found\n", key_char);
        else{
            printf("Key %s found in page", key_char);
            cout<<page_pos<<endl;////////
            myfile.open("origin_index.txt");
            outputfile.open("new_origin_index.txt",std::ofstream::out | std::ofstream::trunc);
            
            if(!myfile){
                cout<<"Cannot open input file"<<endl;
            }
            
            myfile.seekg( (page_pos-1) * p_writer->get_page_size() );
            cout<<"set to"<<page_pos * p_writer->get_page_size()<<endl;///////
            bool foundkey=0;
            while (myfile >> word2){
                cout<<"scan through "<<word2<<" ";
                if (foundkey and (word2=="DOC"+doc)){
//                    b_exact_pos = myfile.tellg();
                    break;;
                }
                else{
                    foundkey=0;
                }
                    
                if (key == word2){
                    b_exact_pos = myfile.tellg();
                    foundkey=1;
                    cout<<"found "<<word2<<endl;
                    continue;
                }
                word1 = word2;
                f_exact_pos = myfile.tellg();
                
            }
            cout<<"front word "<<word1<<" pos at"<<f_exact_pos<<endl;////
            cout<<"find the word "<<word2<<"at"<<b_exact_pos<<endl;////
            myfile >> word2;b_exact_pos = myfile.tellg();
            
            
            // start to write the new file
            cout<<"start update the origin_index"<<endl;/////
            long c_pos=-1;
            myfile.seekg(0);
            //copy the previous lines
            for(int i=0; i<page_pos-1; i++){
                getline(myfile,line);
                c_pos = myfile.tellg();
                outputfile<<line;
                outputfile<<"\n";
            }
            //copy the content in this line before the key
            while(c_pos < f_exact_pos){
                myfile>>line;
                c_pos = myfile.tellg();
                outputfile<<line<<" ";
            }
            
            myfile.ignore(50,' ');
            myfile.ignore(50,' ');
            myfile.ignore(50,' ');
            
            bool odd = 0;
            while(myfile>>line)
            {
                long position = outputfile.tellp();
                //              cout<<"position before write"<<position<<endl;
                if ((position % p_writer->get_page_size() > p_writer->get_page_size() - 100) and odd != 1){
                    outputfile<<"\n";
                    //                cout<<"new page"<<endl;
                    outputfile.seekp((position/p_writer->get_page_size()+1)*p_writer->get_page_size());
                }
                outputfile<< line<<" ";
                odd = !odd;
            }
            

            myfile.close();
            outputfile.close();
            cout<<"finish writing new origin index"<<endl;/////
        }
        tree.remove(key_char);
        cout<<"removed node from tree"<<endl;////
        remove( "origin_index.txt");
        rename("new_origin_index.txt", "origin_index.txt");
        
    }
    
    int count(string keyword);
    
    int printpath(string keyword);
    
    void range (char* k1, char*  k2);//: Range query. Print all of the keywords between keyword1 and keyword2where keyword1 < keyword2


};






#endif /* DATABASE_H_ */

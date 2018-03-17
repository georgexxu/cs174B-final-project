//
//  external_sorter.h
//  174b
//
//  Created by Xiaofeng Xu on 14/03/2018.
//  Copyright © 2018 Xiaofeng Xu. All rights reserved.
//

#ifndef external_sorter_h
#define external_sorter_h

#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<ctime>
#include <stdio.h>
#include<algorithm>
#include "writer.h"
//#include "origin_reader.h"
#include<math.h>
#include "database.h"
using namespace std;

typedef std::pair <string, string> stringPair;

bool sortbysec1(const pair<string,string> &a,
               const pair<string,string> &b)
{   if(a.first==b.first){
    return(a.second<b.second);
    }
    return (a.first < b.first);
}



class external_sorter{
    
    
private:
    int k_way;//input buffer number and page
    int o_buffer;//# output pages
    int page_size;
    
public:
    external_sorter(int buff_pages,int page_size){
        k_way = quadratic_solver(buff_pages);//output buffer 10*k_way
        o_buffer=10*k_way;//output buffer 10*k_way
        this->page_size =page_size;
    }

    long filesize(string filename){
        long size = 0;
        fstream is;
        is.open(filename);
        if(is){
            is.seekg (0, is.end);
            size = is.tellg();
        }
        return size;
    }
    
    void sortvec(std::vector<std::pair <string, string> > &string_pairs){
        sort(string_pairs.begin(),string_pairs.end(),sortbysec1);
    }
    
    int quadratic_solver(int buff_pages){//determine how many way sorts according to buff_pages
        //x^+10x-buff_pages = 0
        int x = sqrt(100+4*buff_pages);
        x = (x-10)/2;
        return x;
    }
    
    int get_num_chunks(string filename){//"p0_temp1.txt", or num_sorted_runs
            double chunk_size =k_way*page_size+0.0;//
            long total_bytes = filesize(filename);
            cout<<"total bytes"<<total_bytes<<"/"<<chunk_size<<": "<<ceil(total_bytes/chunk_size);
            return ceil(total_bytes/chunk_size);
    }
    
    void loading_for_pass0(string filename,writer * p_writer){
        std::vector<std::pair <string, string> > string_pairs;
        fstream myfile;
        myfile.open(filename);
        if(!myfile){
            cout<<"Cannot open input file"<<endl;
        }
        int line_counter = 0;
        while(myfile){
            line_counter++;
//            if(line_counter>4000000){
//                return;
//            }
            //cout<<"inside origin reader, reading my file"<<endl;
            string doc_name = "";
            string line = "";
            std::getline(myfile,line); // get the line
            //cout<<"gettingl line: "<<line<<endl;
            char delim = ' ';
            string item;
            auto i = 0;
            auto pos = line.find(delim);
            //cout<<"  pos: "<<pos<<endl;
            while(pos!=string::npos){
                if(i==0) {
                    doc_name = line.substr(i,pos-i);
                    //cout<<"doc name: "<<doc_name<<endl;
                }else{
                    item = line.substr(i,pos-i);
                    if(item.length()!=1 and item.length()!=0 and !(item.length()>16)and item.length()!=2){//if it is not a single character
                        stringPair s_pair(doc_name,item);
                        string_pairs.push_back(s_pair);
                    }
                }
                i = ++pos;
                pos = line.find(delim,pos);
                if(pos==string::npos){//if it is not a single character
                    item = line.substr(i,line.length());
                    if(item.length()!=1){
                        stringPair s_pair(doc_name,item);
                        string_pairs.push_back(s_pair);
                    }
                }
                
            }
        }
        myfile.close();
        p_writer->write_without_blocks(string_pairs,"p0_temp1.txt");
        //cout<<"finish loading vectors"<<endl;
    }
    void pass_0_sort(string filename,writer * p_writer){//output
        fstream myfile;
        //make original file into file with pairs, unsorted tho
        //finish loading vector
        //empty the vecotr after writing
        //pass0 on the unsorted file
        //k_way => k_way pages per chunk
        //int total_pairs = count_pairs(filename);
        loading_for_pass0(filename, p_writer);
        
        std::vector<std::pair <string, string> > string_pairs;
        int chunk_size =k_way*page_size;//
        long total_bytes = filesize("p0_temp1.txt");
      //  cout<<"total bytes"<<total_bytes<<endl;
       // cout<<"chunk size"<<chunk_size<<endl;
        ofstream os;
        os.open("p0sorted.txt");//output stream
        myfile.open("p0_temp1.txt");
        string keyword;
        string doc_name;
        long pos;
        long pos2=0;
        while(myfile){
            myfile>>keyword;
            myfile>>doc_name;
            //keyword = keyword.substr(0,15);
            stringPair s_pair(keyword,doc_name);
            string_pairs.push_back(s_pair);
            pos = myfile.tellg();
            if ((pos-pos2) % chunk_size > chunk_size - 100){
                pos2 = pos;
                //cout<<"pos: "<<pos<<endl;
                sortvec(string_pairs);
                for( std::vector<std::pair <string, string> >::iterator it =string_pairs.begin(); it !=string_pairs.end(); ++it ){
                    os<<it->first<<" "<<it->second <<" ";
                }
                long pos3 = os.tellp();
                os.seekp((pos3/chunk_size+1)*chunk_size-1);
                os<<"\n";
                pos3 = os.tellp();
        //        cout<<"  "<<pos3<<endl;
                string_pairs.clear();
            }
        }
        if(!string_pairs.empty()){//indicate size < chunk_size
            sortvec(string_pairs);
            for( std::vector<std::pair <string, string> >::iterator it =string_pairs.begin(); it !=string_pairs.end(); ++it ){
                os<<it->first<<" "<<it->second <<" ";
            }
            string_pairs.clear();
        }
     //   cout<<"filling a line smaller than a block";
        long pos3 = os.tellp();
        os.seekp((pos3/chunk_size+1)*chunk_size-1);
        os<<"\n";
        pos3 = os.tellp();
       // cout<<"  "<<pos3<<endl;;
        os.close();
    }
    
    
    void parse_line(std::vector<std::pair <string, string> > &string_str_pairs1,string line1){
        if(line1=="" or std::isspace(line1.at(0))) {
	  // cout<<"parse_line: returned in advance!"<<endl;
         return;
        }
        char delim = ' ';
        auto i1 = 0;
        //int b;
        //cout<<"give a int"<<endl;
       // cin>>b;
        auto pos1 = line1.find(delim);
        //cout<<pos1;
        //cout<<"give a int"<<endl;
        //cin>>b;
        string keyword("");
        string doc("");
        string page_num("");
        int count_even_odd1 = 1; //1 is odd
        //cout<<"  pos: "<<pos<<endl;
        //int a ;
        //cerr<<"inside parse line"<<endl;
        while(pos1!=string::npos){
            if(count_even_odd1 ==1){//keyword part
                //cerr<<"parseline, pos1-i1: "<<pos1-i1<<endl;
                keyword=line1.substr(i1,pos1-i1);
                //cout<<"keyword: "<<keyword<<endl;
                i1 = ++pos1;//keep reading the line
                pos1 = line1.find(delim,pos1);
                count_even_odd1 = 0;
            }else{
                //cerr<<"parseline, pos1-i1: "<<pos1-i1<<endl;
                //cin>>a;
                
                doc=line1.substr(i1,pos1-i1);
                //cout<<"doc: "<<doc<<endl;
                if(keyword!=""){
                    stringPair s_s_pair(keyword,doc);
                    string_str_pairs1.push_back(s_s_pair);
                }
                i1 = ++pos1;//keep reading the line
                pos1 = line1.find(delim,pos1);
                count_even_odd1 = 1;
            }
        }

    }
    
    void write_a_chunk(ofstream & os, std::vector<std::pair <string, string> > &output_buffer,int chunk_size){
        long pos;
        int count = 0;
        for( std::vector<std::pair <string, string> >::iterator it =output_buffer.begin(); it !=output_buffer.end(); ++it ){
            pos = os.tellp();
            if (pos % chunk_size > chunk_size - 100){
                    //cout<<"filling empty"<<endl;
                    os.seekp((pos/chunk_size+1)*chunk_size-1);
                        //cout<<"filling a line"<<endl;
                        os<<"\n";
                cout<<os.tellp();
                output_buffer.erase(output_buffer.begin(),output_buffer.begin()+count);
                return;// just write one chunk
            }
                //output_buffer.clear();//dump if there is anything left
            os<<it->first<<" "<<it->second <<" ";
            count++;
        }
        // first # of count element
    }
    void write_less_than_a_chunk(ofstream & os, std::vector<std::pair <string, string> > &output_buffer,int chunk_size){
        long pos;
        int count = 0;
        for( std::vector<std::pair <string, string> >::iterator it =output_buffer.begin(); it !=output_buffer.end(); ++it ){
            pos = os.tellp();
            if (pos % chunk_size > chunk_size - 100){
                //cout<<"filling empty"<<endl;
                os.seekp((pos/chunk_size+1)*chunk_size-1);
                //cout<<"filling a line"<<endl;
                os<<"\n";
               // cout<<os.tellp();
            }
            //output_buffer.clear();//dump if there is anything left
            os<<it->first<<" "<<it->second <<" ";
            count++;
        }
        cout<<"last small part"<<endl;
        pos = os.tellp();
        os.seekp((pos/chunk_size+1)*chunk_size-1);
        os<<"\n";
        cout<<pos;
        output_buffer.erase(output_buffer.begin(),output_buffer.begin()+count);
    }
    

    void pass_1_merge(string filename,fstream & is,std::vector<std::pair <string, string> > input_buffer[],std::vector<std::pair <string, string> > output_buffer,ofstream & os){
        long num_chunks = get_num_chunks(filename);
        cout<<"num of chunks"<<num_chunks<<endl;
        long temp = num_chunks;
        long current_chunk = 0;
        cout<<"inside pass 1, enter int to continue "<<endl;
        while(current_chunk < temp){
            cout<<"k_way: "<<k_way<<endl;
            for(int k = 0; k < k_way; k++){
                string line;
                if(!is.eof()){
                    std::getline(is,line);
                    //cout<<"getting the line"<<line.substr(0,20)<<endl;
                    parse_line(input_buffer[k], line);
                    if(!input_buffer[k].empty()){
                        //cout<<" "<<k<<" :I am not empty"<<endl;
                    }
                    ++current_chunk;
                }else{
                    ++current_chunk;
                    break;
                }
                if(k==k_way-1 or is.eof())//potential risk
                    ++current_chunk;
            }//finished loading the input buffers
            //keep moving pairs from input buffers to output
            //if output full, then write it to disk and clear it
            //continue until all input buffer empty
	    // cout<<"after empty here "<<endl;
            int num_pairs_holding = 12 * k_way * page_size/20;
            int chunk_size =k_way*page_size;
            //int a;
	    // cout<<"chunk size: "<<chunk_size<<endl;
            while(not_all_empty(input_buffer, k_way)){//becasue this is first pass
                //cout<<"never here?"<<endl;
                //cout<<"vector_processer:"<<endl;
                std::vector<std::pair <string, string> > vec_processer=compare_first_element(input_buffer, k_way);
                //cerr<<"first key: "<<vec_processer.begin()->first<<endl;
                if(output_buffer.size()>=num_pairs_holding){//we can't tell when output buffer is full here, because
                    //we dont know the size of the pairs
                    //so we assume that we can only handle a certain amount of
                    //pairs, it's ok to have more stored but during writing out we
                    //need to keep track of bytes written
                   // cout<<"when output_buffer.size()>=num_pairs_holding) "<<endl;
                    write_a_chunk(os, output_buffer, chunk_size);
                }else{

                    output_buffer.push_back(*vec_processer.begin());
                    vec_processer.clear();
                }
            }
            cout<<"last part"<<endl;
            int i =0 ;
            std::vector<std::pair <string, string> >::iterator it =output_buffer.end();
//            while(i<50){
//                cout<<it->first<<"="<<it->second<<"!"<<endl;
//                it--;
//                i++;
//            }
            write_less_than_a_chunk(os, output_buffer, chunk_size);
            output_buffer.clear();
        }
    }
    
    
    
    void merge_sorting(string filename){//merge sort "p0sorted.txt"
        fstream is;
        ofstream os;
        is.open(filename);
        string f_name1 = "temp1.txt";
        //string f_name2 = "temp_name2.txt";
        os.open(f_name1);//pass1写出名
        int num_chunks = get_num_chunks(filename);
        std::vector<std::pair <string, string> > input_buffer[k_way];//k_way chunks
        std::vector<std::pair <string, string> > output_buffer;//10 chunks
        double temp = num_chunks;
        if(temp == 1) {
            cout<<"sorted in pass 0"<<endl;
            is.close();
            os.close();
            return;
        }
        if(temp>1){
            cout<<"start pass 1 merge"<<endl;
            pass_1_merge(filename, is, input_buffer, output_buffer,os);
            if(temp<k_way) {
                is.close();
                os.close();
                return;//if return rename
            }
        }
        //pass2,3,4,5,6
        int pass_num = 2;
        cout<<"start pass"<<pass_num<<endl;
        while(!pass_n_merge(f_name1, input_buffer, output_buffer,pass_num)){//读入前一步写出文件
            
            pass_num++;
            if(pass_num%2 == 1) {
                f_name1 = "temp2.txt";
                remove("temp1.txt");
            }//odd pass temp2 name, when pass
            else {
                f_name1 = "temp1.txt";
                remove("temp2.txt");
            }
            cout<<"start pass"<<pass_num<<endl;
        }
        is.close();
        os.close();
        cout<<"end of pass, total number of passes:"<<pass_num<<endl;
    }
    
    
    //functions needed by pass_n
    int num_chunks_in_a_unit(int pass){//previous pass filename
        int count = 1;
        for(int i=0;i<pass-1;i++){
            count = count*k_way; //1 chunk
        }
        return count;
    }
    int get_last_unit_chunks(string filename,int pass){
        int total =get_num_chunks(filename);
        int num_of_units = total/num_chunks_in_a_unit(pass);
        int last = total - num_of_units*num_chunks_in_a_unit(pass);
        return last;
    }
    int test_get_last_unit_chunks(int size,int pass){
        int num_of_units = size/num_chunks_in_a_unit(pass);
        int last = size - num_of_units*num_chunks_in_a_unit(pass);
        return last;
    }
    int num_units(string filename,int pass){
        int total =get_num_chunks(filename);
        int num_of_units = ceil((total+0.0)/num_chunks_in_a_unit(pass));
        return num_of_units;
    }
    int test_num_units(int size,int pass){
        int num_of_units = size/num_chunks_in_a_unit(pass);
        return num_of_units;
    }
    void initialize_int_array(int arry[],int k){
        for(int i = 0;i<k;i++){
            arry[i]=0;
        }
    }
    int one_empty(std::vector<std::pair <string, string> > input_buffer[], int k){
        for(int i= 0;i < k; i++){
            if(input_buffer[i].empty()){//if one of them not empty
                return i;
            }
        }
        return false;
    }
    
    bool pass_n_merge(string filename,std::vector<std::pair <string, string> > input_buffer[],std::vector<std::pair <string, string> > output_buffer,int pass_num){
        //determine big chunk size
        fstream is;
        ofstream os;
        string output_f_name="txt";//does not matter
        if(pass_num %2 == 1) output_f_name = "temp1.txt";//odd pass temp2 name, when pass
        else output_f_name = "temp2.txt";
        is.open(filename);
        os.open(output_f_name);
//        os<<pass_num;
//        int n_units = test_num_units(5000000, pass_num);//test
        int n_units = num_units(filename, pass_num);
        if(pass_num>=2){
            int num_chunks_per_unit = num_chunks_in_a_unit(pass_num);
            int last = get_last_unit_chunks(filename, pass_num);
            int offset =k_way*page_size*num_chunks_per_unit;
            int bounds[n_units];
            int counters[k_way];
            int num_pairs_holding = 12 * k_way * page_size/20;
            int chunk_size =k_way*page_size;//
            for(int i = 0;i<n_units;i++){
                
                if(i==n_units-1 &&last != 0)//problem
                    bounds[i]=last;
                else
                    bounds[i] = num_chunks_per_unit-1;// off by one
            }
            int loop = ceil(static_cast<double>(n_units)/(k_way+0.0));//check
	    // cout<<"check n_units :"<<n_units<<endl;
	    // cout<<"loop number check: "<<loop<<endl; // correct
	    // cout<<"num_units check: "<<n_units<<endl;
	    // cout<<"last chunk size check: "<<last<<endl;
	    // cout<<"check bound array: "<<endl;
	    //  cout<<"check chunk size: "<<chunk_size<<endl;
            for(int i = 0;i<n_units;i++){
                cout<<bounds[i]<<" ";// off by one
            }
            //cout<<endl;
            for(int k = 0; k<loop; k++){
                initialize_int_array(counters, k_way);
                int counter_limit;
                if(k_way>=n_units) counter_limit = n_units;
                else if(k==loop-1)
                    counter_limit = last;//last part
                else
                    counter_limit=k_way;

                //vectors ready in the arguments
                //1st time load
                //cout<<"check first time laoding "<<endl;
                for(int i = 0;i<counter_limit;i++){//problem bounds might have smaller size than counter!!
                    if(bounds[i+k*k_way]>counters[i]){//k=0,
                        long pos = 0+i*offset+k*k_way*offset;
                        is.seekg(pos);
                        string line;
                        getline(is,line);
			// if(!line.empty())
			// cout<<line.substr(0,15)<<endl;//correct
                        parse_line(input_buffer[i],line);
                        counters[i]=counters[i]+1;
                    }
                }//end 1st load
                //cout<<"check after first loading "<<endl;
                for(int i = 0;i<counter_limit;i++){
		  //cout<<counters[i]<<" ";// off by one
                }
                //cout<<endl;
                //cout<<"start looping outputting! "<<endl;
                string line;
                while(not_all_empty(input_buffer, k_way)){//not_all_empty correct!!
                    //cout<<"inside checking not all empty"<<endl;
                    int empty;// give the empty input_buffer
                    while(!compare_first_element2(input_buffer,output_buffer,counter_limit,empty)){//pushing the smallest
                        //to output buffer and check whether corres input_buff is empty
                        //which one is empty?
                        //cerr<<"first key: "<<vec_processer.begin()->first<<endl;
                        if(output_buffer.size()>=num_pairs_holding){//see explanation before
                        write_a_chunk(os, output_buffer, chunk_size);
                            //cout<<"counter "<<endl;
                        }
                    }
                    //空了
                    //compare with bound(attention, k), find correct place, fill in,
                    if(counters[empty]>bounds[empty+k*k_way]){//off by one, consistent
//                        cout<<"buffer no."<<empty;
//                        cout<<" hit empty but nothing left: ";
//                        cout<<counters[empty]<<"  "<<"bound: "<<bounds[empty+k*k_way]<<endl;
                        continue;
                    }else{
                        //find the place
//                        cout<<"buffer no."<<empty;
//                        cout<<" hit empty:  "<<counters[empty]<<"  ";
//                        cout<<"bound: "<<bounds[empty+k*k_way]<<endl;
                        long pos = 0+empty*offset+counters[empty]*chunk_size +k*k_way*offset;
                        is.seekg(pos);
                        //get the line
                        getline(is,line);
//                        if(!line.empty())
//                            cout<<" filling line: "<<line.substr(0,15)<<endl;
//                        //fill in the vector
//                        parse_line(input_buffer[empty],line);
//                        if(input_buffer[empty].empty())
//                            cout<<" empty now, nothing to fill"<<endl;
//                        else
//                            cout<<"get filled, not empty"<<endl;
//                        if(input_buffer[empty-1].empty())
//                            cout<<"input buffer "<<empty-1<<" is empty, but should not be empty"<<endl;
                        counters[empty]=counters[empty]+1;
//                        cout<<"====="<<endl;
                    }
                }
//                cout<<"check all empty???"<<endl;
//                for(int i = 0;i<counter_limit;i++){
//                    if(input_buffer[i].empty())
//                        cout<<"inputbuffer "<<i<<" empty now"<<endl;
//                    else
//                        cout<<"inputbuffer "<<i<<" not empty"<<endl;
//                    // off by one
//                }
//                
                write_less_than_a_chunk(os, output_buffer, chunk_size);
            }//
            
        }
        is.close();
        os.close();
        return(k_way >=n_units);
    }
    bool not_all_empty(std::vector<std::pair <string, string> > input_buffer[], int k){//correct
        for(int i= 0;i < k; i++){
            if(!input_buffer[i].empty()){//if one of them not empty
                //cout<<" "<<i<<"th is not empty"<<endl;
                return true;
            }
        }
        //cout<<"all empty"<<endl;
        return false;
    }
    
    std::vector<std::pair <string, string> > compare_first_element(std::vector<std::pair <string, string> > input_buffer[], int k){// this funciton get
       // the first element of k vectors and sort them and return a vector
        std::vector<std::pair <string, string> > vec_processer;
        //pair<string,string>
        int small=0;
        for(int i= 0;i < k; i++){
            if(!input_buffer[i].empty()){
                vec_processer.push_back(*input_buffer[i].begin());//set the first
                small = i;
                break;
            }
        }
        for(int i= 0;i < k; i++){
            if(!input_buffer[i].empty()){
                if(sortbysec1(*input_buffer[i].begin(),*vec_processer.begin())){
                    vec_processer.pop_back();
                    vec_processer.push_back(*input_buffer[i].begin());
                    small =i;
                }
                //input_buffer[i].erase(input_buffer[i].begin());
            }
        }
        input_buffer[small].erase(input_buffer[small].begin());
        return vec_processer;
    }
    
    bool compare_first_element2(std::vector<std::pair <string, string> > input_buffer[], std::vector<std::pair <string, string> > & out_buffer,int k,int & empty){// this funciton get
        // the first element of k vectors and sort them and return a vector
        std::vector<std::pair <string, string> > vec_processer;
        //pair<string,string>
        int small=0;//potential all empty error
        for(int i= 0;i < k; i++){
            if(!input_buffer[i].empty()){
                vec_processer.push_back(*input_buffer[i].begin());//set the first
                small = i;
                empty = i;//remember!
                break;
            }
        }
        for(int i= 0;i < k; i++){
            if(!input_buffer[i].empty()){
                if(sortbysec1(*input_buffer[i].begin(),*vec_processer.begin())){
                    //out_buffer.push_back(*input_buffer[i].begin());
                    vec_processer.pop_back();
                    vec_processer.push_back(*input_buffer[i].begin());
                    small = i;
                    empty = i;
                }
                //input_buffer[i].erase(input_buffer[i].begin());
            }
        }
        out_buffer.push_back(*input_buffer[small].begin());
        input_buffer[small].erase(input_buffer[small].begin());//delete here
        return input_buffer[small].empty();
    }
    
    
    int count_pairs(string filename){//later for deciding # sorted runs
        int total_pair = 0;
        fstream myfile;
        myfile.open(filename);
        if(!myfile){
            cout<<"Cannot open input file"<<endl;
        }
        int line_counter = 0;
        while(myfile){
            line_counter++;
            if(line_counter>10000){//change later
                return total_pair;
            }
            //cout<<"inside origin reader, reading my file"<<endl;
            string doc_name = "";
            string line = "";
            std::getline(myfile,line); // get the line
            //cout<<"gettingl line: "<<line<<endl;
            char delim = ' ';
            string item;
            auto i = 0;
            auto pos = line.find(delim);
            //cout<<"  pos: "<<pos<<endl;
            while(pos!=string::npos){
                if(i==0) {
                    doc_name = line.substr(i,pos-i);
                    //cout<<"doc name: "<<doc_name<<endl;
                }else{
                    item = line.substr(i,pos-i);
                    if(item.length()!=1 and item.length()!=0 and !(item.length()>16)and item.length()!=2){//if it is not a single character
                        cout<<" == "<<item;
                        ++total_pair;
                    }
                }
                i = ++pos;
                pos = line.find(delim,pos);
                if(pos==string::npos){//if it is not a single character
                    item = line.substr(i,line.length());
                    if(item.length()!=1){
                        cout<<" == "<<item;
                        ++total_pair;
                    }
                }
                
            }
        }
        myfile.close();
        return total_pair;
    }
    int get_k_way(){
        return k_way;
    }
    

    //void sort(vector)
};

#endif /* external_sorter_h */

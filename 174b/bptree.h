/*
 * bptree.h
 *
 *  Created on: Feb 23, 2018
 *      Author: georgexu
 */

#ifndef BPTREE_H_
#define BPTREE_H_

#include <stdio.h>

class bptree{

private:
    //node

public:
	bptree();
    void load();
    void insert_doc();
    void delete_doc();
    int count();
    int search(); //need varification
    int printpath();
    void range (char* k1, char*  k2);//: Range query. Print all of the keywords between keyword1 and keyword2where keyword1 < keyword2

};





#endif /* BPTREE_H_ */

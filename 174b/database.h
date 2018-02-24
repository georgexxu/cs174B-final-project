/*
 * database.h
 *
 *  Created on: Feb 23, 2018
 *      Author: georgexu
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include "bptree.h"

class database{

private:
    bptree bpt;


public:
    database(bptree bptr);
    void load();
    void insert_doc();
    void delete_doc();
    int count();
    int search(); //need varification
    int printpath();
    void range (char* k1, char*  k2);//: Range query. Print all of the keywords between keyword1 and keyword2where keyword1 < keyword2


};






#endif /* DATABASE_H_ */

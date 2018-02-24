/*

 * console.h
 *
 *  Created on: Feb 22, 2018
 *      Author: georgexu
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

//#include "database.h"
//#include "time.h"
#include<iostream>
using namespace std;
class console{

private:
    int num_of_pages;
//    database dbs;

public:
    console(int page);
    int getpage();
    void load ();
    void merge();//: Merge the current index file with the second index file, and update the index file on the disk.
    void page(int i);//: Displays the contents of the ith page


};




#endif /* CONSOLE_H_ */

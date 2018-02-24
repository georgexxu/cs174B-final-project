#include<iostream>
#include"console.h"
using namespace std;

console::console(int page){
	num_of_pages = page;
}
int console::getpage(){
	return num_of_pages;
}
void console::load() {
	cout<<"load in the console class"<<endl;
}

void console::merge() {
}

void console::page(int i) {
}
/*

 * console.cpp
 *
 *  Created on: Feb 22, 2018
 *      Author: georgexu
 */







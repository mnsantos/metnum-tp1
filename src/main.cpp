#include <iostream>
#include "Matriz.h"
#include <utility>
#include <algorithm>

using namespace std;

int main(int argc, char **argv){

	Matriz * m1 = new Matriz(3,4);
	m1 -> put(0,0,1);
	m1 -> put(0,1,1);
	m1 -> put(0,2,-1);
	m1 -> put(0,3,9);

	m1 -> put(1,0,0);
	m1 -> put(1,1,1);
	m1 -> put(1,2,3);
	m1 -> put(1,3,3);

	m1 -> put(2,0,-1);
	m1 -> put(2,1,0);
	m1 -> put(2,2,-2);
	m1 -> put(2,3,2);

	Matriz m2 = m1 -> gauss();

	m1 -> print();
	m2.print();
	delete m1;

	return 0;
}
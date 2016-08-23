#include <iostream>
#include "Matriz.h"
#include <utility>
#include <algorithm>

using namespace std;

int main(int argc, char **argv){

	Matriz * m1 = new Matriz(3,3);
	m1 -> put(0,0,4);
	m1 -> put(0,1,2);
	m1 -> put(0,2,-1);

	m1 -> put(1,0,2);
	m1 -> put(1,1,-1);
	m1 -> put(1,2,1);

	m1 -> put(2,0,8);
	m1 -> put(2,1,1);
	m1 -> put(2,2,6);

	Matriz m2 = m1 -> gauss();
	vector<Matriz> lu = m1 -> lu();

	cout << "Original \n";
	m1 -> print();
	cout << "Gauss \n";
	m2.print();
	cout << "L: \n";
	lu[0].print();
	cout << "U:  \n";
	lu[1].print();
	cout << "Producto:  \n";
	lu[0].producto(&lu[1]).print();	

	//m1 -> print();
	//m2.print();
	delete m1;

	return 0;
}
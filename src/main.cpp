#include <iostream>
#include "Matriz.h"
#include <utility>
#include <algorithm>

using namespace std;

int main(int argc, char **argv){

	Matriz * m1 = new Matriz(3,3);
	m1 -> put(0,0,1);
	m1 -> put(0,1,2);
	m1 -> put(0,2,3);

	m1 -> put(1,0,2);
	m1 -> put(1,1,3);
	m1 -> put(1,2,2);

	m1 -> put(2,0,1);
	m1 -> put(2,1,0);
	m1 -> put(2,2,4);

	Matriz m2 = m1 -> gauss();
	vector<Matriz> lu = m1 -> lu();

	cout << "Original" <<endl;
	m1 -> print();
	cout << "Gauss"<< endl;
	m2.print();
	cout << "L: " << endl;
	lu[0].print();
	cout << "U: " << endl;
	lu[1].print();
	cout << "Producto: " << endl;
	lu[0].producto(&lu[1]).print();	

	Matriz * b = new Matriz(3,1);
	b -> put(0,0,1);
	b -> put(1,0,2);
	b -> put(2,0,3);

	Matriz y = lu[0].solucion(b);
	cout << "Y: " << endl;
	y.print();

	Matriz sol = lu[1].solucion(&y);
	cout << "Sol: " << endl;
	sol.print();

	//cout<< y;
	//m1 -> print();
	//m2.print();
	delete m1;
	delete b;

	return 0;
}
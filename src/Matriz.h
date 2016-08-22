#ifndef __MATRIZ_H__
#define __MATRIZ_H__

#include <vector>
#include <utility>
#include <iostream>

using namespace std;

class Matriz {

	public:
			Matriz(int filas, int columnas);
			Matriz(const Matriz& m);
			Matriz(int tamanio);
			//Matriz(istream& is);
			Matriz producto(Matriz * m);
			double elem(int fila, int columna);
			int filas();
			int columnas();
			void put(int fila, int columna, double elem);
			vector<double> fila(int fila);
			void print();
			Matriz gauss();
			vector<Matriz> lu();

			/*Matriz sumar(Matriz& otraMatriz);
			Matriz restar(Matriz& otraMatriz);
			Matriz multiplicar(Matriz& otraMatriz);*/

	private:
			int cantFilas;
			int cantColumnas;
			vector< vector<double> > matriz;
};

#endif
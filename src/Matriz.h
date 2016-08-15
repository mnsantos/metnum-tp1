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
			//Matriz(istream& is);
			double elem(int fila, int columna);
			int filas();
			int columnas();
			void put(int fila, int columna, double elem);
			int findDistinctZero(int fila, int columna);
			vector<double> fila(int fila);
			void print();
			Matriz gauss();

			/*Matriz sumar(Matriz& otraMatriz);
			Matriz restar(Matriz& otraMatriz);
			Matriz multiplicar(Matriz& otraMatriz);*/

	private:
			int cantFilas;
			int cantColumnas;
			vector< vector<double> > matriz;
};

#endif
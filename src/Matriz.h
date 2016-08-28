#ifndef __MATRIZ_H__
#define __MATRIZ_H__

#include <vector>
#include <utility>
#include <iostream>

using namespace std;

class Matriz {

  public:
    Matriz();
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
    vector<Matriz> lu();
    bool esTriangularSuperior();
    bool esTriangularInferior();

  private:
    int cantFilas;
    int cantColumnas;
    vector< vector<double> > matriz;
};

#endif

#ifndef __MATRIZ_H__
#define __MATRIZ_H__
#include "Parametros.h"
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

class Matriz {

  public:
    Matriz();
    Matriz(int filas, int columnas);
    Matriz(const Matriz& m);
    Matriz(vector< vector<double> > matriz);
    Matriz(vector<double> matriz);
    Matriz(int tamanio);
    Matriz(Parametros param);
    //Matriz(istream& is);
    Matriz producto(Matriz * m);
    double elem(int fila, int columna) const;
    int filas() const;
    int columnas() const;
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

  ostream& operator<<(ostream& os, const Matriz& m);

#endif

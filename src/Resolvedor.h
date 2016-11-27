#ifndef __RESOLVEDOR_H__
#define __RESOLVEDOR_H__

#include <utility>
#include <iostream>
#include "Matriz.h"

using namespace std;

class Resolvedor {

  public:
  	Resolvedor();
    Resolvedor(Matriz& m, string metodo);
    Matriz resolverUsandoGauss(Matriz * b);
    Matriz resolverUsandoLU(Matriz * b);
    Matriz resolverTriangular(Matriz * m, Matriz * b);
  private:
    Matriz L;
    Matriz U;
    Matriz matriz;
};

#endif

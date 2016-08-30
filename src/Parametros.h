#ifndef PARAMETROS_H
#define PARAMETROS_H

#include <vector>
#include "Matriz.h"

struct Parametros {
  //variables
  double radioInterno;
  double radioExterno;
  int mMasUno; // cantRadios
  int n; // cantAngulos
  double valorIsoterma;
  int nInst;
  vector<Matriz> bs;
};

#endif

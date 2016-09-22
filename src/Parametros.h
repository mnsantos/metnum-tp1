#ifndef PARAMETROS_H
#define PARAMETROS_H

#include "Matriz.h"
#include <vector>

using namespace std;

class Matriz;

struct Parametros {
  //variables
  double radioInterno;
  double radioExterno;
  int mMasUno; // cantRadios
  int n; // cantAngulos
  double valorIsoterma;
  int nInst;
  vector<Matriz> bs;
  double deltaAngulo;
  double deltaRadio;
};

#endif

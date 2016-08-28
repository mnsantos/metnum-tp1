#include <iostream>
#include "Matriz.h"
#include "Resolvedor.h"
#include <utility>
#include <algorithm>
#include  <fstream>

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

  Matriz * b = new Matriz(3,1);
  b -> put(0,0,1);
  b -> put(1,0,2);
  b -> put(2,0,3);

  Resolvedor * resolvedor = new Resolvedor(*m1);
  cout << resolvedor -> resolverUsandoLU(b) << endl;
  cout << resolvedor -> resolverUsandoGauss(b) << endl;

  delete m1;
  delete b;
  delete resolvedor;

  // double radioInterno;
  // double radioExterno;
  // int mMasUno;
  // int n;
  // double valorIsoterma;

  // ifstream myReadFile;
  // myReadFile.open("text.txt");
  // if (myReadFile.is_open()) {
  //   while (!myReadFile.eof()) {
  //     myReadFile >> radioInterno >> radioExterno >> mMasUno >> n >> valorIsoterma;

  //   }
  // }
  // cout << radioExterno << endl;

  return 0;
}
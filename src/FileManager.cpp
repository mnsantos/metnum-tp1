#include "FileManager.h"
#include <cmath>
#include <stdexcept>
#include <vector>

using namespace std;

FileManager::FileManager(string pathFileIn, string pathFileOut) {
	this -> pathFileIn = pathFileIn;
	this -> pathFileOut = pathFileOut;
}

Parametros FileManager::read() {
  vector<Matriz> bs;
  ifstream myReadFile;

  //variables
  double radioInterno;
  double radioExterno;
  int mMasUno; // cantRadios
  int n; // cantAngulos
  double valorIsoterma;
  int nInst;

  Parametros params;

  myReadFile.open(pathFileIn.c_str());
  if (myReadFile.is_open()) {
    myReadFile >> params.radioInterno >> params.radioExterno >> params.mMasUno >> params.n >> params.valorIsoterma >> params.nInst;
    double cantidadIncognitas = n*mMasUno;
    double elem;
    for (int i = 0; i < nInst; ++i) {
      Matriz b = Matriz(n, 1);
      for (int j = 0; j < n; ++j) {
        myReadFile >> elem;
        b.put(j, 1, elem);
      }
      for (int j = n; j < cantidadIncognitas-n; ++j) {
        b.put(j, 1, 0);
      }
      for (int j = cantidadIncognitas-n; j < cantidadIncognitas; ++j) {
        myReadFile >> elem;
        b.put(j, 1, elem);
      }
      bs.push_back(b);
    }
  }
  params.bs = bs;
  return params;
}

void FileManager::write(Matriz m) {
  ofstream outFile;
  outFile.open(pathFileOut.c_str());
  outFile << m;
}
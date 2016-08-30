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

  Parametros params;

  myReadFile.open(pathFileIn.c_str());
  if (myReadFile.is_open()) {
    myReadFile >> params.radioInterno >> params.radioExterno >> params.mMasUno >> params.n >> params.valorIsoterma >> params.nInst;
    double cantidadIncognitas = params.n*params.mMasUno;
    double elem;
    for (int i = 0; i < params.nInst; ++i) {
      Matriz b = Matriz(params.n, 1);
      for (int j = 0; j < params.n; ++j) {
        myReadFile >> elem;
        b.put(j, 1, elem);
      }
      for (int j = params.n; j < cantidadIncognitas-params.n; ++j) {
        b.put(j, 1, 0);
      }
      for (int j = cantidadIncognitas-params.n; j < cantidadIncognitas; ++j) {
        myReadFile >> elem;
        b.put(j, 1, elem);
      }
      bs.push_back(b);
    }
  }
  params.bs = bs;
  params.deltaAngulo = 360/params.n;
  params.deltaRadio = (params.radioExterno - params.radioInterno) / params.mMasUno;

  return params;
}

void FileManager::write(Matriz m) {
  ofstream outFile;
  outFile.open(pathFileOut.c_str());
  outFile << m;
}
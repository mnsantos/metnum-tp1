#include "FileManager.h"
#include <cmath>
#include <stdexcept>
#include <vector>
#include <iomanip>
#include <iostream> 
#include <sstream>

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
      Matriz b = Matriz(cantidadIncognitas, 1);
      for (int j = 0; j < params.n; ++j) {
        myReadFile >> elem;
        b.put(j, 0, elem);
      }
      for (int j = params.n; j < cantidadIncognitas-(params.n); ++j) {
        b.put(j, 0, 0);
      }
      for (int j = cantidadIncognitas-(params.n); j < cantidadIncognitas; ++j) {
        myReadFile >> elem;
        b.put(j, 0, elem);
      }
      bs.push_back(b);
    }
  }
  params.bs = bs;
  params.deltaAngulo = 360/params.n;
  params.deltaRadio = (params.radioExterno - params.radioInterno) / (params.mMasUno-1);

  return params;
}

void FileManager::write(vector<Matriz> xs, vector<Matriz> isotermas) {
  ofstream outFile;
  outFile.open(pathFileOut.c_str());
  for (int i = 0; i < xs.size(); ++i) {
    outFile << setprecision(9) << xs[i];
  }
  ofstream outFile2;
  outFile2.open((pathFileOut + "_isoterma").c_str());
  for (int i = 0; i < isotermas.size(); ++i) {
    outFile2 << setprecision(9) << isotermas[i];
  }
}

void FileManager::write(vector<Matriz> xs, vector<Matriz> isotermas, string metodo, vector<double> peligrosidades, vector<int> subdiscretizaciones, vector<double> tiemposPorInstancia, double tiempoTotal, Parametros params) {
  this -> write(xs, isotermas);
  ofstream outExperimentos;
  ostringstream paramsUsados;
  paramsUsados << params.radioInterno << "_" << params.radioExterno << "_" << params.mMasUno << "_" << params.n << "_" << params.valorIsoterma << "_" << params.nInst;
  string expOut = pathFileOut.substr(pathFileOut.find('/')+1, pathFileOut.size());
  outExperimentos.open(("experimentos/"+expOut).c_str());
  outExperimentos << paramsUsados.str() << endl;
  if (metodo=="0") {
    outExperimentos << "EG" << endl;
  } else {
    outExperimentos << "LU" << endl;
  }
  for (int i=0; i < peligrosidades.size(); i++){
    outExperimentos << peligrosidades[i] << " ";
  }
  outExperimentos << endl;
  for (int i=0; i < subdiscretizaciones.size(); i++){
    outExperimentos << subdiscretizaciones[i] << " ";
  }
  outExperimentos << endl;
  for (int i=0; i < tiemposPorInstancia.size(); i++){
    outExperimentos << tiemposPorInstancia[i] << " ";
  }
  outExperimentos << endl;
  outExperimentos << tiempoTotal << endl;
}
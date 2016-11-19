#ifndef __FILEMANAGER_H__
#define __FILEMANAGER_H__

#include <utility>
#include <iostream>
#include "Parametros.h"
#include "Matriz.h"
#include <fstream>

using namespace std;

class FileManager {

  public:
    FileManager(string pathFileIn, string pathFileOut);
    Parametros read();
    void write(vector<Matriz> xs, vector<Matriz> isotermas);
    void write(vector<Matriz> xs, vector<Matriz> isotermas, string metodo, vector<double> peligrosidades, vector<int> subdiscretizaciones, vector<double> tiemposPorInstancia, double tiempoTotal, Parametros params);

  private:
    string pathFileIn;
    string pathFileOut;
};

#endif

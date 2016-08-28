#include "Matriz.h"
#include <cmath>
#include <stdexcept>

using namespace std;

Matriz::Matriz(){
}

Matriz::Matriz(const Matriz& m) {
  cantFilas = m.cantFilas;
  cantColumnas = m.cantColumnas;
  for (int i=0; i<cantFilas; i++){
    matriz.push_back(m.matriz[i]);
  }
}

Matriz::Matriz(int filas, int columnas){
  cantFilas = filas;
  cantColumnas = columnas;
  vector<double> fila;
  for(int i = 0; i < columnas; i++){
    fila.push_back(0.0);
  }

  for(int i = 0; i < filas; i++){
    matriz.push_back(fila);
  }
}

void Matriz::put(int fila, int columna, double elem){
  matriz[fila][columna] = elem;
}

int Matriz::filas() const {
  return cantFilas;
}

int Matriz::columnas() const {
  return cantColumnas;
}

double Matriz::elem(int fila, int columna) const{
  return matriz[fila][columna];
}

vector<double> Matriz::fila(int fila) {
  return matriz[fila];
}

Matriz::Matriz(int tamanio){
  cantFilas = tamanio;
  cantColumnas = tamanio;
  vector<double> fila;
  for(int i = 0; i < cantColumnas; i++){
    fila.push_back(0.0);
  }

  for(int i = 0; i < cantFilas; i++){
    matriz.push_back(fila);
  }
}

Matriz::Matriz(vector< vector<double> > m){
  matriz = m;
  cantFilas = m.size(); 
  cantColumnas = m.size();
}

Matriz::Matriz(vector<double> b){

  cantFilas = b.size();
  cantColumnas = 1;
  for(int i = 0; i < cantFilas; i++){
    vector<double> fila;
    for(int j = 0; j < cantColumnas; j++){
      fila.push_back(b[i]);
    }
    matriz.push_back(fila);
  }

}

ostream& operator<<(ostream& os, const Matriz& m) {
  for (int i=0; i<m.filas(); i++){
    for (int j=0; j<m.columnas(); j++){
      os << m.elem(i,j) << " ";
    }
    os << "\n";
  }
  return os;
}

vector<Matriz> Matriz::lu(){
  Matriz u = Matriz(* this);
  Matriz l = Matriz(cantFilas, cantColumnas);

  for (int k=0; k<cantColumnas; k++){
    l.put(k,k,1);
    for (int i=k+1; i<cantFilas; i++){
      double z = u.elem(i,k) / u.elem(k,k);
      u.put(i,k,0); 
      l.put(i,k,z);
      for (int j=k+1; j<cantColumnas; j++) {
        u.put(i, j , u.elem(i,j) - z * u.elem(k,j));
      }
    }
  }
  vector<Matriz> factorizacionLu;
  factorizacionLu.push_back(l);
  factorizacionLu.push_back(u);
  return factorizacionLu;
}

Matriz Matriz::producto(Matriz * m){
  Matriz res = Matriz(cantFilas, m -> columnas());
  for (int i=0; i<cantFilas; i++){
    for (int j=0; j<cantColumnas; j++){
      double v = 0;
      for (int k=0; k<cantColumnas; k++){
        v += matriz[i][k] * m -> elem(k,j);
      }
      res.put(i,j,v);
    }
  }
  return res;
}

bool Matriz::esTriangularSuperior(){
  for (int i=0; i<cantFilas; i++){
    for (int j=0; j<i-1; j++){
      if (matriz[i][j] != 0){
        return false;
      }
    }
  }
  return true;
}

bool Matriz::esTriangularInferior(){
  for (int i=0; i<cantFilas; i++){
    for (int j=i+1; j<cantColumnas; j++){
      if (matriz[i][j] != 0){
        return false;
      }
    }
  }
  return true;
}

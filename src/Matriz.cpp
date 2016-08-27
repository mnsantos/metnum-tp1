#include "Matriz.h"
#include <cmath>
#include <stdexcept>

using namespace std;

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

int Matriz::filas() {
  return cantFilas;
}

int Matriz::columnas() {
  return cantColumnas;
}

double Matriz::elem(int fila, int columna){
  return matriz[fila][columna];
}

void Matriz::print() {
  cout << "Filas: " << cantFilas << " .Columnas: " << cantColumnas << "\n";
  for (int i=0; i<cantFilas; i++){
    for (int j=0; j<cantColumnas; j++){
      cout << matriz[i][j] << " ";
    }
    cout << "\n";
  }
}

vector<double> Matriz::fila(int fila) {
  return matriz[fila];
}

Matriz Matriz::gauss(){
  Matriz m = Matriz(* this);

  for (int k=0; k<cantFilas; k++){
    for (int i=k+1; i<cantFilas; i++){
      double z = m.elem(i,k) / m.elem(k,k);
      m.put(i,k,0); 
      for (int j=k+1; j<cantColumnas; j++) {
        m.put(i, j , m.elem(i,j) - z * m.elem(k,j));
      }
    }
  }
  return m;
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
}

bool Matriz::esTriangularInferior(){
  for (int i=0; i<cantFilas; i++){
    for (int j=i+1; j<cantColumnas; j++){
      if (matriz[i][j] != 0){
        return false;
      }
    }
  }
}


Matriz Matriz::solucion(Matriz * b){
  Matriz sol = Matriz(b -> filas(), b -> columnas());
  double acum;
  int pasoVertical=cantColumnas-1;

  if (esTriangularSuperior()){
    //caso triangular superior: backward substitution
    for (int i=pasoVertical; i>=0; i--){
      acum = 0.0;
      if (matriz[i][i]!=0){
        for (int j=i+1; j<cantColumnas; j++){
          acum=acum+matriz[i][j]*sol.elem(j,0);
        }
        sol.put(i,0,(b -> elem(i,0)-acum)/matriz[i][i]);
      } else {
        sol.put(i,0,0.0);
      }
    }
    return sol;
  } else if (esTriangularInferior()){
    //caso triangular superior: forward substitution
    for (int i=0; i<cantColumnas; i++){
      acum = 0.0;
      if (matriz[i][i]!=0){
        for (int j=0; j<i; j++){
          cout << matriz[i][j]*sol.elem(j,0) << endl;
          acum=acum+matriz[i][j]*sol.elem(j,0);
        }
        sol.put(i,0,(b -> elem(i,0)-acum)/matriz[i][i]);
      } else {
        sol.put(i,0,0.0);
      }
    }
    return sol;
  } else {
    throw std::runtime_error("La matriz no es triangular");
  }
  
}

#include "Resolvedor.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace std;

Resolvedor::Resolvedor(Matriz& m){
  vector<Matriz> lu = m.lu();
  matriz = m;
  L = lu[0];
  U = lu[1];
}

Matriz Resolvedor::resolverTriangular(Matriz * m, Matriz * b){
  Matriz sol = Matriz(b -> filas(), b -> columnas());
  double acum;
  int pasoVertical=m->columnas()-1;

  if (m->esTriangularSuperior()){
    //caso triangular superior: backward substitution
    for (int i=pasoVertical; i>=0; i--){
      acum = 0.0;
      if (m->elem(i,i)!=0){
        for (int j=i+1; j<m->columnas(); j++){
          acum=acum+m->elem(i,j)*sol.elem(j,0);
        }
        sol.put(i,0,(b->elem(i,0)-acum)/m->elem(i,i));
      } else {
        sol.put(i,0,0.0);
      }
    }
    return sol;
  } else if (m->esTriangularInferior()){
    //caso triangular superior: forward substitution
    for (int i=0; i<m->columnas(); i++){
      acum = 0.0;
      if (m->elem(i,i)!=0){
        for (int j=0; j<i; j++){
          cout << m->elem(i,j)*sol.elem(j,0) << endl;
          acum=acum+m->elem(i,j)*sol.elem(j,0);
        }
        sol.put(i,0,(b -> elem(i,0)-acum)/m->elem(i,i));
      } else {
        sol.put(i,0,0.0);
      }
    }
    return sol;
  } else {
    throw std::runtime_error("La matriz no es triangular");
  }
}

Matriz Resolvedor::resolverUsandoLU(Matriz * b){
  Matriz y = resolverTriangular(&L,b);
  Matriz sol = resolverTriangular(&U, &y);
  return sol;
}

Matriz Resolvedor::resolverUsandoGauss(Matriz * b){
  Matriz m = Matriz(matriz);
  Matriz y = Matriz(*b);

  for (int k=0; k<m.filas(); k++){
    for (int i=k+1; i<m.filas(); i++){
      double z = m.elem(i,k) / m.elem(k,k);
      m.put(i,k,0); 
      for (int j=k+1; j<m.columnas(); j++) {
        m.put(i, j, m.elem(i,j) - z * m.elem(k,j));
      }
      y.put(i,0,y.elem(i,0)- z * y.elem(k,0));
    }
  }
  Matriz sol = resolverTriangular(&m, &y);
  return sol;
}
#include "Matriz.h"
#include <cmath>
#include <stdexcept>

using namespace std;

Matriz::Matriz(){
}

//Hay que armarla en base a radio, luego angulo.
Matriz::Matriz(Parametros param)
{
  vector< vector<double> > matrizCoeficientes;
  int n = param.n;
  int mMasUno = param.mMasUno;
  int cantidadIncognitas = n * mMasUno;
  double deltaRadio = param.deltaRadio;
  double deltaAngulo = param.deltaAngulo;
  double radioInterno = param.radioInterno;

  matrizCoeficientes.resize( cantidadIncognitas , vector<double>( cantidadIncognitas , 0 ) );

  for (int i = 0; i < n; ++i)
  {
    matrizCoeficientes[i][i] = 1;
    matrizCoeficientes[cantidadIncognitas-1-i][cantidadIncognitas-1-i] = 1;
  }

    //para moverse entre filas, cada una representa a un punto de la discretización
  for (int k = n; k < cantidadIncognitas-n; ++k)
  {
    int fAnguloActual = k % n;
    int fRadioActual = k / n;


      //Para moverse en una fila entre columnas, cada columna representa a una incognita.
    for (int j = 0; j < cantidadIncognitas; ++j)
    {
      int cAnguloActual = j % n;
      int cRadioActual = j / n;

      double coeficiente = 0;

        //calculo el radio actual, ya que va en las ecuaciones
      double rActual = radioInterno + (fRadioActual*deltaRadio);

        //coeficientes en función de (rActual, T_j,k)
      double a = ( ( 1 / ( deltaRadio*deltaRadio) ) - (1/(rActual*deltaRadio) ) );
      double b = ( ( 1 / ( rActual*deltaRadio) ) - ( 2/( deltaRadio*deltaRadio) ) - ( 2/( (deltaAngulo*deltaAngulo)*(rActual*rActual) ) ) );
      double c = ( ( 1 / ( deltaRadio*deltaRadio) ) );
      double d = ( ( 1 / ( ( rActual*rActual) * (deltaAngulo*deltaAngulo) ) ) );
      double e = ( ( 1 / ( ( rActual*rActual) * (deltaAngulo*deltaAngulo)) ) );

      {
          //caso borde con fAngulo = 0 dice que estoy la linea de angulo 0.
        if (fAnguloActual == 0) 
        {
          if (cRadioActual == fRadioActual -1)
          {
            if(cAnguloActual == fAnguloActual)
            {
              coeficiente = a;
            } 
          }
          if (cRadioActual == fRadioActual)
          {
            if (cAnguloActual == n-1)
            {
              coeficiente = d;
            }
            if (cAnguloActual == fAnguloActual)
            {
              coeficiente = b;
            }
            if (cAnguloActual == fAnguloActual +1)
            {
              coeficiente = e;
            }
          }
          if (cRadioActual == fRadioActual +1)
          {
            if(cAnguloActual == fAnguloActual)
            {
              coeficiente = c;
            }
          }
        }


        if (fAnguloActual == n-1) 
        {
          if (cRadioActual == fRadioActual -1)
          {
            if(cAnguloActual == fAnguloActual)
            {
              coeficiente = a;
            } 
          }
          if (cRadioActual == fRadioActual)
          {
            if (cAnguloActual == fAnguloActual -1)
            {
              coeficiente = d;
            }
            if (cAnguloActual == fAnguloActual)
            {
              coeficiente = b;
            }
            if (cAnguloActual == 0)
            {
              coeficiente = e;
            }
          }
          if (cRadioActual == fRadioActual +1)
          {
            if(cAnguloActual == fAnguloActual)
            {
              coeficiente = c;
            } 
          }
        }


        else
        {
          if (cRadioActual == fRadioActual -1)
          {
            if(cAnguloActual == fAnguloActual)
            {
              coeficiente = a;
            } 
          }
          if (cRadioActual == fRadioActual)
          {
            if (cAnguloActual == fAnguloActual -1)
            {
              coeficiente = d;
            }
            if (cAnguloActual == fAnguloActual)
            {
              coeficiente = b;
            }
            if (cAnguloActual == fAnguloActual +1)
            {
              coeficiente = e;
            }
          }
          if (cRadioActual == fRadioActual +1)
          {
            if(cAnguloActual == fAnguloActual)
            {
              coeficiente = c;
            } 
          }
        }


      }
      matrizCoeficientes[k][j] = coeficiente;
    }
  }

  matriz = matrizCoeficientes;
  cantFilas = matrizCoeficientes.size();
  cantColumnas = cantFilas;
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
    for (int j=0; j< m -> columnas(); j++){
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

Matriz Matriz::diferencia(Matriz b){
  Matriz diff = Matriz(cantFilas, cantColumnas);
  for (int i=0; i<cantFilas; i++){
    for (int j = 0; j < cantColumnas; j++)
    {
      diff.matriz[i][j] = (matriz[i][j] - b.matriz[i][j]);
    }
  }
  return diff;
}

double Matriz::maxElem() const{
  double maxElem = 0;
  for (int i=0; i<cantFilas; i++){
    for (int j = 0; j < cantColumnas; j++)
    {
      if(abs(matriz[i][j]) > maxElem)
        maxElem = (matriz[i][j]);
    }
  }
  return maxElem;
}


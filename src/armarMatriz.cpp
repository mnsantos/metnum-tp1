#include <stdio.h>
#include <vector>
#include <iostream>
#include "Matriz.h"
#include "Resolvedor.h"
#include <fstream>

using namespace std;
//Entrada
//(ri , re , m+1, n, vi, Ti , Te(θ))
//10 100 30 30 500
//1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 

//variables
double radioInterno;
double radioExterno;
int mMasUno; // cantRadios
int n; // cantAngulos
double valorIsoterma;
int nInst;
double deltaRadio;
double deltaAngulo;
int cantidadIncognitas;
vector< vector<double> > matrizCoeficientes;
vector< vector <double> > b_1;

void entrada(string path){

  ifstream myReadFile;
  myReadFile.open(path.c_str());
  if (myReadFile.is_open()) {
    myReadFile >> radioInterno >> radioExterno >> mMasUno >> n >> valorIsoterma >> nInst;
    cantidadIncognitas = n*mMasUno;
    matrizCoeficientes.resize( cantidadIncognitas , vector<double>( cantidadIncognitas , 0 ) );
    b_1.resize( cantidadIncognitas , vector<double>( cantidadIncognitas , 0 ) );
    
    for (int i = 0; i < nInst; ++i)
    {
      for (int j = 0; j < n; ++j)
      {
        myReadFile >> b_1[i][j];
      }
      
      for (int j = 0; j < n; ++j)
      {
        myReadFile >> b_1[i][cantidadIncognitas-1-j];
      }
    }

    for (int i = 0; i < n; ++i)
      {
        matrizCoeficientes[i][i] = 1;
      }
      
      for (int i = 0; i < n; ++i)
      {
        matrizCoeficientes[cantidadIncognitas-1-i][cantidadIncognitas-1-i] = 1;
      }
  }

  //cantidad de segmentos de radio
  deltaRadio = (radioExterno-radioInterno)/n;

  //cantidad de segmentos de angulo
  deltaAngulo = 360/n;

}


//Matriz de coeficientes donde rActual es dato y T_j,k es incógnita.
//Hay que armarla en base a radio, luego angulo.
void armado(){
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
        if (cRadioActual == fRadioActual -1 )
        {
          if (cAnguloActual == fAnguloActual)
          {
            coeficiente = a;
          }
        }
        if (cRadioActual == fRadioActual)
        {
          if (cAnguloActual == fAnguloActual - 1 || cAnguloActual == n-1 )
          {
            coeficiente = d;
          }
          if (cAnguloActual == fAnguloActual)
          {
            coeficiente = b;
          }
          if (cAnguloActual == fAnguloActual + 1 || cAnguloActual == n )
          {
            coeficiente = e;
          }
        }
        if (cRadioActual == fRadioActual + 1 )
        {
          if (cAnguloActual == fAnguloActual)
          {
            coeficiente = c;
          }
        }
      }

      //asigno a la matriz de coeficientes el coeficiente correspondiente
      if (matrizCoeficientes[k][j] == 0)
      {
        matrizCoeficientes[k][j] = coeficiente;
      }
    }
  }
}

void salida(){
  for (int k = 0; k < cantidadIncognitas; ++k)
  {
    int anguloActual = k / n;
    int radioActual = k % n;

    //cout << " fila: "<< k << " _ " << "anguloActual: " << anguloActual << "radioActual: " << radioActual << endl;
    //Para moverse en una fila entre columnas, cada columna representa a una incognita.
    for (int j = 0; j < cantidadIncognitas; ++j)
    {
      //cout << " col: "<< j << " | "; //<< "anguloActual: " << anguloActual << "radioActual: " << radioActual;
      cout << matrizCoeficientes[k][j] << " | ";
    }
    cout << endl;
  } 
}

 int main( int argc, const char* argv[]){

  entrada(argv[1]);

  armado();
  //salida();
  Matriz m = Matriz(matrizCoeficientes);
  // cout <<"traspaso de matriz" << endl;
  // cout << m;
  // cout <<"fin de matriz"<< endl;
  ofstream outFile;
  outFile.open (argv[2]);
  for (int i = 0; i < nInst; ++i)
    {
      Matriz b = Matriz(b_1[i]);
      // cout <<"traspaso de b" << endl;
      // cout << b;
      // cout <<"fin de b"<< endl;
      Resolvedor r = Resolvedor(m);
      
      outFile << r.resolverUsandoLU(&b);
    }

 }
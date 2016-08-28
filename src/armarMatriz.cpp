#include <stdio.h>
#include <vector>
#include <iostream>
using namespace std;
//Entrada
//(ri , re , m+1, n, vi, Ti , Te(θ))
//10 100 30 30 500
//1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 1500 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 

//variables
double radioInterno;
double radioExterno;
int mMasUno;
int n;
double valorIsoterma;
double deltaRadio;
double deltaAngulo;
int cantidadIncognitas;
vector< vector<double> > matrizCoeficientes;

void entrada(){
  cin >> radioInterno;
  cin >> radioExterno;
  cin >> mMasUno;
  cin >> n;
  cin >> valorIsoterma;
  cantidadIncognitas = n*n;
  matrizCoeficientes.resize( cantidadIncognitas , vector<double>( cantidadIncognitas , 0 ) );

  for (int i = 0; i < n; ++i)
  {
    matrizCoeficientes[i][i] = 1;
    /*cin >> */matrizCoeficientes[i][cantidadIncognitas] = 1500;
  }
  
  for (int i = 0; i < n; ++i)
  {
    matrizCoeficientes[cantidadIncognitas-1-i][cantidadIncognitas-1-i] = 1;
    /*cin >> */matrizCoeficientes[cantidadIncognitas-1-i][cantidadIncognitas] = 2;
  }

  //cantidad de segmentos de radio
  deltaRadio = (radioExterno-radioInterno)/n;

  //cantidad de segmentos de angulo
  deltaAngulo = 360/n;

}


//Matriz de coeficientes donde rActual es dato y T_j,k es incógnita.
//Hay que armarla en base a ángulo, luego radio, por pedido de la cátedra.
void armado(){
  //para moverse entre filas, cada una representa a un punto de la discretización
  for (int k = n; k < cantidadIncognitas-n; ++k)
  {
    int iAnguloActual = k / n;
    int iRadioActual = k % n;
      
    //Para moverse en una fila entre columnas, cada columna representa a una incognita.
    for (int j = 0; j < cantidadIncognitas; ++j)
    {
      int jAnguloActual = j / n;
      int jRadioActual = j % n;

      double coeficiente = 0;

      //calculo el radio actual, ya que va en las ecuaciones
      double rActual = radioInterno + (iRadioActual*deltaRadio);

      //coeficientes en función de (rActual, T_j,k)
      double a = ( (1/rActual*rActual) - (1/rActual*deltaRadio) );
      double b = ( (1/(rActual*deltaRadio)) - (2/(deltaRadio*deltaRadio)) - (2/((deltaAngulo*deltaAngulo)*(deltaRadio*deltaRadio))) );
      double c = ( (1/(deltaRadio*deltaRadio)) );
      double d = ( (1/((rActual*rActual) * (deltaAngulo*deltaAngulo))) );
      double e = ( (1/((rActual*rActual)*(deltaAngulo*deltaAngulo))) );

      if (jRadioActual == iRadioActual -1 )
      {
        if (jAnguloActual == iAnguloActual)
        {
          coeficiente = a;
        }
      }
      if (jRadioActual == iRadioActual)
      {
        if (jAnguloActual == iAnguloActual -1 )
        {
          coeficiente = d;
        }
        if (jAnguloActual == iAnguloActual)
        {
          coeficiente = b;
        }
        if (iRadioActual == iRadioActual +1 )
        {
          coeficiente = e;
        }
      }
      if (jRadioActual == iRadioActual +1 )
      {
        if (jAnguloActual == iAnguloActual)
        {
          coeficiente = c;
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
    for (int j = 0; j <= cantidadIncognitas; ++j)
    {
      //cout << " col: "<< j << " | "; //<< "anguloActual: " << anguloActual << "radioActual: " << radioActual;
      cout << matrizCoeficientes[k][j] << " | ";
    }
    cout << endl;
  } 
}

// int main( int argc, const char* argv[] ){
//  entrada();
//  armado();
//  salida();
// }
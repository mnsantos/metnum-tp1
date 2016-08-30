#include <stdio.h>
#include <vector>
#include <iostream>
#include "Matriz.h"
#include "Resolvedor.h"
#include <fstream>
#include "Parametros.h"
#include "FileManager.h"

using namespace std;

//Matriz de coeficientes donde rActual es dato y T_j,k es incógnita.
//Hay que armarla en base a radio, luego angulo.
Matriz armado(Parametros param)
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

  return Matriz(matrizCoeficientes);
}

void salida(vector<Matriz> xs, FileManager manager)
{
    manager.write(xs);
}

int main(int argc, char **argv)
{
  clock_t inicio_, final_;
  // desde cuando quiero medir
  inicio_=clock();

  //lectura de parámetros y archivos de entrada
  FileManager manager = FileManager(argv[1], argv[2]);
  //arma la matriz de coeficientes a resolver.
  Parametros params = manager.read();
  Matriz A = armado(params);
  Resolvedor resolvedor = Resolvedor(A);
  //cout << "A: " <<endl;
  //cout << A;
  //cout << "B: " <<endl;
  //cout << params.bs[0];
  //matriz de soluciones (cada solución es una columna)
  vector<Matriz> xs;

  //resuelve la cantidad de instancias que nos pasen
  for (int i = 0; i < params.nInst; ++i)
  {
    //resuelve usando el método pedido por parámetro y ubica en un vector de soluciones
    if(argv[3] == "0"){
      xs.push_back(resolvedor.resolverUsandoGauss(&params.bs[i]));
    } else {
      xs.push_back(resolvedor.resolverUsandoLU(&params.bs[i]));
    }
    //cout << "x: " <<endl;
    //cout << xs[0];
  }

  salida(xs, manager);

  final_=clock();
  ///////////////////////////////
  //printf("El Tiempo es: %f\n",(final_ - inicio_)/CLK_TCK);
}
  
double temp(int radio, int angulo, vector<double> sol, Parametros param)
{
  int i = 0;
  i =  radio * param.mMasUno;
  i += angulo;
  return sol[i];
}
/*
vector<int, int> hallarRadios(vector<double> sol, double isoterma) {
    vector<int> radiosMinimos;
    vector<int> radiosMaximos;
    for (int i=0; i < cantRadios; i++) {
      for (int j=0; j < cantAngulos; j++) {
        double temp = t(i, j, sol);
        if (temp < isoterma) {
          radiosMinimos.push_back(i-1);
          radiosMaximos.push_back(i);
        }
      }
    }
    int radioMinimo = * min_element(v.begin(), v.end());
    int radioMaximo = * max_element(v.begin(), v.end());
    vector<double> res;
    res.push_back(radioMinimo, radioMaximo);
    return res;
}

vector<double> armarNuevosParametros(vector<double> sol, double isoterma) {
  vector<double> radios = hallarRadios(sol, isoterma);
  int radioMinimo = radios[0];
  int radioMaximo = radios[1];
  vector<double> temperaturasInternas;
  vector<double> temperaturasExternas;
  for (int i=0; i < cantAngulos; i++) {
    temperaturasInternas.push_back(t(radioMinimo, j, sol));
    temperaturasExternas.push_back(t(radioMaximo, j, sol));
  }
  double nuevoRadioInterno = deltaRadio * radioMinimo;
  double nuevoRadioExterno = deltaRadio * radioMaximo;
  vector<double> nuevosParametros;
  nuevosParametros.push_back(nuevoRadioInterno);
  nuevosParametros.push_back(nuevoRadioExterno);
  nuevosParametros.push_back(n);
  nuevosParametros.push_back(mMasUno);
  for (int i=0; i < temperaturasInternas.size(); i++){
    nuevosParametros.push_back(temperaturasInternas[i]);
  }
  for (int i=0; i < temperaturasExternas.size(); i++){
    nuevosParametros.push_back(temperaturasExternas[i]);
  }
  return nuevosParametros;
}

*/
  
/*
vector<double > calcularIsoterma(vector<double> sol, double isoterma) {
  vector<double > radiosIsoterma;
  double Tactual=0.0;
  double Tsiguiente=0.0; 
  for(int ang=0; ang< cantAngulos; ang++){
    for(int rad=0; rad< cantRadios-1; rad++){
      Tactual=t(rad, ang, sol);
      Tsiguiente=t(rad+1, ang, sol);
      if(Tactual >= isoterma && Tsiguiente < isoterma){
        double valor=(rad*actual + (rad-1)*siguiente)/(actual+siguiente);
        radiosIsoterma.push_back(valor);
      }
    } 
  }
}
*/
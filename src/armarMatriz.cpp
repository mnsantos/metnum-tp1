#include <stdio.h>
#include <vector>
#include <iostream>
#include "Matriz.h"
#include "Resolvedor.h"
#include <fstream>

using namespace std;
//Entrada
//(ri , re , m+1, n, vi, Ti , Te(θ))

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

      //asigno a la matriz de coeficientes el coeficiente correspondiente
      //if (matrizCoeficientes[k][j] == 0)
      //{
        matrizCoeficientes[k][j] = coeficiente;
      //}
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

Matriz resolver(double rI, double rE, Matriz b)
{
  radioInterno = rI;
  radioExterno = rE;
  mMasUno = mMU;
  n = nN;
  for (int i = 0; i < b.size(); ++i)
  {
    b_1[i] = b[i];
  }
  for (int i = 0; i < n; ++i)
  {
    matrizCoeficientes[i][i] = 1;
  }

  for (int i = 0; i < n; ++i)
  {
    matrizCoeficientes[cantidadIncognitas-1-i][cantidadIncognitas-1-i] = 1;
  }
  armado();
}

Matriz resolver(String path)
{
  entrada(string path);
  armado();
}

 int main( int argc, const char* argv[]){
  clock_t inicio_, final_;
  // desde cuando quiero medir
  inicio_=clock();
  ///////////////////////////////
  resolver(argv[1]);
  
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
  // hasta cuando quiero medir
  final_=clock();
  ///////////////////////////////
    printf("El Tiempo es: %f\n",(final_ - inicio_)/CLK_TCK);
    outFile << r.resolverUsandoLU(&b);
  }



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
<<<<<<< HEAD
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
  
=======
  }

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
	return radiosIsoterma;
  }

  
>>>>>>> 3d289a33d7a2753dec1db48a71e04b7dbadb60d4

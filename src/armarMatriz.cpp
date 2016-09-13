#include <stdio.h>
#include <vector>
#include <iostream>
#include "Matriz.h"
#include "Resolvedor.h"
#include "Parametros.h"
#include "FileManager.h"
#include <fstream>

using namespace std;

void salida(vector<Matriz> xs, vector<Matriz> isotermas, FileManager manager)
{
    manager.write(xs, isotermas);
}

double temp(int radio, int angulo, Matriz sol, Parametros param)
{
  int i = 0;
  i =  radio * param.mMasUno-1;
  i += angulo;
  return sol.elem(i,0);
}

vector<double > calcularIsoterma(Parametros params, Matriz sol) 
{
  vector<double > radiosIsoterma;
  double Tactual = 0.0;
  double Tsiguiente = 0.0; 
  double cantRadios = params.mMasUno;
  double cantAngulos = params.n;
  for(int ang=0; ang< cantAngulos; ang++){
    for(int rad=0; rad< cantRadios-1; rad++){
      Tactual=temp(rad, ang, sol, params);
      Tsiguiente=temp(rad+1, ang, sol, params);
      double valor;
      // habria que chequear si no es ~= en vez de =
      if(Tactual == params.valorIsoterma) {
        valor = Tactual;
        radiosIsoterma.push_back(params.radioInterno + valor * params.deltaRadio);
        break;
      }
      else if(Tsiguiente < params.valorIsoterma){
        //este calculo esta bien?
        valor =(rad*Tactual + (rad+1)*Tsiguiente)/(Tactual+Tsiguiente);
        radiosIsoterma.push_back(params.radioInterno + valor * params.deltaRadio);
        break;
      }
    }
  }
  return radiosIsoterma;
}

/*
vector<int, int> hallarRadios(Matriz sol, Parametros params) {
    vector<int> radiosMinimos;
    vector<int> radiosMaximos;
    for (int i=0; i < cantRadios; i++) {
      for (int j=0; j < cantAngulos; j++) {
        double temp = t(i, j, sol, params);
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

vector<double> armarNuevosParametros(vector<double> sol, double isoterma, vector<double> radios) {
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
}*/

int main(int argc, char **argv)
{
  clock_t inicio_, final_;
  // desde cuando quiero medir
  inicio_=clock();

  //lectura de parámetros y archivos de entrada
  FileManager manager = FileManager(argv[1], argv[2]);
  //arma la matriz de coeficientes a resolver.
  Parametros params = manager.read();
  //Matriz de coeficientes donde rActual es dato y T_j,k es incógnita.
  Matriz A = Matriz(params);
  Resolvedor resolvedor = Resolvedor(A);
  //matriz de soluciones (cada solución es una columna)
  vector<Matriz> xs;
  vector<Matriz> isotermas;

  //resuelve la cantidad de instancias que nos pasen
  for (int i = 0; i < params.nInst; ++i)
  {
    //resuelve usando el método pedido por parámetro y ubica en un vector de soluciones
    Matriz sol;
    if(argv[3] == "0"){
      sol = resolvedor.resolverUsandoGauss(&params.bs[i]);
      
    } else {
      sol = resolvedor.resolverUsandoLU(&params.bs[i]);
    }
    isotermas.push_back(calcularIsoterma(params, sol));
    xs.push_back(sol);
  }

  salida(xs, isotermas, manager);

  final_=clock();
  //printf("El Tiempo es: %f\n",(final_ - inicio_)/CLK_TCK);
  }
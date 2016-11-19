#include <stdio.h>
#include <vector>
#include <iostream>
#include <cmath>
#include "Matriz.h"
#include "Resolvedor.h"
#include "Parametros.h"
#include "FileManager.h"
#include <fstream>
#include <algorithm> 

using namespace std;

double CONSTANTE = 0.25;
bool DISCRETIZAR_NUEVAMENTE = true;
vector<int> SUBDISCRETIZACIONES;

void salida(vector<Matriz> xs, vector<Matriz> isotermas, string metodo, vector<double> peligrosidades, Parametros params, vector<int> subdiscretizaciones, vector<double> tiemposPorInstancia, double tiempoTotal, FileManager manager) {
  manager.write(xs, isotermas, metodo, peligrosidades, subdiscretizaciones, tiemposPorInstancia, tiempoTotal, params);
}

double temp(int radio, int angulo, Matriz sol, Parametros param) {
  int i = 0;
  i =  radio * param.n;
  i += angulo;
  return sol.elem(i,0);
}

Matriz calcularIsoterma(Parametros params, Matriz sol) {
  double Tactual = 0.0;
  double Tsiguiente = 0.0; 
  double cantRadios = params.mMasUno;
  double cantAngulos = params.n;
  Matriz radiosIsoterma(cantAngulos, 1);
  for(int ang=0; ang< cantAngulos; ang++){
    for(int rad=0; rad< cantRadios-1; rad++){
      //cout<< "ang: " << ang<<endl;
      //cout<< "rad: " << rad<<endl;
      Tactual=temp(rad, ang, sol, params);
      Tsiguiente=temp(rad+1, ang, sol, params);
      //cout << "obtuvo T" << endl;
      double valor;
      //cout << "isoterma: " << params.valorIsoterma << endl;
      // habria que chequear si no es ~= en vez de =
      if(Tactual == params.valorIsoterma) {
        valor = Tactual;
        radiosIsoterma.put(ang, 0, params.radioInterno + valor * params.deltaRadio);
        break;
      }
      else if(Tsiguiente < params.valorIsoterma){
        valor =(rad/(Tactual-params.valorIsoterma) + (rad+1)/(params.valorIsoterma-Tsiguiente))/(1/(Tactual-params.valorIsoterma)+1/(params.valorIsoterma-Tsiguiente));
        radiosIsoterma.put(ang, 0, params.radioInterno + valor * params.deltaRadio);
        break;
      }
    }
  }
  return radiosIsoterma;
}

double medirPeligrosidad(Matriz radiosIsoterma, Parametros params) {
  double radioMaximo = params.radioInterno;
  for (int i=0; i<radiosIsoterma.filas(); i++){
    if (radiosIsoterma.elem(i,0) > radioMaximo){
      radioMaximo = radiosIsoterma.elem(i,0);
    }
  }
  return (radioMaximo - params.radioInterno) / (params.radioExterno - params.radioInterno);
}

vector<double> hallarRadios(Matriz sol, Parametros params) {
  //cout << "hallarRadios" << endl;
  vector<int> radiosMinimos;
  vector<int> radiosMaximos;

  for (int i=0; i < params.n; i++) {
    for (int j=0; j < params.mMasUno-1; j++) {
      double t = temp(j, i, sol, params);
      if (t < params.valorIsoterma) {
        radiosMinimos.push_back(j-1);
        radiosMaximos.push_back(j);
        break;
      }
    }
  }

  int radioMinimo = * min_element(radiosMinimos.begin(), radiosMinimos.end());
  int radioMaximo = * max_element(radiosMaximos.begin(), radiosMaximos.end());
  vector<double> res;
  res.push_back(radioMinimo);
  res.push_back(radioMaximo);
  return res;
}

bool hayQueDiscretizarNuevamente(Matriz sol, Parametros params) {
  vector<double> radios = hallarRadios(sol, params);
  for (int j=0; j<params.n; j++){
    if ( (abs(temp(radios[0],j,sol,params)-params.valorIsoterma) > CONSTANTE) || (abs(temp(radios[0],j,sol,params)-params.valorIsoterma) > CONSTANTE) ) {
      return true;
    }
  }
  return false;
}

Parametros nuevosParametros(Matriz sol, Parametros params) {
  vector<double> radios = hallarRadios(sol, params);
  int radioMinimo = radios[0];
  //cout<<"Radio minimo "<<radioMinimo<<endl;
  int radioMaximo = radios[1];
  //cout<<"Radio maximo "<<radioMaximo<<endl;
  double nuevoRadioInterno = params.radioInterno + params.deltaRadio * radioMinimo;
  double nuevoRadioExterno = params.radioInterno + params.deltaRadio * radioMaximo;
  
  Parametros nuevosParametros;
  nuevosParametros.radioInterno = nuevoRadioInterno;
  nuevosParametros.radioExterno = nuevoRadioExterno;
  nuevosParametros.mMasUno = params.mMasUno;
  nuevosParametros.n = params.n;
  nuevosParametros.valorIsoterma = params.valorIsoterma;
  nuevosParametros.nInst = params.nInst;
  nuevosParametros.deltaAngulo = params.deltaAngulo;
  nuevosParametros.deltaRadio = (nuevosParametros.radioExterno - nuevosParametros.radioInterno) / (nuevosParametros.mMasUno-1);

  double cantidadIncognitas = params.n*params.mMasUno;

  Matriz b = Matriz(cantidadIncognitas,1);

  for (int j = 0; j < params.n; ++j) {
    b.put(j, 0, temp(radioMinimo, j, sol, params));
  }
  for (int j = params.n; j < cantidadIncognitas-(params.n); ++j) {
    b.put(j, 0, 0);
  }
  int h = 0;
  for (int j = cantidadIncognitas-(params.n); j < cantidadIncognitas; ++j) {
    b.put(j, 0, temp(radioMaximo, h, sol, params));
    h++;
  }
  vector<Matriz> bs;
  bs.push_back(b);
  nuevosParametros.bs = bs;
  return nuevosParametros;
}

Matriz resolver(Parametros params, int i, string metodo, Resolvedor resolvedor) {
  Matriz sol;
  //matriz de soluciones (cada solución es una columna)
  if(metodo == "0"){
    sol = resolvedor.resolverUsandoGauss(&params.bs[i]);
  } else {
    sol = resolvedor.resolverUsandoLU(&params.bs[i]);
  }
  return sol;
}

Matriz obtenerIsoterma(Matriz sol, Parametros params, int i, string metodo, Resolvedor r){
  Parametros copyParms = params;
  if (DISCRETIZAR_NUEVAMENTE) {
    int j = 1;
    SUBDISCRETIZACIONES.push_back(0);
    while (hayQueDiscretizarNuevamente(sol, copyParms)) {
      SUBDISCRETIZACIONES[i] ++;
      cout << j <<endl;
      copyParms = nuevosParametros(sol, copyParms);
      sol = resolver(copyParms, i, metodo, r);
      j++;
    }
  }
  return calcularIsoterma(params, sol);
}

int main(int argc, char **argv) {

  //Lectura de parámetros y archivos de entrada y salida
  FileManager manager = FileManager(argv[1], argv[2]);
  Parametros params = manager.read();
  string metodo = argv[3];

  //Comienzo de mediciones para informe
  clock_t inicio, final, inicioAux, finAux;
  vector<double> tiemposPorInstancia;
  inicio = clock();
  //Resolución con la discretización pedida por parámetro
  vector<Matriz> soluciones;
  vector<Matriz> isotermas;
  vector<double> peligrosidades;
  //arma la matriz de coeficientes a resolver.
  Matriz m = Matriz(params);
  Resolvedor resolvedor = Resolvedor(m);
  for (int i=0; i<params.nInst; i++){
    inicioAux = clock();
    Matriz sol = resolver(params, i, metodo, resolvedor);
    soluciones.push_back(sol);
    Matriz isot = obtenerIsoterma(sol, params, i, metodo, resolvedor);
    finAux = clock();
    isotermas.push_back(isot);
    peligrosidades.push_back(medirPeligrosidad(isot, params));
    tiemposPorInstancia.push_back(double(finAux - inicioAux) / CLOCKS_PER_SEC);
  }
  final = clock();

  double total = double(final - inicio) / CLOCKS_PER_SEC;

  //for (int i=0; i<params.mMasUno ; i++){
  //  cout << xs[0].elem(0,i) << endl;
  //}
  //cout << xs[0].filas() <<endl;
  //cout << xs[0].columnas() <<endl;
  //for (int i=0; i<50; i++){
  //  cout << xs[0].elem(i,0) << endl;
  //}
  //Obtención de isoterma mediante refinamiento de las soluciones y promedios
  //vector<Matriz> isotermas = obtenerIsotermas(xs, params, metodo);

  salida(soluciones, isotermas, metodo, peligrosidades, params, SUBDISCRETIZACIONES, tiemposPorInstancia, total, manager);

  //tiempo ejecución
  //¡params entrada! usados
  //peligrosidad
  //printf("El Tiempo es: %f\n",(final_ - inicio_)/CLK_TCK);
}
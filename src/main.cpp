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

double CONSTANTE = 0.5;
bool DISCRETIZAR_NUEVAMENTE = false; 
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
  //cout << "pre for" << endl;
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
        valor = rad;
        radiosIsoterma.put(ang, 0, params.radioInterno + valor * params.deltaRadio);
        break;
      }
      else if(Tsiguiente < params.valorIsoterma){
        valor = (rad/(Tactual-params.valorIsoterma) + (rad+1)/(params.valorIsoterma-Tsiguiente))/(1/(Tactual-params.valorIsoterma)+1/(params.valorIsoterma-Tsiguiente));
        radiosIsoterma.put(ang, 0, params.radioInterno + valor * params.deltaRadio);
        break;
      }
      else if(Tsiguiente == params.valorIsoterma){
        //cout << "puto!" << endl;
        valor = rad+1;
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
  ////cout << sol << endl;
  //////cout << "hallarRadios" << endl;
  vector<int> radiosMinimos;
  vector<int> radiosMaximos;

  for (int angulo=0; angulo < params.n; angulo++) {
    for (int radio=0; radio < params.mMasUno; radio++) {
      ////cout << "pre temp" <<endl;
      double t = temp(radio, angulo, sol, params);
      ////cout << "t: " << t << endl;
      //////cout << "post temp" <<endl;
      if (t < params.valorIsoterma) {
        radiosMinimos.push_back(radio-1);
        radiosMaximos.push_back(radio);
        break;
      } else if (t == params.valorIsoterma)
      {
      //  cout << "entra!" <<endl;
        radiosMaximos.push_back(radio);
        radiosMinimos.push_back(radio);
      }
    }
  }
  ////cout << radiosMinimos[0] << endl;
  ////cout << "sali del for de hallarRadios" <<endl;
  int radioMinimo = * min_element(radiosMinimos.begin(), radiosMinimos.end());
  ////cout << "radioMinimo: " << radioMinimo << endl;
  int radioMaximo = * max_element(radiosMaximos.begin(), radiosMaximos.end());
  ////cout << "radioMaximo: " << radioMaximo << endl;
  vector<double> res;
  ////cout << "marca 3" << endl;
//  cout << "t radioMinimo 0: " << temp(radioMinimo, 0, sol, params) << endl;
//  cout << "t radioMaximo 0: " << temp(radioMaximo, 0, sol, params) << endl;
//  cout << "t radioMinimo 1: " << temp(radioMinimo, 1, sol, params) << endl;
//  cout << "t radioMaximo 1: " << temp(radioMaximo, 1, sol, params) << endl;
  res.push_back(radioMinimo);
  ////cout << "marca 4" << endl;
  res.push_back(radioMaximo);
  ////cout << "marca 5" << endl;
  return res;
}

bool hayQueDiscretizarNuevamente(Matriz sol, Parametros params) {
  ////cout << "voy a hallarRadios" << endl;
  vector<double> radios = hallarRadios(sol, params);
  ////cout << "pre for hay que DISCRETIZAR_NUEVAMENTE" << endl;
  for (int j=0; j<params.n; j++){
    ////cout << "in for: " << j <<endl;
    if ( (abs(temp(radios[0],j,sol,params)-params.valorIsoterma) < CONSTANTE) || (abs(temp(radios[1],j,sol,params)-params.valorIsoterma) < CONSTANTE) ) {
      ////cout << "entro al if" << endl;
      return false;
    }
  }
  ////cout << "post for hay que DISCRETIZAR_NUEVAMENTE" << endl;
  return true;
}

Parametros nuevosParametros(Matriz sol, Parametros params) {
  vector<double> radios = hallarRadios(sol, params);
  int radioMinimo = radios[0];
  //////cout<<"Radio minimo "<<radioMinimo<<endl;
  int radioMaximo = radios[1];
  //////cout<<"Radio maximo "<<radioMaximo<<endl;
  double nuevoRadioInterno = params.radioInterno + params.deltaRadio * radioMinimo;
  double nuevoRadioExterno = params.radioInterno + params.deltaRadio * radioMaximo;
  
  Parametros nuevosParametros;
  nuevosParametros.radioInterno = nuevoRadioInterno;
//  cout << "nuevoRadioInterno: " << nuevoRadioInterno << endl;
  nuevosParametros.radioExterno = nuevoRadioExterno;
//  cout << "nuevoRadioExterno: " << nuevoRadioExterno << endl;
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
  cout << "entra a resolver" << endl;
  //matriz de soluciones (cada solución es una columna)
  if(metodo == "0"){
    cout << "va a resolver por Gauss"<<endl;
    sol = resolvedor.resolverUsandoGauss(&params.bs[i]);
  } else {
    cout << "va a resolver por LU"<<endl;
    sol = resolvedor.resolverUsandoLU(&params.bs[i]);
  }
  return sol;
}

Matriz obtenerIsoterma(Matriz sol, Parametros params, int i, string metodo){
  Parametros copyParms = params;
  Parametros prevParams;
  if (DISCRETIZAR_NUEVAMENTE) {
    int j = 1;
    SUBDISCRETIZACIONES.push_back(0);
    while (hayQueDiscretizarNuevamente(sol, copyParms)) {
      Parametros copyParms2;
      //cout << "inicio while" << endl;
      SUBDISCRETIZACIONES[i]++;
      //cout << j <<endl;
      prevParams = copyParms;
      copyParms = nuevosParametros(sol, copyParms);
      if (prevParams.radioExterno == copyParms.radioExterno && prevParams.radioInterno == copyParms.radioInterno){
        break;
      }
      cout << "pre sol" << endl;
      //arma la matriz de coeficientes a resolver.
      cout << 1 << endl;
      Matriz m = Matriz(copyParms);
      cout << 2 << endl;
      Resolvedor resolvedor = Resolvedor(m, metodo);
      cout << 3 << endl;
      sol = resolver(copyParms, 0, metodo, resolvedor);
      cout << 4 << endl;
      j++;
      cout << "fin while" << endl;
    }
  }
  cout << "voy a calcular isoterma" << endl;
  return calcularIsoterma(copyParms, sol);
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
  inicioAux = clock();
  Resolvedor resolvedor = Resolvedor(m, metodo);
  for (int i=0; i<params.nInst; i++){
    if (i!=0) {
      inicioAux = clock();
    }
    cout << "0" << endl;
    Matriz sol = resolver(params, i, metodo, resolvedor);
    cout << "1" << endl;
    soluciones.push_back(sol);
    Matriz isot = obtenerIsoterma(sol, params, i, metodo);
    cout << "2" << endl;
    finAux = clock();
    isotermas.push_back(isot);
    peligrosidades.push_back(medirPeligrosidad(isot, params));
    cout << "3" << endl;
    tiemposPorInstancia.push_back(double(finAux - inicioAux) / CLOCKS_PER_SEC);
    cout << "4" << endl;
  }
  final = clock();

  double total = double(final - inicio) / CLOCKS_PER_SEC;

  salida(soluciones, isotermas, metodo, peligrosidades, params, SUBDISCRETIZACIONES, tiemposPorInstancia, total, manager);

  //tiempo ejecución
  //¡params entrada! usados
  //peligrosidad
  //printf("El Tiempo es: %f\n",(final_ - inicio_)/CLK_TCK);
}


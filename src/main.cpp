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

void salida(vector<Matriz> xs, vector<Matriz> isotermas, FileManager manager) {
  manager.write(xs, isotermas);
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
  Matriz radiosIsoterma(cantRadios, 1);
  for(int ang=0; ang< cantAngulos; ang++){
    for(int rad=0; rad< cantRadios-1; rad++){
      Tactual=temp(rad, ang, sol, params);
      Tsiguiente=temp(rad+1, ang, sol, params);
      //cout<<Tactual<<endl;
      //cout<<Tsiguiente<<endl;
      double valor;
      // habria que chequear si no es ~= en vez de =
      if(Tactual == params.valorIsoterma) {
        valor = Tactual;
        radiosIsoterma.put(ang, 0, params.radioInterno + valor * params.deltaRadio);
        break;
      }
      else if(Tsiguiente < params.valorIsoterma){
        //este calculo esta bien?
        //cout<<"entre"<<endl;
        valor =(rad/(Tactual-params.valorIsoterma) + (rad+1)/(params.valorIsoterma-Tsiguiente))/(1/(Tactual-params.valorIsoterma)+1/(params.valorIsoterma-Tsiguiente));
        radiosIsoterma.put(ang, 0, params.radioInterno + valor * params.deltaRadio);
        break;
      }
    }
  }
  return radiosIsoterma;
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
  //cout << res << endl;
  //cout<<"termine radios"<<endl;
  return res;
}

bool hayQueDiscretizarNuevamente(Matriz sol, Parametros params) {
  //cout<<"entreee0"<<endl;
  //cout<<sol<<endl;
  double Tactual = 0.0;
  double Tsiguiente = 0.0; 
  double cantRadios = params.mMasUno;
  double cantAngulos = params.n;
  for(int ang=0; ang< cantAngulos; ang++){
    for(int rad=0; rad< cantRadios-1; rad++){
      Tactual=temp(rad, ang, sol, params);
      Tsiguiente=temp(rad+1, ang, sol, params);
      if (Tsiguiente < params.valorIsoterma){
        if ((Tactual - params.valorIsoterma) > CONSTANTE && (abs(Tsiguiente - params.valorIsoterma) > CONSTANTE)){
          //cout << (Tactual - params.valorIsoterma) << endl;
          return true;
        }
      }
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

Matriz resolver(Parametros params, int i, string metodo) {
  //matriz de soluciones (cada solución es una columna)
  vector<Matriz> xs;
  //arma la matriz de coeficientes a resolver.
  Matriz A = Matriz(params);
/*  cout<<params.n<<endl;
  cout<<params.mMasUno<<endl;
  cout<<params.valorIsoterma<<endl;
  cout<<params.nInst<<endl;
  cout<<params.radioInterno<<endl;
  cout<<params.radioExterno<<endl;
  cout<<params.deltaAngulo<<endl;
  cout<<params.deltaRadio<<endl;*/
  //cout<<A<<endl;
  Resolvedor resolvedor = Resolvedor(A);
  
  Matriz sol;
  //matriz de soluciones (cada solución es una columna)
  if(metodo == "0"){
    sol = resolvedor.resolverUsandoGauss(&params.bs[i]);
  } else {
    sol = resolvedor.resolverUsandoLU(&params.bs[i]);
  }
  return sol;
}

Matriz obtenerIsoterma(Matriz sol, Parametros params, int i, string metodo){
  Parametros copyParms = params;
  if (DISCRETIZAR_NUEVAMENTE) {
    int j = 1;
    while (hayQueDiscretizarNuevamente(sol, copyParms)) {
      cout << j <<endl;
      copyParms = nuevosParametros(sol, copyParms);
      sol = resolver(copyParms, i, metodo);
      j++;
    }
  }
  return calcularIsoterma(params, sol);
}

int main(int argc, char **argv) {
  //Comienzo de mediciones para informe
  clock_t inicio_, final_;
  // desde cuando quiero medir
  inicio_=clock();

  //Lectura de parámetros y archivos de entrada y salida
  FileManager manager = FileManager(argv[1], argv[2]);
  Parametros params = manager.read();
  string metodo = argv[3];

  
  //Resolución con la discretización pedida por parámetro
  vector<Matriz> soluciones;
  vector<Matriz> isotermas;
  for (int i=0; i<params.nInst; i++){
    Matriz sol = resolver(params, i, metodo);
    soluciones.push_back(sol);
    isotermas.push_back(obtenerIsoterma(sol, params, i, metodo));
  }
  
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

  salida(soluciones, isotermas, manager);

  final_=clock();
  //printf("El Tiempo es: %f\n",(final_ - inicio_)/CLK_TCK);
}
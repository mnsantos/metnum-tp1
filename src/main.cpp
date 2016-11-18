#include <stdio.h>
#include <vector>
#include <iostream>
#include "Matriz.h"
#include "Resolvedor.h"
#include "Parametros.h"
#include "FileManager.h"
#include <fstream>
#include <algorithm> 

using namespace std;

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
      cout<<Tactual<<endl;
      cout<<Tsiguiente<<endl;
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
        valor =(rad*Tactual + (rad+1)*Tsiguiente)/(Tactual+Tsiguiente);
        radiosIsoterma.put(ang, 0, params.radioInterno + valor * params.deltaRadio);
        break;
      }
    }
  }
  return radiosIsoterma;
}

vector<double> hallarRadios(Matriz sol, Parametros params) {
  cout << "hallarRadios" << endl;
  vector<int> radiosMinimos;
  vector<int> radiosMaximos;
  for (int i=0; i < params.mMasUno-1; i++) {
    //cout<<"iteracion i"<<i<<endl;
    for (int j=0; j < params.n; j++) {
      double t = temp(i, j, sol, params);
      //cout<<t<<endl;
      //cout<<params.valorIsoterma<<endl;
      if (t < params.valorIsoterma) {
        //cout <<sol.elem(0,0) << endl;
        //cout<<"entre con valor i " << i << " y j " << j << " y temperatura " << t << endl;
        radiosMinimos.push_back(i-1);
        radiosMaximos.push_back(i);
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
  return false;
  cout << "hayQueDiscretizarNuevamente" << endl;
  vector<double> radios = hallarRadios(sol, params);
  int radioMinimo = radios[0];
  int radioMaximo = radios[1];
  double CONSTANTE_A_DEFINIR = 10;
  return (radioMaximo - radioMinimo > CONSTANTE_A_DEFINIR);
}

Parametros nuevosParametros(Matriz sol, Parametros params) {
  vector<double> radios = hallarRadios(sol, params);
  int radioMinimo = radios[0];
  int radioMaximo = radios[1];
  double nuevoRadioInterno = params.radioInterno + params.deltaRadio * radioMinimo;
  double nuevoRadioExterno = params.radioExterno + params.deltaRadio * radioMaximo;
  
  Parametros nuevosParametros;
  nuevosParametros.radioInterno = nuevoRadioInterno;
  nuevosParametros.radioExterno = nuevoRadioExterno;
  nuevosParametros.mMasUno = params.mMasUno;
  nuevosParametros.n = params.n;
  nuevosParametros.valorIsoterma = params.valorIsoterma;
  nuevosParametros.nInst = params.nInst;

  double cantidadIncognitas = params.n*params.mMasUno;

  Matriz b = Matriz(cantidadIncognitas,1);

  for (int j = 0; j < params.n; ++j) {
    b.put(j, 0, temp(radioMinimo, j, sol, params));
  }
  for (int j = params.n; j < cantidadIncognitas-(params.n); ++j) {
    b.put(j, 0, 0);
  }
  for (int j = cantidadIncognitas-(params.n); j < cantidadIncognitas; ++j) {
    b.put(j, 0, temp(radioMaximo, j, sol, params));
  }

  vector<Matriz> bs;
  bs.push_back(b);
  nuevosParametros.bs = bs;
  return nuevosParametros;
}

vector<Matriz> resolver(Parametros params, string metodo) {
  //matriz de soluciones (cada solución es una columna)
  vector<Matriz> xs;
  //arma la matriz de coeficientes a resolver.
  Matriz A = Matriz(params);
  Resolvedor resolvedor = Resolvedor(A);
  //resuelve la cantidad de instancias que nos pasen
  for (int i = 0; i < params.nInst; ++i){
    //Solucion al sistema con el b
    Matriz sol;
    //matriz de soluciones (cada solución es una columna)
    if(metodo == "0"){
      sol = resolvedor.resolverUsandoGauss(&params.bs[i]);
    } else {
      sol = resolvedor.resolverUsandoLU(&params.bs[i]);
    }
    xs.push_back(sol);
  }
  return xs;
}

vector<Matriz> obtenerIsotermas(vector<Matriz> xs, Parametros params, string metodo){
  vector<Matriz> isotermas;
  for(int i=0; i<xs.size(); i++){
    Matriz sol = Matriz(xs[i]);
    cout << "llegue" <<endl;
    while (hayQueDiscretizarNuevamente(sol, params)) {
      params = nuevosParametros(sol, params);
      vector<Matriz> xss = resolver(params, metodo);
      sol = xss[0];
    }
    isotermas.push_back(calcularIsoterma(params, sol));
  }
  return isotermas;
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
  vector<Matriz> xs = resolver(params, metodo);
  //for (int i=0; i<params.mMasUno ; i++){
  //  cout << xs[0].elem(0,i) << endl;
  //}
  //cout << xs[0].filas() <<endl;
  //cout << xs[0].columnas() <<endl;
  //for (int i=0; i<50; i++){
  //  cout << xs[0].elem(i,0) << endl;
  //}
  //Obtención de isoterma mediante refinamiento de las soluciones y promedios
  vector<Matriz> isotermas = obtenerIsotermas(xs, params, metodo);

  salida(xs, isotermas, manager);

  final_=clock();
  //printf("El Tiempo es: %f\n",(final_ - inicio_)/CLK_TCK);
}
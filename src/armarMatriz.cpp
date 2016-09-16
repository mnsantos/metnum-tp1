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
      double valor;
      // habria que chequear si no es ~= en vez de =
      if(Tactual == params.valorIsoterma) {
        valor = Tactual;
        radiosIsoterma.put(rad, 0, params.radioInterno + valor * params.deltaRadio);
        break;
      }
      else if(Tsiguiente < params.valorIsoterma){
        //este calculo esta bien?
        valor =(rad*Tactual + (rad+1)*Tsiguiente)/(Tactual+Tsiguiente);
        radiosIsoterma.put(rad, 0, params.radioInterno + valor * params.deltaRadio);
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
    for (int j=0; j < params.n; j++) {
      double t = temp(i, j, sol, params);
      if (t < params.valorIsoterma) {
        radiosMinimos.push_back(i-1);
        radiosMaximos.push_back(i);
      }
    }
  }
  int radioMinimo = * min_element(radiosMinimos.begin(), radiosMinimos.end());
  int radioMaximo = * max_element(radiosMaximos.begin(), radiosMaximos.end());
  vector<double> res;
  res.push_back(radioMinimo);
  res.push_back(radioMaximo);
  cout << res << endl;
  return res;
}

bool hayQueDiscretizarNuevamente(Matriz sol, Parametros params) {
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

Matriz resolver(Parametros params, string metodo, Matriz b){
  //arma la matriz de coeficientes a resolver.
  Matriz A = armado(params);
  Resolvedor resolvedor = Resolvedor(A);
  //matriz de soluciones (cada solución es una columna)
  if(metodo == "0"){
    return resolvedor.resolverUsandoGauss(&b);
  } else {
    return resolvedor.resolverUsandoLU(&b);
  }
}

int main(int argc, char **argv) {
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
    xs.push_back(sol);
    while (hayQueDiscretizarNuevamente(sol, params)) {
      Parametros params2 = nuevosParametros(sol, params);
      cout << params2.bs[0] << endl;
      resolver(params2, argv[3], params.bs[0]);
    }
    isotermas.push_back(calcularIsoterma(params, sol));
  }

  salida(xs, isotermas, manager);

  final_=clock();
  //printf("El Tiempo es: %f\n",(final_ - inicio_)/CLK_TCK);
  }

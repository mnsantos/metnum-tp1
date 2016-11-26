import os
import sys
import matplotlib.pyplot as plt

def readResults(filename):
	with open(filename) as f:
		params = f.readline().split('_')
		tamanio = int(params[2]) * int(params[3])
		instancias = int(params[-1])
		metodo = f.readline()[:-1]
		peligrosidad = f.readline().split()
		subdiscretizaciones = f.readline().split()
		tiemposPorInstancia = f.readline().split()
		tiempoTotal = float(f.readline()[:-1])
		listaSubDiscretizaciones = []
		sumSubDiscretizaciones = 0
		for i in xrange(0,len(subdiscretizaciones)):
			sumSubDiscretizaciones += float(subdiscretizaciones[i])
			listaSubDiscretizaciones.append((i, sumSubDiscretizaciones))
		print instancias
		print tiemposPorInstancia
		print tamanio
		print tiempoTotal
	return metodo, listaSubDiscretizaciones, (tamanio, tiempoTotal)

def printTiempos(tiemposLU, tiemposGauss, filename):
	X = [ x for (x,y) in tiemposLU ]
	Y = [ y for (x,y) in tiemposLU ]
	plt.plot(X, Y, label='Factorizacion LU')
	X = [ x for (x,y) in tiemposGauss ]
	Y = [ y for (x,y) in tiemposGauss ]
	plt.plot(X, Y, label='Factorizacion por Gauss')
	#plt.title(title)
	plt.xlabel('Cantidad de puntos discretizados')
	plt.ylabel('Tiempo en segundos')
	plt.grid(True)
	plt.legend(loc='upper center', shadow=True)
	plt.savefig("experimentos/"+filename+".png")
	plt.close()
	#plt.show()

def obtenerTiemposCirculo():
	tiempoTotalLU = []
	tiempoTotalGauss = []
	for i in xrange(1,6):
		for method in xrange(0,2):
			filename = 'experimentos/test'+str(i)+'_'+str(method)+'.out'
			metodo, listaSubDiscretizaciones, [tamanio, tiempoTotal] = readResults(filename)
			if metodo == 'LU':
				tiempoTotalLU.append(((tamanio, tiempoTotal)))
			else:
				tiempoTotalGauss.append(((tamanio, tiempoTotal)))
	printTiempos(tiempoTotalLU, tiempoTotalGauss, "circulo")
			
def obtenerTiemposEstrella():
	tiempoTotalLU = []
	tiempoTotalGauss = []
	for i in xrange(6,11):
		for method in xrange(0,2):
			filename = 'experimentos/test'+str(i)+'_'+str(method)+'.out'
			metodo, listaSubDiscretizaciones, [tamanio, tiempoTotal] = readResults(filename)
			if metodo == 'LU':
				tiempoTotalLU.append(((tamanio, tiempoTotal)))
			else:
				tiempoTotalGauss.append(((tamanio, tiempoTotal)))
	printTiempos(tiempoTotalLU, tiempoTotalGauss, "estrella")

def obtenerTiemposParedes():
	tiempoTotalLU = []
	tiempoTotalGauss = []
	for i in xrange(11,16):
		for method in xrange(0,2):
			filename = 'experimentos/test'+str(i)+'_'+str(method)+'.out'
			metodo, listaSubDiscretizaciones, [tamanio, tiempoTotal] = readResults(filename)
			if metodo == 'LU':
				tiempoTotalLU.append(((tamanio, tiempoTotal)))
			else:
				tiempoTotalGauss.append(((tamanio, tiempoTotal)))
	printTiempos(tiempoTotalLU, tiempoTotalGauss, "paredes")

def obtenerTiemposOvalo():
	tiempoTotalLU = []
	tiempoTotalGauss = []
	for i in xrange(16,21):
		for method in xrange(0,2):
			filename = 'experimentos/test'+str(i)+'_'+str(method)+'.out'
			metodo, listaSubDiscretizaciones, [tamanio, tiempoTotal] = readResults(filename)
			if metodo == 'LU':
				tiempoTotalLU.append(((tamanio, tiempoTotal)))
			else:
				tiempoTotalGauss.append(((tamanio, tiempoTotal)))
	printTiempos(tiempoTotalLU, tiempoTotalGauss, "ovalo")

def obtenerTiemposMultiB():
	tiempoTotalLU = []
	tiempoTotalGauss = []
	for i in xrange(21,22):
		for method in xrange(0,2):
			filename = 'experimentos/test'+str(i)+'_'+str(method)+'.out'
			metodo, listaSubDiscretizaciones, [tamanio, tiempoTotal] = readResults(filename)
			if metodo == 'LU':
				tiempoTotalLU = listaSubDiscretizaciones
			else:
				tiempoTotalGauss = listaSubDiscretizaciones
	printTiempos(tiempoTotalLU, tiempoTotalGauss, "multiB")

def progress(count, total, suffix=''):
    bar_len = 60
    filled_len = int(round(bar_len * count / float(total)))
    percents = round(100.0 * count / float(total), 1)
    bar = '=' * filled_len + '-' * (bar_len - filled_len)
    sys.stdout.write('[%s] %s%s ...%s\r' % (bar, percents, '%', suffix))
    sys.stdout.flush()

def ejecutarTests():
	os.system('python metnum.py build')
	j = 0
	for i in xrange(1,24):
		for method in xrange(0,2):
			fin = 'tests/test'+str(i)+'.in'
			fout = 'results/test'+str(i)+'_'+str(method)+'.out'
			print './tp '+fin+' '+fout+' '+str(method)
			os.system('./tp '+fin+' '+fout+' '+str(method))
			j += 1
			progress(j, 40, suffix='')

def obtenerTiempos():
	obtenerTiemposCirculo()
	obtenerTiemposEstrella()
	obtenerTiemposParedes()
	obtenerTiemposOvalo()

def main():
	#ejecutarTests()
	obtenerTiempos()

if __name__ == '__main__':
	main()
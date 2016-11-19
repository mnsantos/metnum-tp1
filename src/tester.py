import os
import sys

def progress(count, total, suffix=''):
    bar_len = 60
    filled_len = int(round(bar_len * count / float(total)))
    percents = round(100.0 * count / float(total), 1)
    bar = '=' * filled_len + '-' * (bar_len - filled_len)
    sys.stdout.write('[%s] %s%s ...%s\r' % (bar, percents, '%', suffix))
    sys.stdout.flush()

def main():
	os.system('python metnum.py build')
	j = 0
	for i in xrange(1,21):
		for method in xrange(0,2):
			fin = 'tests/test'+str(i)+'.in'
			fout = 'results/test'+str(i)+'_'+str(method)+'.out'
			print './tp '+fin+' '+fout+' '+str(method)
			os.system('./tp '+fin+' '+fout+' '+str(method))
			j += 1
			progress(j, 40, suffix='')

if __name__ == '__main__':
	main()
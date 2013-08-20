all: os.func.demo os.bonus.func.demo

os.bonus.func.demo: os.o os.bonus.func.tester utility.o
		gcc -o os.bonus.func.demo os.o os.bonus.func.tester utility.o -g

os.func.demo: os.o os.func.tester utility.o
		gcc -o os.func.demo os.o os.func.tester utility.o -g

os.perf.demo: os.o os.perf.tester
		gcc -o os.perf.demo os.o os.perf.tester -g

os.o: os.c
		gcc -c -o os.o os.c -g

os.bonus.func.tester: os.bonus.func.tester.c
		gcc -c -o  os.bonus.func.tester os.bonus.func.tester.c -g

os.func.tester: os.func.tester.c
		gcc -c -o os.func.tester os.func.tester.c -g

utility.o: utility.c
		gcc -c -o utility.o utility.c -g

os.perf.tester: os.perf.tester.c
		gcc -c -o os.perf.tester os.perf.tester.c -g

clean:
		rm -f *.demo *.tester *.o


main:
	gcc -pthread -o main main.c

NUM = "default"
run:
	./main $(NUM)

omp:
	gcc -pthread -fopenmp -o OpenMPtest OpenMPtest.c

clean:
	rm -rf main OpenMPtest
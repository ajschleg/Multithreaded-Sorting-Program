main:
	gcc -pthread -o main main.c

NUM = "default"
run:
	./main $(NUM)

omp:
	gcc -pthread -o OpenMPtest OpenMPtest.c

clean:
	rm -rf main
main:
	gcc -pthread -o main main.c

NUM = "default"
run:
	./main $(NUM)

clean:
	rm -rf main
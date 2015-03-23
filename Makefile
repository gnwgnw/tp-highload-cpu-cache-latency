all:
	gcc main.c -O3 -std=gnu99 -o cpu_latency

clean:
	rm cpu_latency
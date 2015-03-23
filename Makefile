all:
	gcc main.c -std=gnu99 -o cpu_latency

clean:
	rm cpu_latency
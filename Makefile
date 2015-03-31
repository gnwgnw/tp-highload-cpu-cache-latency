all: release

release:
	gcc main.c -std=gnu99 -O3 -o cpu_latency

debug:
	gcc main.c -std=gnu99 -Wall -g -o cpu_latency

clean:
	rm cpu_latency
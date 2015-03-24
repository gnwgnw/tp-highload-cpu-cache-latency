all: release

release:
	gcc main.c -std=gnu99 -o cpu_latency

debug:
	gcc main.c -std=gnu99 -g -o cpu_latency

clean:
	rm cpu_latency
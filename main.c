#include <stdint.h>
#include <x86intrin.h>
#include <getopt.h>
#include <stdio.h>
#include <time.h>
#include <inttypes.h>

#define CYCLES_COUNT 100

struct node {
	struct node* next;
	//struct node* PAD[NODE_SIZE - 1];
};

void build_list_conseq(void* head, uint32_t size, uint8_t node_size) {
	struct node* current = head;

	for (uint32_t i = 1; i < size; ++i) {
		current->next = head + i * node_size;
		current = current->next;
	}
	current->next = head;
}

void build_list_random(void* head, uint32_t size, uint8_t node_size) {
	if (size == 1) {
		((struct node*) head)->next = head;
		return;
	}

	struct node* current = head;
	srand((unsigned int) time(NULL));

	for (uint32_t i = 1; i < size; ++i) {
		uint32_t next = rand() % size;
		void* temp = head + next * node_size;
		while (temp == current || ((struct node*) temp)->next) {
			next = ++next % size;
			temp = head + next * node_size;
		}
		current->next = temp;
		current = current->next;
	}
	current->next = head;
}

uint64_t measure(struct node* head, uint32_t size) {
	struct node* current = head;
	uint64_t avg = 0;
	int cycles = CYCLES_COUNT;

	for (int i = 0; i < cycles; ++i) {
		uint64_t start = __rdtsc();

		for (int j = 0; j < size; ++j) {
			current = (*current).next;
		}

		uint64_t stop = __rdtsc();
		avg += stop - start;
	}
	avg /= cycles;

	return avg / size;
}

void arg_parser(int argc, char** argv, uint8_t* list_size_pow, uint8_t* node_size) {
	int c = 0;
	while ((c = getopt(argc, argv, "n:l:")) != -1)
		switch (c) {
			case 'l':
				(*list_size_pow) = (uint8_t) atoi(optarg);
		        break;
			case 'n':
				(*node_size) = (uint8_t) atoi(optarg);
		        break;
			default:
				exit(1);
		}
}

int main(int argc, char** argv) {
	uint8_t list_size_pow = 20; //2^list_size_pow; < 32
	uint8_t node_size = 1; //in words

	arg_parser(argc, argv, &list_size_pow, &node_size);

	node_size *= sizeof(struct node);
	uint32_t list_size = (uint32_t) (1 << list_size_pow);

	printf("node = %d, list = %d, total = %uK\n", node_size, list_size, (node_size * list_size) >> 10);

	void* list = calloc(list_size, node_size);
	if (list) {
		build_list_random(list, list_size, node_size);
		printf("Build done\n");
		printf("avg ticks = %" PRIu64 "\n", measure(list, list_size));
		free(list);
	}
	return 0;
}
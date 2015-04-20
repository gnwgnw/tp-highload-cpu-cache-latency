#include <stdint.h>
#include <x86intrin.h>
#include <getopt.h>
#include <stdio.h>
#include <time.h>
#include <inttypes.h>

#define ACCESS_CYCLES_COUNT 100000
#define AVG_CYCLES_COUNT 100
#define WORKSET_SIZE 1 << 24 //16Mb

struct node {
	struct node* next;
};

void build_list_conseq(void* head, uint32_t size, uint16_t node_size) {
	struct node* current = head;

	for (uint32_t i = 1; i < size; ++i) {
		current->next = head + i * node_size;
		current = current->next;
	}
	current->next = head;
}

void build_list_random(void* head, uint32_t size, uint16_t node_size) {
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

uint64_t measure(struct node* head) {
	struct node* current = head;
	uint64_t avg = 0;

	for (int i = 0; i < AVG_CYCLES_COUNT; ++i) {
		uint64_t start = __rdtsc();
		for (int j = 0; j < ACCESS_CYCLES_COUNT; ++j) {
			current = current->next;
		}
		uint64_t stop = __rdtsc();
		avg += (stop - start) / AVG_CYCLES_COUNT;
	}

	avg /= ACCESS_CYCLES_COUNT;

	return avg;
}

void arg_parser(int argc, char** argv, uint32_t * list_size, uint16_t* node_size, uint8_t* is_random) {
	int c = 0;
	while ((c = getopt(argc, argv, "rn:l:")) != -1)
		switch (c) {
			case 'l':
				(*list_size) = (uint32_t) atoi(optarg);
		        break;
			case 'n':
				(*node_size) = (uint16_t) atoi(optarg);
		        break;
			case 'r':
				(*is_random) = 1;
		        break;
			default:
				exit(1);
		}
}

int main(int argc, char** argv) {
	uint32_t list_size = 0;
	uint16_t node_size = 0;
	uint8_t is_random = 0;

	arg_parser(argc, argv, &list_size, &node_size, &is_random);

	if (node_size < sizeof(struct node)) {
		node_size = sizeof(struct node);
	}

	if (list_size * node_size > WORKSET_SIZE) {
		return 0;
	}

	void* list = calloc(list_size, node_size);
	if (list) {
		if (is_random)
			build_list_random(list, list_size, node_size);
		else
			build_list_conseq(list, list_size, node_size);

		if (is_random)
			printf("Random: ");
		else
			printf("Conseq: ");

		printf("node = %d, list = %d, total = %u, ", node_size, list_size, ((uint32_t) node_size * list_size));
		printf("avg ticks = %" PRIu64 "\n", measure(list));
		free(list);
	}
	return 0;
}
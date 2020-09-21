#include "../include/main.h"

int main(){
	Heap *heap = create_heap();
	Node *node1 = create_node(1);
	Node *node2 = create_node(2);
	Node *node3 = create_node(3);
	Node *node4 = create_node(4);
	heap_insert_tail(heap, node1);
	heap_insert_tail(heap, node2);
	heap_insert_head(heap, node3);
	heap_insert_head(heap, node4);
	heap_remove_head(heap);
	heap_remove_head(heap);
	heap_remove_head(heap);
	heap_remove_head(heap);
	heap_insert_tail(heap, node1);
	heap_insert_tail(heap, node2);
	heap_insert_head(heap, node3);
	heap_insert_head(heap, node4);
	destroy_heap(heap);
}

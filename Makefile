test_collect_bytes_frequency:
	g++ -o collect_bytes_frequency tests/collect_bytes_frequency.c huffman/*.cpp
	./collect_bytes_frequency
	rm collect_bytes_frequency

test_huffman_tree:
	g++ -o huffman_tree tests/huffman_tree.c huffman/*.cpp
	./huffman_tree
	rm huffman_tree

test_min_heap:
	g++ -o min_heap tests/min_heap.c huffman/*.cpp
	./min_heap
	rm min_heap

test: test_collect_bytes_frequency test_huffman_tree test_min_heap
	echo "All tests successfully finished!"

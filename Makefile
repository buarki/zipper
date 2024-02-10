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

test_build_min_heap_from_bytes_frequency:
	g++ -o build_min_heap_from_bytes_frequency tests/build_min_heap_from_bytes_frequency.c huffman/*.cpp
	./build_min_heap_from_bytes_frequency
	rm build_min_heap_from_bytes_frequency

test_build_huffman_tree_from_min_heap:
	g++ -o build_huffman_tree_from_min_heap tests/build_huffman_tree_from_min_heap.c huffman/*.cpp
	./build_huffman_tree_from_min_heap
	rm build_huffman_tree_from_min_heap

test_build_symbol_codes_from_tree:
	g++ -o build_symbol_codes_from_tree tests/build_symbol_codes_from_tree.c huffman/*.cpp
	./build_symbol_codes_from_tree
	rm build_symbol_codes_from_tree

test_header:
	g++ -o header tests/header.c huffman/*.cpp
	./header
	rm header

test: test_collect_bytes_frequency test_huffman_tree test_min_heap test_build_min_heap_from_bytes_frequency test_build_huffman_tree_from_min_heap test_build_symbol_codes_from_tree test_header
	echo "All tests successfully finished!"

HUFFMAN_SOURCES := $(shell find huffman -name '*.c' ! -name 'main.c')

wasm:
	emcc huffman/*.c \
			 -s WASM=1 \
			 -s EXPORTED_FUNCTIONS="[_free, _malloc]" \
			 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap', 'UTF8ToString', 'writeArrayToMemory', 'getValue']" \
			 -s INITIAL_MEMORY=200mb \
			 -o zipper.js || exit 1
	mv zipper.js public/zipper.js
	mv zipper.wasm public/zipper.wasm

emserve:
	emrun --port 8080 public/

test_collect_bytes_frequency:
	g++ -o collect_bytes_frequency tests/collect_bytes_frequency.c $(HUFFMAN_SOURCES)
	./collect_bytes_frequency
	rm collect_bytes_frequency

test_huffman_tree:
	g++ -o huffman_tree tests/huffman_tree.c $(HUFFMAN_SOURCES)
	./huffman_tree
	rm huffman_tree

test_min_heap:
	g++ -o min_heap tests/min_heap.c $(HUFFMAN_SOURCES)
	./min_heap
	rm min_heap

test_build_min_heap_from_bytes_frequency:
	g++ -o build_min_heap_from_bytes_frequency tests/build_min_heap_from_bytes_frequency.c $(HUFFMAN_SOURCES)
	./build_min_heap_from_bytes_frequency
	rm build_min_heap_from_bytes_frequency

test_build_huffman_tree_from_min_heap:
	g++ -o build_huffman_tree_from_min_heap tests/build_huffman_tree_from_min_heap.c $(HUFFMAN_SOURCES)
	./build_huffman_tree_from_min_heap
	rm build_huffman_tree_from_min_heap

test_build_symbol_codes_from_tree:
	g++ -o build_symbol_codes_from_tree tests/build_symbol_codes_from_tree.c $(HUFFMAN_SOURCES)
	./build_symbol_codes_from_tree
	rm build_symbol_codes_from_tree

test_header:
	g++ -o header tests/header.c $(HUFFMAN_SOURCES)
	./header
	rm header

test_compute_required_bytes_for_encoded_symbols:
	g++ -o compute_required_bytes_for_encoded_symbols tests/compute_required_bytes_for_encoded_symbols.c $(HUFFMAN_SOURCES)
	./compute_required_bytes_for_encoded_symbols
	rm compute_required_bytes_for_encoded_symbols

test_collect_symbol_codes_to_export:
	g++ -o collect_symbol_codes_to_export tests/collect_symbol_codes_to_export.c $(HUFFMAN_SOURCES)
	./collect_symbol_codes_to_export
	rm collect_symbol_codes_to_export

test_compress:
	g++ -o compress tests/compress.c $(HUFFMAN_SOURCES)
	./compress
	rm compress

test_compute_compressed_file_symbols_length:
	g++ -o compute_compressed_file_symbols_length tests/compute_compressed_file_symbols_length.c $(HUFFMAN_SOURCES)
	./compute_compressed_file_symbols_length
	rm compute_compressed_file_symbols_length

test_compute_padding_bits_for_compressed_codes:
	g++ -o compute_padding_bits_for_compressed_codes tests/compute_padding_bits_for_compressed_codes.c $(HUFFMAN_SOURCES)
	./compute_padding_bits_for_compressed_codes
	rm compute_padding_bits_for_compressed_codes

test_build_huffman_tree_from_compressed_file:
	g++ -o build_huffman_tree_from_compressed_file tests/build_huffman_tree_from_compressed_file.c $(HUFFMAN_SOURCES)
	./build_huffman_tree_from_compressed_file
	rm build_huffman_tree_from_compressed_file

test_build_decompressed_file:
	g++ -o build_decompressed_file tests/build_decompressed_file.c $(HUFFMAN_SOURCES)
	./build_decompressed_file
	rm build_decompressed_file

test_decompress:
	g++ -o decompress tests/decompress.c $(HUFFMAN_SOURCES)
	./decompress
	rm decompress

test_compute_bytes_required_for_decompressed_file:
	g++ -o compute_bytes_required_for_decompressed_file tests/compute_bytes_required_for_decompressed_file.c $(HUFFMAN_SOURCES)
	./compute_bytes_required_for_decompressed_file
	rm compute_bytes_required_for_decompressed_file

test_integration:
	g++ -o integration tests/integration.c $(HUFFMAN_SOURCES)
	./integration
	rm integration

test: test_collect_bytes_frequency test_huffman_tree test_min_heap test_build_min_heap_from_bytes_frequency test_build_huffman_tree_from_min_heap test_build_symbol_codes_from_tree test_header test_compute_required_bytes_for_encoded_symbols test_collect_symbol_codes_to_export test_compress test_compute_compressed_file_symbols_length test_compute_padding_bits_for_compressed_codes test_decompress test_compute_bytes_required_for_decompressed_file test_integration test_build_huffman_tree_from_compressed_file test_build_decompressed_file
	echo "All tests successfully finished!"

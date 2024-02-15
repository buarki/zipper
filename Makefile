wasm:
	em++ huffman/*.cpp \
			 -s WASM=1 \
			 -s EXPORTED_FUNCTIONS="[_receiveContent, _c_compress, _c_decompress, _collectDecompressedContent, _free, _malloc]" \
			 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap', 'UTF8ToString', 'writeArrayToMemory', 'getValue']" \
			 -o zipper.js || exit 1
	mv zipper.js public/zipper.js
	mv zipper.wasm public/zipper.wasm

emserve:
	emrun --port 8080 public/

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

test_compute_required_bytes_for_encoded_symbols:
	g++ -o compute_required_bytes_for_encoded_symbols tests/compute_required_bytes_for_encoded_symbols.c huffman/*.cpp
	./compute_required_bytes_for_encoded_symbols
	rm compute_required_bytes_for_encoded_symbols

test_collect_symbol_codes_to_export:
	g++ -o collect_symbol_codes_to_export tests/collect_symbol_codes_to_export.c huffman/*.cpp
	./collect_symbol_codes_to_export
	rm collect_symbol_codes_to_export

test_compress:
	g++ -o compress tests/compress.c huffman/*.cpp
	./compress
	rm compress

test_compute_compressed_file_symbols_length:
	g++ -o compute_compressed_file_symbols_length tests/compute_compressed_file_symbols_length.c huffman/*.cpp
	./compute_compressed_file_symbols_length
	rm compute_compressed_file_symbols_length

test_compute_padding_bits_for_compressed_codes:
	g++ -o compute_padding_bits_for_compressed_codes tests/compute_padding_bits_for_compressed_codes.c huffman/*.cpp
	./compute_padding_bits_for_compressed_codes
	rm compute_padding_bits_for_compressed_codes

test_build_huffman_tree_from_compressed_file:
	g++ -o build_huffman_tree_from_compressed_file tests/build_huffman_tree_from_compressed_file.c huffman/*.cpp
	./build_huffman_tree_from_compressed_file
	rm build_huffman_tree_from_compressed_file

test_build_decompressed_file:
	g++ -o build_decompressed_file tests/build_decompressed_file.c huffman/*.cpp
	./build_decompressed_file
	rm build_decompressed_file

test_decompress:
	g++ -o decompress tests/decompress.c huffman/*.cpp
	./decompress
	rm decompress

test_compute_bytes_required_for_decompressed_file:
	g++ -o compute_bytes_required_for_decompressed_file tests/compute_bytes_required_for_decompressed_file.c huffman/*.cpp
	./compute_bytes_required_for_decompressed_file
	rm compute_bytes_required_for_decompressed_file

test_integration:
	g++ -o integration tests/integration.c huffman/*.cpp
	./integration
	rm integration

test: test_collect_bytes_frequency test_huffman_tree test_min_heap test_build_min_heap_from_bytes_frequency test_build_huffman_tree_from_min_heap test_build_symbol_codes_from_tree test_header test_compute_required_bytes_for_encoded_symbols test_collect_symbol_codes_to_export test_compress test_compute_compressed_file_symbols_length test_compute_padding_bits_for_compressed_codes test_decompress test_compute_bytes_required_for_decompressed_file test_integration test_build_huffman_tree_from_compressed_file test_build_decompressed_file
	echo "All tests successfully finished!"

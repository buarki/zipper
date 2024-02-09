test_collect_bytes_frequency:
	g++ -o collect_bytes_frequency tests/collect_bytes_frequency.c huffman/*.cpp
	./collect_bytes_frequency
	rm collect_bytes_frequency 

test: test_collect_bytes_frequency
	echo "All tests successfully finished!"
	
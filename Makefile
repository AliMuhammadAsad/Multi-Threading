.PHONY: build runs cleans ignore

build:
	gcc -Wall -o fps file_processor_singlethreaded.c
	@echo "To run the program, use: make runs <input_file>"

	gcc -Wall -o fpm file_processor_multithreaded.c -lpthread
	@echo "To run the program, use: make runm <input_file> <num_threads>"
	
runs:
	./fps $(filter-out $@,$(MAKECMDGOALS))

runm:
	./fpm $(filter-out $@,$(MAKECMDGOALS))

clean:
	rm -f fps fpm

ignore:
	@:

%: ignore
	@:

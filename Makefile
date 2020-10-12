COMPILE_FLAGS = -Wall
SRC = syscall_tracer.c
CC = gcc
EXEC_NAME = syscall_tracer

run_tests: interactive_call_test fatal_error_call_test compile
	./$(EXEC_NAME) -i ./interactive_call_test
	./$(EXEC_NAME) -a ./fatal_error_call_test
	./$(EXEC_NAME) -i ls -a


compile : $(SRC)
		$(CC) $(COMPILE_FLAGS) $(SRC) -o $(EXEC_NAME)

interactive_call_test: interactive_call_test.c
	$(CC) $(COMPILE_FLAGS) interactive_call_test.c -o interactive_call_test

fatal_error_call_test: fatal_error_call_test.c
	$(CC) $(COMPILE_FLAGS) fatal_error_call_test.c -o fatal_error_call_test

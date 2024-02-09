CFLAGS = -Wall -Wextra -Werror

ASAN = -fsanitize=address,undefined -g

LIB_DIR = lib
EXECUTABLE = bin

OPTIM = -O0

LDFLAGS = 

all: compile

compile: 
	cc -c *.c $(CFLAGS) $(LDFLAGS) $(OPTIM)
	cc -c $(LIB_DIR)/*.c $(CFLAGS) $(LDFLAGS) $(OPTIM)
	cc *.o $(CFLAGS) -o $(EXECUTABLE)_clean $(LDFLAGS) $(OPTIM)
	rm *.o

debug: 
	cc -c -g *.c $(LDFLAGS) $(OPTIM)
	cc -c -g $(LIB_DIR)/*.c $(LDFLAGS) $(OPTIM)
	cc *.o -o $(EXECUTABLE)_debug $(LDFLAGS) $(OPTIM)
	rm *.o

asan: 
	clang -c *.c $(CFLAGS) $(ASAN) $(OPTIM)
	clang -c $(LIB_DIR)/*.c $(CFLAGS) $(ASAN) $(OPTIM)
	clang *.o $(CFLAGS) $(ASAN) -o $(EXECUTABLE)_asan $(LDFLAGS) $(OPTIM)
	rm *.o

static: 
	clang --analyze -Xanalyzer -analyzer-output=html *.c $(LIB_DIR)/*.c

formating:
	clang-format *.c -i -style=file

clean:
	rm -rf $(EXECUTABLE)* *plist

help:
	@echo "Available targets:"
	@echo "  all      : Build the executable with default settings"
	@echo "  compile  : Build the executable"
	@echo "  debug    : Build the executable with debugging information"
	@echo "  asan     : Build the executable with AddressSanitizer"
	@echo "  static   : Run static analysis and generate HTML reports"
	@echo "  clean    : Remove generated files"
	@echo "  formating: Format source code using clang-format"
	@echo "  help     : Display this help message"
	

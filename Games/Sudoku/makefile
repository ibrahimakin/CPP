ifeq ($(OS), Windows_NT)
	target = SudokuWindows.exe
else
	target = SudokuLinux.exe
endif

$(target): main.cpp
	g++ main.cpp -o $(target)
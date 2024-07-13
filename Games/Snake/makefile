ifeq ($(OS), Windows_NT)
	target = SnakeWindows.exe
else
	target = SnakeLinux.exe
endif

$(target): main.cpp
	g++ main.cpp -o $(target)
#-ltinfo -lcurses
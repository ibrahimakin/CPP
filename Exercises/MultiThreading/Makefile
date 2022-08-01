all: thread_mutex.exe thread_creation.exe

thread_mutex.exe: thread_mutex.cpp
	g++ thread_mutex.cpp -o thread_mutex.exe -lpthread

thread_creation.exe: thread_creation.cpp
	g++ thread_creation.cpp -o thread_creation.exe -lpthread

clean:
	rm -f thread_creation.exe thread_mutex.exe
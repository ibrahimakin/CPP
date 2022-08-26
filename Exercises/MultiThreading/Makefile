all: thread_race.exe thread_active.exe thread_mutex_pimple.exe thread_mutex.exe thread_totaliser.exe thread_creation.exe

thread_race.exe: thread_race.cpp
	g++ thread_race.cpp -o thread_race.exe -lpthread

thread_active.exe: thread_active.cpp
	g++ thread_active.cpp -o thread_active.exe -lpthread

thread_mutex_pimple.exe: thread_mutex_pimple.cpp
	g++ thread_mutex_pimple.cpp -o thread_mutex_pimple.exe -lpthread

thread_mutex.exe: thread_mutex.cpp
	g++ thread_mutex.cpp -o thread_mutex.exe -lpthread

thread_totaliser.exe: thread_totaliser.cpp
	g++ thread_totaliser.cpp -o thread_totaliser.exe -lpthread

thread_creation.exe: thread_creation.cpp
	g++ thread_creation.cpp -o thread_creation.exe -lpthread

clean:
	rm -f thread_race.exe thread_active.exe thread_mutex_pimple.exe thread_mutex.exe thread_totaliser.exe thread_creation.exe
#include <iostream>
#include <pthread.h>

using namespace std;

typedef struct
{
    int *start_ptr;
    int start;
    int end;
} thread_creation;

void *thread_started(void *)
{
    cout << "Thread Started" << endl;
    pthread_exit((void *)9);
    return NULL;
}

void *thread_doubler(void *user_data)
{
    cout << "Thread Doubler Started" << endl;

    if (user_data != nullptr)
    {
        int* pvalue = reinterpret_cast<int *>(user_data);
        (*pvalue) *= 2;
    }
    return NULL;
}

void *four_thread_doubler(void *user_data)
{
    cout << "Four Thread Doubler Started" << endl;

    if (user_data != nullptr)
    {
        thread_creation* pvalue = reinterpret_cast<thread_creation*>(user_data);
        for (size_t i = pvalue->start; i < pvalue->end; i++)
        {
            pvalue->start_ptr[i] *= 2;
        }
    }
    return NULL;
}

void CreateThread()
{
    cout << "TEST - Create Thread" << endl;

    cout << "About to start thread" << endl;

    pthread_t thread;
    int ret = pthread_create(&thread, NULL, &thread_started, NULL);

    if (ret != 0)
    {
        cout << "Error: pthread_create() failed" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Thread started, waiting for finish" << endl;
    pthread_join(thread, 0);
    cout << "Thread finished, deleting " << ret << endl;
    pthread_cancel(thread);
    // pthread_exit(NULL);
}

void CreateThreadDoubler()
{
    cout << "TEST - Create Thread Doubler" << endl;

    int* value = new int(6);
    cout << "About to start doubler thread with value " << *value << endl;

    pthread_t thread;
    int ret = pthread_create(&thread, NULL, &thread_doubler, value);
    if (ret != 0)
    {
        cout << "Error: pthread_create() failed" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Thread started, waiting for finish" << endl;
    pthread_join(thread, 0);
    cout << "Thread doubler finished, value now " << *value << endl;
    pthread_cancel(thread);
    delete value;
    // pthread_exit(NULL);
}

void CreateManyThreads()
{
    cout << "TEST - Create Many Threads" << endl;

    const size_t Number_Of_Threads = 10;

    int* values = new int[Number_Of_Threads]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    pthread_t threads[Number_Of_Threads];

    for (unsigned int thread_index = 0; thread_index < Number_Of_Threads; ++thread_index)
    {
        cout << "About to start doubler thread with value " << values[thread_index] << endl;
        string thread_name = "thread doubler " + thread_index;

        int ret = pthread_create(&threads[thread_index], NULL, thread_doubler, &values[thread_index]);
        if (ret != 0)
        {
            cout << "Error: pthread_create() failed thread " << thread_index << endl;
            exit(EXIT_FAILURE);
        }
    }

    cout << "All Threads started, waiting for finish" << endl;

    for (unsigned int thread_index = 0; thread_index < Number_Of_Threads; ++thread_index)
    {
        if (threads[thread_index])
        {
            pthread_join(threads[thread_index], NULL);
            cout << "Thread " << thread_index << " finished, value now " << values[thread_index] << endl;
            pthread_cancel(threads[thread_index]);
            // pthread_exit(NULL);
        }
    }
    delete[] values;
    cout << "All Threads finished" << endl;
}

void CreateFourThreads()
{
    cout << "TEST - Create Four Threads" << endl;

    const size_t Number_Of_Threads = 4;
    const size_t Array_Size = 18;

    int over = Array_Size % Number_Of_Threads;
    int size = Array_Size / Number_Of_Threads;

    int* values = new int[Array_Size]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};

    thread_creation* m_threads = new thread_creation[Number_Of_Threads];

    int i, end = 0;
    for (i = 0; i < Number_Of_Threads; i++)
    {
        m_threads[i].start_ptr = values;
        m_threads[i].start = end;
        m_threads[i].end = end + size + (i < over);
        end = m_threads[i].end;
    }

    pthread_t threads[Number_Of_Threads];

    for (i = 0; i < Number_Of_Threads; i++)
    {
        cout << "About to start doubler thread" << endl;
        cout << m_threads[i].start << " " << m_threads[i].end << endl;
        string thread_name = "four thread doubler " + i;

        int ret = pthread_create(&threads[i], NULL, four_thread_doubler, &m_threads[i]);
        if (ret != 0)
        {
            cout << "Error: pthread_create() failed thread " << i << endl;
            exit(EXIT_FAILURE);
        }
    }

    cout << "Four Threads started, waiting for finish" << endl;

    for (i = 0; i < Number_Of_Threads; i++)
    {
        if (threads[i])
        {
            pthread_join(threads[i], NULL);

            for (size_t j = m_threads[i].start; j < m_threads[i].end; j++)
            {
                cout << "Thread " << i << " finished, value now " << values[j] << endl;
            }

            pthread_cancel(threads[i]);
            // pthread_exit(NULL);
        }
    }
    delete[] values;
    cout << "All Threads finished" << endl;
}

int main()
{
    CreateThread();
    cout << endl;
    CreateThreadDoubler();
    cout << endl;
    CreateManyThreads();
    cout << endl;
    CreateFourThreads();
    return 0;
}
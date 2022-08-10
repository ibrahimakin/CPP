#include <vector>
#include <iostream>
#include <pthread.h>

using namespace std;

class Totaliser
{
public:
    typedef vector<int> ValueList;

    Totaliser();

    int isInitialised() const;
    // add to the total
    void add(int value);
    // subtract from the total
    void reduce(int value);
    // return the current total
    int total() const;
    // add a collection of values
    void add(const ValueList& values);

private:
    void addLocked(int value);
    void reduceLocked(int value);
    int totalLocked() const;
    void addLocked(const ValueList& values);

    // disable copying
    Totaliser(const Totaliser &rhs);
    Totaliser &operator=(const Totaliser &rhs);

    int m_running_total;
    mutable pthread_mutex_t m_mutex;
    int m_init_ok;
};

Totaliser::Totaliser()
    : m_running_total(0)
{
    m_init_ok = pthread_mutex_init(&m_mutex, 0);
}

/* If you were to implement copying, I think it would be done like this
Totaliser::Totaliser(const Totaliser& tot)
    : m_running_total(tot.total())
{
    m_init_ok = pthread_mutex_init(&m_mutex, 0);
}
Totaliser&
Totaliser::operator=(const Totaliser& rhs)
{
    pthread_mutex_lock(&m_mutex);
    m_running_total = rhs.total();
    return *this;
}*/

int
Totaliser::isInitialised() const
{
    return m_init_ok;
}

void
Totaliser::add(int value)
{
    pthread_mutex_lock(&m_mutex);
    addLocked(value);
    pthread_mutex_unlock(&m_mutex);
}

void
Totaliser::addLocked(int value)
{
    m_running_total += value;
}

void
Totaliser::reduce(int value)
{
    pthread_mutex_lock(&m_mutex);
    reduceLocked(value);
    pthread_mutex_unlock(&m_mutex);
}

void
Totaliser::reduceLocked(int value)
{
    m_running_total -= value;
}

int
Totaliser::total() const
{
    pthread_mutex_lock(&m_mutex);
    int ret = totalLocked();
    pthread_mutex_unlock(&m_mutex);
    return ret;
}

int
Totaliser::totalLocked() const
{
    return m_running_total;
}

void
Totaliser::add(const ValueList& values)
{
    pthread_mutex_lock(&m_mutex);
    addLocked(values);
    pthread_mutex_unlock(&m_mutex);
}

void
Totaliser::addLocked(const ValueList& values)
{
    for (ValueList::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        addLocked(*it);
    }
}

int main()
{
    cout << "TEST - Totaliser Exercise" << endl;

    Totaliser tot;
    if (tot.isInitialised() != 0)
    {
        cout << "Totaliser init failed" << endl;
        return 0;
    }

    tot.add(3);
    tot.reduce(2);
    const int total = tot.total();
    cout << "Total " << tot.total() << endl;    // 1

    Totaliser::ValueList values;
    values.push_back(2);
    values.push_back(3);
    tot.add(values);
    cout << "Total " << tot.total() << endl;    // 6

    vector<int> numbers;
    numbers.push_back(4);
    numbers.push_back(5);
    tot.add(numbers);
    cout << "Total " << tot.total() << endl;    // 15

    return 0;
}
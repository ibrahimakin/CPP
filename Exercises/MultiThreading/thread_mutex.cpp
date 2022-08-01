#include <vector>
#include <iostream>
#include <pthread.h>

using namespace std;

class BagOfThings
{
public:
    BagOfThings();
    ~BagOfThings();

    int isInitialised() const;

    // put a number into the bag
    void putIn(unsigned int value);
    // see if a number is in the bag
    bool lookFor(unsigned int value) const;
    // remove a number from the bag
    void deleteFrom(unsigned int value);
    // put value in the bag if not there already
    void ensureContains(unsigned int value);

private:
    // Methods called by the public interface, after the mutex is locked
    void putInLocked(unsigned int value);
    bool lookForLocked(unsigned int value) const;
    void deleteFromLocked(unsigned int value);
    void ensureContainsLocked(unsigned int value);

    // disable copying
    BagOfThings(const BagOfThings& rhs);
    BagOfThings& operator=(const BagOfThings& rhs);

    typedef vector<unsigned int> BagType;

    BagType m_bag;
    mutable pthread_mutex_t m_mutex;    // mutable as need to lock it in const methods
    int m_init_ok;
};

BagOfThings::BagOfThings()
{
    m_init_ok = pthread_mutex_init(&m_mutex, 0);
}

BagOfThings::~BagOfThings()
{
}

int
BagOfThings::isInitialised() const
{
    return m_init_ok;
}

void
BagOfThings::putIn(unsigned int value)
{
    pthread_mutex_lock(&m_mutex);
    putInLocked(value);
    pthread_mutex_unlock(&m_mutex);
}

void
BagOfThings::putInLocked(unsigned int value)
{
    m_bag.push_back(value);
}

bool
BagOfThings::lookFor(unsigned int value) const
{
    pthread_mutex_lock(&m_mutex);
    bool ret = lookForLocked(value);
    pthread_mutex_unlock(&m_mutex);
    return ret;
}

bool
BagOfThings::lookForLocked(unsigned int value) const
{
    for (BagType::const_iterator it = m_bag.begin(); it != m_bag.end(); ++it)
    {
        if (*it == value)
        {
            return true;    // found one
        }
    }
    return false;           // not found
}

void
BagOfThings::deleteFrom(unsigned int value)
{
    pthread_mutex_lock(&m_mutex);
    deleteFromLocked(value);
    pthread_mutex_unlock(&m_mutex);
}

void
BagOfThings::deleteFromLocked(unsigned int value)
{
    for (BagType::iterator it = m_bag.begin(); it != m_bag.end(); ++it)
    {
        if (*it == value)
        {
            m_bag.erase(it);
            return;
        }
    }
}

void
BagOfThings::ensureContains(unsigned int value)
{
    pthread_mutex_lock(&m_mutex);
    ensureContainsLocked(value);
    pthread_mutex_unlock(&m_mutex);
}

void
BagOfThings::ensureContainsLocked(unsigned int value)
{
    if (!lookForLocked(value))
    {
        putInLocked(value);
    }
}

void *thread_starter1(void* user_data)
{
    cout << "Thread Starter 1" << endl;
    BagOfThings& bag = *reinterpret_cast<BagOfThings *>(user_data);
    bag.putIn(6);
    const bool found_it = bag.lookFor(6);
    bag.ensureContains(4);

    cout << "Found the 6 " << found_it << endl;
    return NULL;
}

void *thread_starter2(void* user_data)
{
    cout << "Thread Starter 2" << endl;
    BagOfThings& bag = *reinterpret_cast<BagOfThings *>(user_data);
    bag.putIn(7);
    bag.deleteFrom(6);
    bag.ensureContains(4);
    return NULL;
}

int main()
{
    cout << "TEST - Basic Mutex Example" << endl;

    BagOfThings bag;
    if (bag.isInitialised() != 0)
    {
        cout << "Bag init failed" << endl;
        return 0;
    }

    pthread_t* thread1;
    int ret = pthread_create(thread1, 0, thread_starter1, (void*)&bag);
    if (ret != 0)
    {
        cout << "Thread1 startup failed" << endl;
        exit(EXIT_FAILURE);
    }
    pthread_t* thread2;
    ret = pthread_create(thread2, 0, thread_starter2, (void*)&bag);
    if (ret != 0)
    {
        cout << "Thread2 startup failed" << endl;
        exit(EXIT_FAILURE);
    }

    pthread_join(*thread1, 0);
    pthread_join(*thread2, 0);

    cout << "Look for 4 -> " << bag.lookFor(4) << endl;

    pthread_cancel(*thread1);
    pthread_cancel(*thread2);

    return 0;
}
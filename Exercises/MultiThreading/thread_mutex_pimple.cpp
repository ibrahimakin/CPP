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
    // put value in bag if not there already
    void ensureContains(unsigned int value);

private:
    class LockedBag
    {
    public:
        // put a number into the bag
        void putIn(unsigned int value);
        // see if a number is in the bag
        bool lookFor(unsigned int value) const;
        // remove a number from the bag
        void deleteFrom(unsigned int value);
        // put value in bag if not there already
        void ensureContains(unsigned int value);

    private:
        typedef vector<unsigned int> BagType;

        BagType m_bag;
    };

    // disable copying
    BagOfThings(const BagOfThings& rhs);
    BagOfThings& operator=(const BagOfThings& rhs);

    LockedBag m_locked_bag;

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
    m_locked_bag.putIn(value);
    pthread_mutex_unlock(&m_mutex);
}

void
BagOfThings::LockedBag::putIn(unsigned int value)
{
    m_bag.push_back(value);
}

bool
BagOfThings::lookFor(unsigned int value) const
{
    pthread_mutex_lock(&m_mutex);
    bool ret = m_locked_bag.lookFor(value);
    pthread_mutex_unlock(&m_mutex);
    return ret;
}

bool
BagOfThings::LockedBag::lookFor(unsigned int value) const
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
    m_locked_bag.deleteFrom(value);
    pthread_mutex_unlock(&m_mutex);
}

void
BagOfThings::LockedBag::deleteFrom(unsigned int value)
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
    m_locked_bag.ensureContains(value);
    pthread_mutex_unlock(&m_mutex);
}

void
BagOfThings::LockedBag::ensureContains(unsigned int value)
{
    if (!lookFor(value))
    {
        putIn(value);
    }
}

void *thread_starter1(void* user_data)
{
    cout << "Thread Starter 1" << endl;
    BagOfThings& bag = *reinterpret_cast<BagOfThings *>(user_data);
    bag.putIn(6);
    const bool found_it = bag.lookFor(6);
    bag.ensureContains(4);

    cout << "Found the 6 -> " << found_it << endl;
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

int BasicMutex()
{
    cout << endl << "TEST - Pimpled Mutex Example" << endl;

    BagOfThings bag;
    if (bag.isInitialised() != 0)
    {
        cout << "Bag init failed" << endl;
        return 0;
    }

    pthread_t thread1;
    int ret = pthread_create(&thread1, 0, thread_starter1, &bag);
    if (ret != 0)
    {
        cout << "Thread1 startup failed" << endl;
        return 0;
    }
    pthread_t thread2;
    ret = pthread_create(&thread2, 0, thread_starter2, &bag);
    if (ret != 0)
    {
        cout << "Thread2 startup failed" << endl;
        return 0;
    }
    
    pthread_join(thread1, 0);
    pthread_join(thread2, 0);

    cout << "Look for 4 -> " << bag.lookFor(4) << endl;

    pthread_cancel(thread1);
    pthread_cancel(thread2);

    return 0;
}

class PersonalInfo
{
public:
    PersonalInfo(const string &firstname, const string &lastname, uint8_t day, uint8_t month, uint16_t year);

    // fullname is just firstname + lastname
    string fullname() const;
    // identifier is initials + birth year mod 100
    string identifier() const;

    void setName(const string &firstname, const string &lastname);
    void setBirthday(uint8_t day, uint8_t month, uint16_t year);

private:
    string m_firstname;
    string m_lastname;
    // Date of birth
    uint8_t m_day;
    uint8_t m_month;
    uint16_t m_year;
};

PersonalInfo::PersonalInfo(const string &firstname, const string &lastname, uint8_t day, uint8_t month, uint16_t year)
    : m_firstname(firstname), m_lastname(lastname), m_day(day), m_month(month), m_year(year)
{
}

string
PersonalInfo::fullname() const
{
    return m_firstname + " " + m_lastname;
}

string
PersonalInfo::identifier() const
{
    return m_firstname.substr(0, 1) + m_lastname.substr(0, 1) + to_string(m_year % 100);
}

void
PersonalInfo::setName(const string &firstname, const string &lastname)
{
    m_firstname = firstname;
    m_lastname = lastname;
}

void
PersonalInfo::setBirthday(uint8_t day, uint8_t month, uint16_t year)
{
    m_day = day;
    m_month = month;
    m_year = year;
}

void PersonalInfoTest()
{
    PersonalInfo somebody("Jessica", "Jones", 1, 11, 2001);
    cout << endl;
    cout << "Fullname:   " << somebody.fullname() << endl;
    cout << "Identifier: " << somebody.identifier() << endl << endl;

    somebody.setName("Zebediah", "Killgrave");
    somebody.setBirthday(10, 10, 1964);

    cout << "Fullname:   " << somebody.fullname() << endl;
    cout << "Identifier: " << somebody.identifier() << endl << endl;
}

int main()
{
    BasicMutex();
    PersonalInfoTest();
    return 0;
}
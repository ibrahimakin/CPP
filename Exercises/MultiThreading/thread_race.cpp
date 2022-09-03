#include <vector>
#include <memory>
#include <iostream>
#include <pthread.h>

using namespace std;

namespace ThreadRaceStack
{
class Stack
{
public:
    Stack();
    ~Stack() {};

    void pop();
    int pop(bool& did_pop);
    int top() const;
    void push(int);
    void print() const;
    int size() const;
    bool empty() const;

    int top_and_pop();

    int isInitialised() const;

private:
    typedef vector<int> DataContainer;

    DataContainer m_data;
    mutable pthread_mutex_t m_mutex;
    int m_init_ok;
};

Stack::Stack()
{
    m_init_ok = pthread_mutex_init(&m_mutex, 0);
}

int
Stack::isInitialised() const
{
    return m_init_ok;
}

int
Stack::top() const
{
    pthread_mutex_lock(&m_mutex);
    int ret = m_data.back();
    pthread_mutex_unlock(&m_mutex);
    return ret;
}

int
Stack::size() const
{
    pthread_mutex_lock(&m_mutex);
    int ret = m_data.size();
    pthread_mutex_unlock(&m_mutex);
    return ret;
}

bool
Stack::empty() const
{
    pthread_mutex_lock(&m_mutex);
    bool ret = m_data.empty();
    pthread_mutex_unlock(&m_mutex);
    return ret;
}

void
Stack::pop()
{
    pthread_mutex_lock(&m_mutex);
    m_data.erase(m_data.end() - 1);
    pthread_mutex_unlock(&m_mutex);
}

void
Stack::push(int n)
{
    pthread_mutex_lock(&m_mutex);
    m_data.push_back(n);
    pthread_mutex_unlock(&m_mutex);
}

void
Stack::print() const
{
    cout << "initial stack :" << endl;
    pthread_mutex_lock(&m_mutex);
    for (DataContainer::const_iterator it = m_data.begin(); it != m_data.end(); ++it)
    {
        cout << *it << endl;
    }
    cout << endl;
    pthread_mutex_unlock(&m_mutex);
}

int
Stack::pop(bool& did_pop)
{
    pthread_mutex_lock(&m_mutex);
    if (m_data.empty())
    {
        did_pop = false;
        pthread_mutex_unlock(&m_mutex);
        return -1;
    }
    did_pop = true;
    int val = m_data.back();
    m_data.erase(m_data.end() - 1);
    pthread_mutex_unlock(&m_mutex);
    return val;
}

int
Stack::top_and_pop()
{
    pthread_mutex_lock(&m_mutex);
    int val = m_data.back();
    m_data.erase(m_data.end() - 1);
    pthread_mutex_unlock(&m_mutex);
    return val;
}

struct ThreadArguments
{
    ThreadArguments(Stack& st, const char* message, Stack& st_out);
    ThreadArguments(const ThreadArguments&) = delete;
    ThreadArguments& operator=(const ThreadArguments&) = delete;

    Stack& m_stack;
    Stack& m_stack_out;
    const char* m_message;
};

ThreadArguments::ThreadArguments(Stack& st, const char* message, Stack& st_out)
    : m_stack(st),
      m_message(message),
      m_stack_out(st_out)
{
}

void*
thread_function(void* user_arg)
{
    ThreadArguments& ta = *(reinterpret_cast<ThreadArguments*>(user_arg));

    Stack& st = ta.m_stack;

    bool did_pop;
    int val = st.pop(did_pop);
    while (did_pop)
    {
        ta.m_stack_out.push(val);
        val = st.pop(did_pop);
        cout << ta.m_message << " : " << val << endl;
    }
    return 0;
}

void
thread_function_top_and_pop(void* user_arg)
{
    ThreadArguments& ta = *(reinterpret_cast<ThreadArguments*>(user_arg));

    Stack& st = ta.m_stack;

    int val = st.top_and_pop();
    cout << ta.m_message << " : " << val << endl;
}

void
thread_race_test_stack()
{
    Stack st;

    size_t test_size = 100;
    for (size_t i = 0; i < test_size; i++)
    {
        st.push(i);
    }
    
    st.print();
    if (st.isInitialised() != 0)
    {
        cout << "Stack init failed" << endl;
        return;
    }
    
    Stack st_out;
    if (st_out.isInitialised() != 0)
    {
        cout << "Stack init failed" << endl;
        return;
    }
    
    while (st.size() > 0)
    {
        pthread_t thread1, thread2;

        ThreadArguments ta1(st, "thread1", st_out);
        ThreadArguments ta2(st, "thread2", st_out);

        if (st.size() > 0)
        {
            int ret = pthread_create(&thread1, 0, thread_function, &ta1);
            if (ret != 0)
            {
                cout << "Thread 1 startup failed" << endl;
                return;
            }
        }

        if (st.size() > 0)
        {
            int ret = pthread_create(&thread2, 0, thread_function, &ta2);
            if (ret != 0)
            {
                cout << "Thread 2 startup failed" << endl;
                return;
            }
        }

        pthread_join(thread1, 0);
        pthread_join(thread2, 0);
        pthread_cancel(thread1);
        pthread_cancel(thread2);
        cout << "st_out.size() == test_size   "
             << st_out.size() << " == " << test_size
             << endl;
    }
}
} // namespace ThreadRaceStack

namespace ThreadRaceArray
{
class ServiceInfo
{
public:
    ServiceInfo(const string& name);
    ServiceInfo(const ServiceInfo& si);
    void setName(const string& name);

    const string& name() const;

private:
    ServiceInfo& operator=(const ServiceInfo&) = delete;

    string m_name;
    mutable pthread_mutex_t m_mutex;
};

ServiceInfo::ServiceInfo(const string& name)
    : m_name(name)
{
    int init_ok = pthread_mutex_init(&m_mutex, 0);
    if (init_ok != 0)
    {
        cout << "Failed to init ServiceInfo" << endl;
        return;
    }
}

ServiceInfo::ServiceInfo(const ServiceInfo& si)
    : m_name(si.name())
{
    int init_ok = pthread_mutex_init(&m_mutex, 0);
    if (init_ok != 0)
    {
        cout << "Failed to init ServiceInfo" << endl;
        return;
    }
}

void
ServiceInfo::setName(const string& name)
{
    pthread_mutex_lock(&m_mutex);
    m_name = name;
    pthread_mutex_unlock(&m_mutex);
}

const string&
ServiceInfo::name() const
{
    pthread_mutex_lock(&m_mutex);
    pthread_mutex_unlock(&m_mutex);
    return m_name;
}

class Services
{
public:
    typedef shared_ptr<ServiceInfo> ServiceHandle;

    Services();

    int isInitialised() const;

    // add a new service info to the list
    void add(const ServiceInfo& si);

    // find a service info with a matching name and return index of it in list
    // returns -1 if no match found
    int find(const string& name) const;

    // update the info at this index with a new name
    void update(int index, const string& name);

    // erase the service info at this index
    void erase(int index);

    // replace the service info at this index with a new one
    void replace(int index, const ServiceInfo& si);

    // Print out a list of the services
    void print() const;

private:
    typedef vector<ServiceHandle> ServiceList;

    ServiceList m_services;
    mutable pthread_mutex_t m_mutex;
    int m_init_ok;
};

Services::Services()
{
    m_init_ok = pthread_mutex_init(&m_mutex, 0);
}

int
Services::isInitialised() const
{
    return m_init_ok;
}

void
Services::add(const ServiceInfo& si)
{
    pthread_mutex_lock(&m_mutex);
    m_services.push_back(make_shared<ServiceInfo>(si));
    pthread_mutex_unlock(&m_mutex);
}

int
Services::find(const string& name) const
{
    pthread_mutex_lock(&m_mutex);
    for (unsigned int service_index = 0; service_index < m_services.size(); ++service_index)
    {
        if (m_services[service_index]->name() == name)
        {
            pthread_mutex_unlock(&m_mutex);
            return service_index;
        }
    }
    pthread_mutex_unlock(&m_mutex);
    return -1;      // no match, return error code
}

void
Services::update(int index, const string& name)
{
    pthread_mutex_lock(&m_mutex);
    m_services[index]->setName(name);
    pthread_mutex_unlock(&m_mutex);
}

void
Services::erase(int index)
{
    pthread_mutex_lock(&m_mutex);
    m_services.erase(m_services.begin() + index);
    pthread_mutex_unlock(&m_mutex);
}

void
Services::replace(int index, const ServiceInfo& si)
{
    pthread_mutex_lock(&m_mutex);
    m_services[index] = make_shared<ServiceInfo>(si);
    pthread_mutex_unlock(&m_mutex);
}

void
Services::print() const
{
    pthread_mutex_lock(&m_mutex);
    cout << "Service list:" << endl;
    for (unsigned int service_index = 0; service_index < m_services.size(); ++service_index)
    {
        cout << "Service " << service_index << ": " << m_services[service_index]->name() << endl;
    }
    cout << endl;
    pthread_mutex_unlock(&m_mutex);
}

void
make_service_list(Services& services)
{
    services.add(ServiceInfo("Vestel TV Gold"));
    services.add(ServiceInfo("Samsung blows"));
    services.add(ServiceInfo("Endless Marvel Movies"));
    services.add(ServiceInfo("Lannister News Channel"));
}

struct ThreadArgs
{
    ThreadArgs(Services& services, const char* up_target, const char* up_new, const char* erase);

    ThreadArgs(const ThreadArgs&) = delete;
    ThreadArgs& operator=(const ThreadArgs&) = delete;

    Services& m_services;
    const char* m_update_target;
    const char* m_update_new_name;
    const char* m_erase;
};

ThreadArgs::ThreadArgs(Services& services, const char* up_target, const char* up_new, const char* erase)
    : m_services(services),
      m_update_target(up_target),
      m_update_new_name(up_new),
      m_erase(erase)
{
}

void*
thread_function(void* user_arg)
{
    ThreadArgs& args = *(reinterpret_cast<ThreadArgs*>(user_arg));
    Services& services = args.m_services;
    int info_index = services.find(args.m_update_target);
    if (info_index != -1)
    {
        services.update(info_index, args.m_update_new_name);
    }
    info_index = services.find(args.m_erase);
    if (info_index != -1)
    {
        services.erase(info_index);
    }
    return 0;
}

void
thread_race_test_array()
{
    Services services;
    if (services.isInitialised() != 0)
    {
        cout << "Failed to init services" << endl;
        return;
    }
    make_service_list(services);

    cout << "Initial state:" << endl;
    services.print();

    ThreadArgs ta1(services, "Endless Marvel Movies", "Endless X-Men Movies", "Vestel TV Gold");
    ThreadArgs ta2(services, "Lannister News Channel", "Stark News Channel", "Samsung blows");

    pthread_t thread1, thread2;
    int ret = pthread_create(&thread1, 0, thread_function, &ta1);
    if (ret != 0)
    {
        cout << "Thread 1 startup failed" << endl;
        return;
    }
    
    ret = pthread_create(&thread2, 0, thread_function, &ta2);
    if (ret != 0)
    {
        cout << "Thread 2 startup failed" << endl;
        return;
    }

    pthread_join(thread1, 0);
    pthread_join(thread2, 0);
    pthread_cancel(thread1);
    pthread_cancel(thread2);

    cout << "After processing:" << endl;
    services.print();
}
} // namespace ThreadRaceArray

namespace ThreadRaceSharedPtr
{
using ThreadRaceArray::ServiceInfo;

class Services
{
public:
    typedef shared_ptr<ServiceInfo> ServiceHandle;

    Services();

    int isInitialised() const;

    // add a new service info to the list
    void add(const ServiceInfo& si);

    // find a service info with a matching name and return index of it in list
    // returns and empty handle if no match found
    ServiceHandle find(const string& name) const;

    // erase the service info at this index
    void erase(const ServiceHandle& sh);

    // Print out a list of the services
    void print() const;

private:
    typedef vector<ServiceHandle> ServiceList;

    ServiceList m_services;
    mutable pthread_mutex_t m_mutex;
    int m_init_ok;
};

Services::Services()
{
    m_init_ok = pthread_mutex_init(&m_mutex, 0);
}

int
Services::isInitialised() const
{
    return m_init_ok;
}

void
Services::add(const ServiceInfo& si)
{
    pthread_mutex_lock(&m_mutex);
    m_services.push_back(make_shared<ServiceInfo>(si));
    pthread_mutex_unlock(&m_mutex);
}

Services::ServiceHandle
Services::find(const string& name) const
{
    pthread_mutex_lock(&m_mutex);
    for (ServiceList::const_iterator it = m_services.begin(); it != m_services.end(); ++it)
    {
        if ((*it)->name() == name)
        {
            pthread_mutex_unlock(&m_mutex);
            return *it;
        }
    }
    pthread_mutex_unlock(&m_mutex);
    return ServiceHandle();     // no match, return error code
}

void
Services::erase(const ServiceHandle& sh)
{
    pthread_mutex_lock(&m_mutex);
    for (ServiceList::iterator it = m_services.begin(); it < m_services.end(); ++it)
    {
        if (*it == sh)
        {
            m_services.erase(it);
            pthread_mutex_unlock(&m_mutex);
            return;
        }
    }
    pthread_mutex_unlock(&m_mutex);
}

void
Services::print() const
{
    pthread_mutex_lock(&m_mutex);
    cout << "Service list:" << endl;
    for (unsigned int service_index = 0; service_index < m_services.size(); ++service_index)
    {
        cout << "Service " << service_index << ": " << m_services[service_index]->name() << endl;
    }
    cout << endl;
    pthread_mutex_unlock(&m_mutex);
}

void
make_service_list(Services& services)
{
    services.add(ServiceInfo("Vestel TV Gold"));
    services.add(ServiceInfo("Samsung blows"));
    services.add(ServiceInfo("Endless Marvel Movies"));
    services.add(ServiceInfo("Lannister News Channel"));
}

struct ThreadArgs
{
    ThreadArgs(Services& services, const char* up_target, const char* up_new, const char* erase);

    ThreadArgs(const ThreadArgs&) = delete;
    ThreadArgs& operator=(const ThreadArgs&) = delete;
    Services& m_servives;
    const char* m_update_target;
    const char* m_update_new_name;
    const char* m_erase;
};

ThreadArgs::ThreadArgs(Services& services, const char* up_target, const char* up_new, const char* erase)
    : m_servives(services),
      m_update_target(up_target),
      m_update_new_name(up_new),
      m_erase(erase)
{
}

void*
thread_function(void* user_arg)
{
    ThreadArgs& args = *(reinterpret_cast<ThreadArgs*>(user_arg));
    Services& services = args.m_servives;
    Services::ServiceHandle info_handle = services.find(args.m_update_target);
    if (info_handle.get() != 0)
    {
        info_handle->setName(args.m_update_new_name);
    }
    info_handle = services.find(args.m_erase);
    if (info_handle.get() != 0)
    {
        services.erase(info_handle);
    }
    return 0;
}

void
thread_race_test_shared_ptr()
{
    Services services;
    if (services.isInitialised() != 0)
    {
        cout << "Failed to init services" << endl;
        return;
    }
    make_service_list(services);

    cout << "Initial state:" << endl;
    services.print();

    ThreadArgs ta1(services, "Endless Marvel Movies", "Endless X-Men Movies", "Vestel TV Gold");
    ThreadArgs ta2(services, "Lannister News Channel", "Stark News Channel", "Samsung blows");

    pthread_t thread1, thread2;
    int ret = pthread_create(&thread1, 0, thread_function, &ta1);
    if (ret != 0)
    {
        cout << "Thread 1 startup failed" << endl;
        return;
    }

    ret = pthread_create(&thread2, 0, thread_function, &ta2);
    if (ret != 0)
    {
        cout << "Thread 2 startup failed" << endl;
        return;
    }

    pthread_join(thread1, 0);
    pthread_join(thread2, 0);
    pthread_cancel(thread1);
    pthread_cancel(thread2);

    cout << "After processing:" << endl;
    services.print();
}
} // namespace ThreadRaceSharedPtr

int main()
{
    cout << "Thread Race Stack" << endl;
    ThreadRaceStack::thread_race_test_stack();
    cout << endl << "Thread Race Array" << endl;
    ThreadRaceArray::thread_race_test_array();
    cout << endl << "Thread Race SharedPtr" << endl;
    ThreadRaceSharedPtr::thread_race_test_shared_ptr();
    return 0;
}
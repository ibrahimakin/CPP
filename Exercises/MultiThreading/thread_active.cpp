#include <memory>
#include <vector>
#include <iostream>
#include <unistd.h>

using namespace std;

class AnyCommand
{
public:
    virtual ~AnyCommand() {}
    virtual void execute() = 0;
};

class CommandThread
{
public:
    CommandThread();
    ~CommandThread();

    bool initialise();
    bool enqueueCommand(AnyCommand *command_to_enqueue);

private:
    // Copy and assign prohibited.
    CommandThread(CommandThread const &);
    CommandThread& operator=(CommandThread const &);
};

CommandThread::CommandThread() {}

CommandThread::~CommandThread() {}

bool
CommandThread::initialise()
{
    return true;
}

bool
CommandThread::enqueueCommand(AnyCommand *command_to_enqueue)
{
    command_to_enqueue->execute();
    return true;
}

class ActiveBag
{
public:
    class AnyBagClient;
    typedef shared_ptr<AnyBagClient> ClientHandle;

    ActiveBag();
    ~ActiveBag();

    int isInitialised() const;

    // put a number into the bag
    void putIn(unsigned int value);
    // see if a number is in the bag
    void lookFor(ClientHandle client, unsigned int value) const;
    void deleteFrom(unsigned int value);
    void ensureContains(unsigned int value);
    void containsDuplicates(ClientHandle client) const;

private:
    // disable copying
    ActiveBag(const ActiveBag& rhs);
    ActiveBag& operator=(const ActiveBag& rhs);

    void putInImpl(unsigned int value);
    bool lookForImpl(unsigned int value) const;
    void deleteFromImpl(unsigned int value);
    void ensureContainsImpl(unsigned int value);
    bool containsDuplicatesImpl() const;

    // local classes to hold commands
    class PutInCommand;
    class LookForCommand;
    class DeleteFromCommand;
    class EnsureContainsCommand;
    class ContainsDuplicatesCommand;

    typedef vector<unsigned int> BagType;

    BagType m_bag;
    int m_init_ok;
    mutable CommandThread m_command_thread;
};

class ActiveBag::PutInCommand : public AnyCommand
{
public:
    PutInCommand(ActiveBag& bag, unsigned int value);
    virtual void execute();
private:
    PutInCommand(const PutInCommand&);
    PutInCommand& operator=(const PutInCommand&);

    unsigned int m_value;
    ActiveBag& m_active_bag;
};

ActiveBag::PutInCommand::PutInCommand(ActiveBag& bag, unsigned int value)
    : m_value(value)
    , m_active_bag(bag)
{
}

void
ActiveBag::PutInCommand::execute()
{
    m_active_bag.putInImpl(m_value);
}

void
ActiveBag::putInImpl(unsigned int value)
{
    m_bag.push_back(value);
}

void
ActiveBag::putIn(unsigned int value)
{
    m_command_thread.enqueueCommand(new PutInCommand(*this, value));
}

class ActiveBag::AnyBagClient
{
public:
    AnyBagClient() {}
    virtual ~AnyBagClient() {}

    // Callback interface to indicate if the value was in the bag
    virtual void lookedFor(bool found, unsigned int value) = 0;

    virtual void checkedForDuplicates(bool found) = 0;

protected:
    AnyBagClient(const AnyBagClient& rhs);
    AnyBagClient& operator=(const AnyBagClient& rhs);
};

class ActiveBag::LookForCommand : public AnyCommand
{
public:
    LookForCommand(const ActiveBag& bag, ActiveBag::ClientHandle& client, unsigned int value);
    virtual ~LookForCommand() {}

    virtual void execute();

private:
    LookForCommand(const LookForCommand&);
    LookForCommand& operator=(const LookForCommand&);

    unsigned int m_value;
    ActiveBag::ClientHandle m_bag_client;
    const ActiveBag& m_active_bag;
};

ActiveBag::LookForCommand::LookForCommand(const ActiveBag& bag, ActiveBag::ClientHandle& client, unsigned int value)
    : m_value(value)
    , m_bag_client(client)
    , m_active_bag(bag)
{
}

void
ActiveBag::LookForCommand::execute()
{
    const bool found = m_active_bag.lookForImpl(m_value);
    m_bag_client->lookedFor(found, m_value);
}

void
ActiveBag::lookFor(ActiveBag::ClientHandle client, unsigned int value) const
{
    m_command_thread.enqueueCommand(new LookForCommand(*this, client, value));
}

bool
ActiveBag::lookForImpl(unsigned int value) const
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

class ActiveBag::DeleteFromCommand : public AnyCommand
{
public:
    DeleteFromCommand(ActiveBag& bag, unsigned int value);

    virtual void execute();

private:
    DeleteFromCommand(const DeleteFromCommand&);
    DeleteFromCommand& operator=(const DeleteFromCommand&);

    unsigned int m_value;
    ActiveBag& m_active_bag;
};

ActiveBag::DeleteFromCommand::DeleteFromCommand(ActiveBag& bag, unsigned int value)
    : m_value(value)
    , m_active_bag(bag)
{
}

void
ActiveBag::DeleteFromCommand::execute()
{
    m_active_bag.deleteFromImpl(m_value);
}

void
ActiveBag::deleteFrom(unsigned int value)
{
    m_command_thread.enqueueCommand(new DeleteFromCommand(*this, value));
}

void
ActiveBag::deleteFromImpl(unsigned int value)
{
    for (BagType::const_iterator it = m_bag.begin(); it != m_bag.end(); ++it)
    {
        if (*it == value)
        {
            m_bag.erase(it);
            return;
        }
    }
}

class ActiveBag::EnsureContainsCommand : public AnyCommand
{
public:
    EnsureContainsCommand(ActiveBag& bag, unsigned int value);
    void execute() override;

private:
    EnsureContainsCommand(const EnsureContainsCommand&);
    EnsureContainsCommand& operator=(const EnsureContainsCommand&);

    unsigned int m_value;
    ActiveBag& m_active_bag;
};

ActiveBag::EnsureContainsCommand::EnsureContainsCommand(ActiveBag& bag, unsigned int value)
    : m_value(value)
    , m_active_bag(bag)
{
}

void
ActiveBag::EnsureContainsCommand::execute()
{
    m_active_bag.ensureContainsImpl(m_value);
}

void
ActiveBag::ensureContains(unsigned int value)
{
    m_command_thread.enqueueCommand(new EnsureContainsCommand(*this, value));
}

void
ActiveBag::ensureContainsImpl(unsigned int value)
{
    for (BagType::const_iterator it = m_bag.begin(); it != m_bag.end(); ++it)
    {
        if (*it == value)
        {
            return;
        }
    }
    m_bag.push_back(value);
}

class ActiveBag::ContainsDuplicatesCommand : public AnyCommand
{
public:
    ContainsDuplicatesCommand(const ActiveBag& bag, ActiveBag::ClientHandle& client);

    void execute() override;

private:
    ContainsDuplicatesCommand(const ContainsDuplicatesCommand&);
    ContainsDuplicatesCommand& operator=(const ContainsDuplicatesCommand&);

    ActiveBag::ClientHandle m_bag_client;
    const ActiveBag& m_active_bag;
};

ActiveBag::ContainsDuplicatesCommand::ContainsDuplicatesCommand(const ActiveBag& bag, ActiveBag::ClientHandle& client)
    : m_bag_client(client)
    , m_active_bag(bag)
{
}

void
ActiveBag::ContainsDuplicatesCommand::execute()
{
    m_bag_client->checkedForDuplicates(m_active_bag.containsDuplicatesImpl());
}

void
ActiveBag::containsDuplicates(ActiveBag::ClientHandle client) const
{
    m_command_thread.enqueueCommand(new ContainsDuplicatesCommand(*this, client));
}

bool
ActiveBag::containsDuplicatesImpl() const
{
    for (size_t i = 0; i < m_bag.size(); ++i)
    {
        for (size_t j = i + 1; j < m_bag.size(); ++j)
        {
            if (m_bag[i] == m_bag[j])
            {
                return true;
            }
        }
    }
    return false;
}

ActiveBag::ActiveBag()
{
    m_init_ok = m_command_thread.initialise();
}

ActiveBag::~ActiveBag()
{
}

int
ActiveBag::isInitialised() const
{
    return m_init_ok;
}

class BagMan : public ActiveBag::AnyBagClient,
               public enable_shared_from_this<BagMan>
{
public:
    BagMan();
    virtual ~BagMan() {}
    void bagOperations(ActiveBag& bag);
    int isInitialised() const;

    bool wasLastFound() const;
    bool didLastContainDuplicates() const;

private:
    virtual void lookedFor(bool found, unsigned int value);
    virtual void checkedForDuplicates(bool found);

    void lookedForImpl(bool found, unsigned int value);
    void checkedForDuplicatesImpl(bool found);

    class LookedForCommand;
    class CheckedForDuplicatesCommand;

    bool m_last_found = false;
    unsigned int m_last_found_value = 0;
    bool m_last_contained_duplicates = false;

    mutable CommandThread m_command_thread;
    int m_init_ok;
};

BagMan::BagMan()
{
    m_init_ok = m_command_thread.initialise();
}

int
BagMan::isInitialised() const
{
    return m_init_ok;
}

void
BagMan::bagOperations(ActiveBag& bag)
{
    // put some values in the bag
    bag.putIn(6);
    bag.putIn(7);
    // start the ActiveBag looking for values
    bag.lookFor(shared_from_this(), 7);
    bag.lookFor(shared_from_this(), 3);

    bag.containsDuplicates(shared_from_this());
    bag.putIn(7);
    bag.containsDuplicates(shared_from_this());
}

bool
BagMan::wasLastFound() const
{
    return m_last_found;
}

bool
BagMan::didLastContainDuplicates() const
{
    return m_last_contained_duplicates;
}

class BagMan::LookedForCommand : public AnyCommand
{
public:
    LookedForCommand(BagMan& bagman, bool found, unsigned int value);
    void execute() override;

private:
    LookedForCommand(const LookedForCommand&);
    LookedForCommand& operator=(const LookedForCommand&);

    bool m_found;
    BagMan& m_bagman;
    unsigned int m_value;
};

BagMan::LookedForCommand::LookedForCommand(BagMan& bagman, bool found, unsigned int value)
    : m_bagman(bagman)
    , m_found(found)
    , m_value(value)
{
}

void
BagMan::LookedForCommand::execute()
{
    m_bagman.lookedForImpl(m_found, m_value);
}

void
BagMan::lookedForImpl(bool found, unsigned int value)
{
    m_last_found = found;
    m_last_found_value = value;
}

void
BagMan::lookedFor(bool found, unsigned int value)
{
    cout << "Bag looked for found: " << found << ", value: " << value << endl;
    m_command_thread.enqueueCommand(new LookedForCommand(*this, found, value));
}

class BagMan::CheckedForDuplicatesCommand : public AnyCommand
{
public:
    CheckedForDuplicatesCommand(BagMan& bagman, bool found);
    void execute() override;

private:
    CheckedForDuplicatesCommand(const CheckedForDuplicatesCommand&);
    CheckedForDuplicatesCommand& operator=(const CheckedForDuplicatesCommand&);

    bool m_found;
    BagMan& m_bagman;
};

BagMan::CheckedForDuplicatesCommand::CheckedForDuplicatesCommand(BagMan& bagman, bool found)
    : m_bagman(bagman)
    , m_found(found)
{
}

void
BagMan::CheckedForDuplicatesCommand::execute()
{
    m_bagman.checkedForDuplicatesImpl(m_found);
}

void
BagMan::checkedForDuplicatesImpl(bool found)
{
    m_last_contained_duplicates = found;
}

void
BagMan::checkedForDuplicates(bool found)
{
    cout << "Bag checked for duplicates: " << found << endl;
    m_command_thread.enqueueCommand(new CheckedForDuplicatesCommand(*this, found));
}

void *thread_active_starter1(void* user_data)
{
    cout << "Thread Starter 1" << endl;
    ActiveBag& bag = *reinterpret_cast<ActiveBag*>(user_data);
    shared_ptr<BagMan> bagMan = make_shared<BagMan>();
    if (!bagMan->isInitialised())
    {
        cout << "Could not initialise BagMan" << endl;
        return 0;
    }
    bagMan->bagOperations(bag);
    sleep(2);
    return 0;
}

void *thread_active_starter2(void* user_data)
{
    cout << "Thread Starter 2" << endl;
    ActiveBag& bag = *reinterpret_cast<ActiveBag*>(user_data);
    bag.putIn(4);
    return 0;
}

void *thread_exercise_function1(void* user_data)
{
    cout << "Thread Function 1" << endl;
    ActiveBag* bag = reinterpret_cast<ActiveBag*>(user_data);

    bag->deleteFrom(3);
    bag->ensureContains(5);
    return 0;
}

void *thread_exercise_function2(void* user_data)
{
    cout << "Thread Function 2" << endl;
    ActiveBag* bag = reinterpret_cast<ActiveBag*>(user_data);

    bag->ensureContains(2);
    bag->deleteFrom(2);
    bag->deleteFrom(0);
    bag->ensureContains(10);
    return 0;
}

void Exercise()
{
    cout << "Exercise" << endl;

    ActiveBag bag;
    if (!bag.isInitialised())
    {
        cout << "Could not initialise ActiveBag" << endl;
        return;
    }

    bag.putIn(1);
    bag.putIn(2);
    bag.putIn(3);
    bag.putIn(4);

    pthread_t thread1;
    int ret = pthread_create(&thread1, 0, thread_exercise_function1, &bag);
    if (ret != 0)
    {
        cout << "Thread 1 startup failed" << endl;
        return;
    }

    pthread_t thread2;
    ret = pthread_create(&thread2, 0, thread_exercise_function2, &bag);
    if (ret != 0)
    {
        cout << "Thread 2 startup failed" << endl;
        return;
    }
    
    pthread_join(thread1, 0);
    pthread_join(thread2, 0);

    pthread_cancel(thread1);
    pthread_cancel(thread2);
}

void ActiveObject()
{
    cout << "Active Object" << endl;

    ActiveBag bag;
    if (!bag.isInitialised())
    {
        cout << "Could not initialise ActiveBag" << endl;
        return;
    }

    pthread_t thread1;
    int ret = pthread_create(&thread1, 0, thread_active_starter1, &bag);
    if (ret != 0)
    {
        cout << "Thread 1 startup failed" << endl;
        return;
    }

    pthread_t thread2;
    ret = pthread_create(&thread2, 0, thread_active_starter2, &bag);
    if (ret != 0)
    {
        cout << "Thread 2 startup failed" << endl;
        return;
    }

    pthread_join(thread1, 0);
    pthread_join(thread2, 0);

    pthread_cancel(thread1);
    pthread_cancel(thread2);
}

int main()
{
    cout << "Thread Active" << endl << endl;
    Exercise();
    cout << endl;
    ActiveObject();
    cout << endl;
    return 0;
}
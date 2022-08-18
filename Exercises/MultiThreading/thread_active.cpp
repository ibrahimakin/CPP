#include <memory>
#include <vector>

using namespace std;

class Command
{
public:
    virtual ~Command() {}
    virtual void execute() = 0;
};

class CommandThread
{
private:
    
public:
    CommandThread();
    ~CommandThread();

    bool initialise();
    bool enqueueCommand(Command* command); 
};

CommandThread::CommandThread()
{
}

CommandThread::~CommandThread()
{
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
    mutable CommandThread m_command_thread;
    int m_init_ok;
};

ActiveBag::ActiveBag()
{
}

ActiveBag::~ActiveBag()
{
}

int main()
{
    return 0;
}
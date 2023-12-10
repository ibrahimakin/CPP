// https://refactoring.guru/design-patterns/chain-of-responsibility
#include <iostream>
#include <string>
#include <vector>

using namespace std;

/*
The Handler declares a method for building the chain of handlers.
It also declares a method for executing a request.
*/
class Handler
{
public:
    virtual Handler *SetNext(Handler *handler) = 0;
    virtual string Handle(string request) = 0;
};

// The default chaining behavior can be implemented inside a base handler class.
class AbstractHandler : public Handler
{
private:
    /**
     * @var Handler
     */
    Handler *m_next_handler;

public:
    AbstractHandler() : m_next_handler(nullptr) {}
    Handler *SetNext(Handler *handler) override
    {
        this->m_next_handler = handler;
        // Returning a handler from here will let us link handlers in a convenient way like this:
        // $monkey->setNext($squirrel)->setNext($dog);
        return handler;
    }

    string Handle(string request) override
    {
        if (this->m_next_handler)
        {
            return this->m_next_handler->Handle(request);
        }
        return {};
    }
};

// All Concrete Handlers either handle a request or pass it to the next handler in the chain.
class MonkeyHandler : public AbstractHandler
{
public:
    string Handle(string request) override
    {
        if (request == "banana")
        {
            return "Monkey: I'll eat the " + request + ".\n";
        }
        else
        {
            return AbstractHandler::Handle(request);
        }
    }
};

class SquirrelHandler : public AbstractHandler
{
public:
    string Handle(string request) override
    {
        if (request == "nut")
        {
            return "Squirrel: I'll eat the " + request + ".\n";
        }
        else
        {
            return AbstractHandler::Handle(request);
        }
    }
};

class DogHandler : public AbstractHandler
{
public:
    string Handle(string request) override
    {
        if (request == "meat ball")
        {
            return "Dog: I'll eat the " + request + ".\n";
        }
        else
        {
            return AbstractHandler::Handle(request);
        }
    }
};

/*
The client code usually suited to work with a single handler.
In most cases, it is not even aware that the handler is part of chain.
*/
void Client(Handler &handler)
{
    vector<string> food = {"nut", "banana", "Cup of coffee"};
    for (string &f : food)
    {
        cout << "Client: Who wants a " << f << "?" << endl;
        const string result = handler.Handle(f);
        if (!result.empty())
        {
            cout << " " << result;
        }
        else
        {
            cout << " " << f << " was left untouched." << endl;
        }
    }
}

// The other part of the client code constucts the actual chain.
int main(int argc, char const *argv[])
{
    cout << "Chain of Responsibility" << endl;
    MonkeyHandler *monkey = new MonkeyHandler;
    SquirrelHandler *squirrel = new SquirrelHandler;
    DogHandler *dog = new DogHandler;
    monkey->SetNext(squirrel)->SetNext(dog);
    // The client should be able to send a request to any client, not just first one in the chain.
    cout << "Chain: Monkey > Squirrel > Dog\n\n";
    Client(*monkey);
    cout << "\nSubchain: Squirrel > Dog\n\n";
    Client(*squirrel);

    delete monkey;
    delete squirrel;
    delete dog;

    return 0;
}
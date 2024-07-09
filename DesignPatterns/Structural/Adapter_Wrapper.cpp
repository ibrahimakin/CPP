// https://refactoring.guru/design-patterns/adapter
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

// The Target defines domain-specific interface used by the client code.
class Target
{
public:
    virtual ~Target() = default;

    virtual string Request() const
    {
        return "Target: The default target's behavior.";
    }
};

/*
The Adaptee contains some useful behaviors, but its interface is incompatible
with the existing client code. The Adaptee needs some adaptation before the
client code can use it.
*/
class Adaptee
{
public:
    string SpecificRequest() const
    {
        return ".eetpadA eht fo roivaheb laicepS";
    }
};

// The Adapter makes Adaptee's interface compatible with the Target's interface.
class Adapter : public Target
{
private:
    Adaptee *m_adaptee;

public:
    Adapter(Adaptee *adaptee) : m_adaptee(adaptee) {}
    string Request() const override
    {
        string to_reverse = this->m_adaptee->SpecificRequest();
        reverse(to_reverse.begin(), to_reverse.end());
        return "Adapter: (Translated) " + to_reverse;
    }
};

// The client supports all classes that follow Target interface.
void Client(const Target *target)
{
    cout << target->Request();
}

int main(int argc, char const *argv[])
{
    cout << "Adapter (Wrapper)" << endl;
    cout << "Client: I can work just fine with the Target objects:" << endl;
    Target *target = new Target;
    Client(target);
    cout << "\n\n";
    Adaptee *adaptee = new Adaptee;
    cout << "Client: Adaptee class has a weird interface. See, I don't understand it:" << endl;
    cout << "Adaptee: " << adaptee->SpecificRequest() << endl << endl;
    cout << "Client: But I can work with it via the Adapter:" << endl;
    Adapter *adapter = new Adapter(adaptee);
    Client(adapter);
    cout << "\n";

    delete target;
    delete adaptee;
    delete adapter;

    return 0;
}
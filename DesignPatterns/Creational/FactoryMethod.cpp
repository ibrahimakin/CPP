// https://refactoring.guru/design-patterns/factory-method
#include <iostream>
#include <string>

using namespace std;

// The product interface declares the oprations that all concrete must implement.
class Product
{
public:
    virtual ~Product(){};
    virtual string Operation() const = 0;
};

// Concrete Products provide various implementations of the Product interface.
class ConcreteProduct1 : public Product
{
public:
    string Operation() const override
    {
        return "Result of the Concrete Product 1";
    }
};

class ConcreteProduct2 : public Product
{
public:
    string Operation() const override
    {
        return "Result of the Concrete Product 2";
    }
};

/*
The Creator class declares the factory method that is supposed to return
an object of a Product class. The Creator's subclasses usually provide
the implementation of this method.
*/
class Creator
{
    /*
    Note that the Creator class may also provide some default implementation of
    the factory method.
    */
public:
    virtual ~Creator(){};
    virtual Product *FactoryMethod() const = 0;
    /*
    Also note that, despite its name, Generally Creator's primary responsibility
    is not creating products. Usually, it contains some core business logic that
    relies on Product objects, returned by the factory method. Subclasses can
    indirectly change that business logic by overriding the factory method and
    returning a different type of product from it.
    */
    string SomeOperation() const
    {
        // Call the factory method to create a Product object.
        Product *product = this->FactoryMethod();
        // Now, we use the product.
        string result = "Creator: The same creator's code has just worked with " + product->Operation();
        delete product;
        return result;
    }
};

// Concrete Creators override the factory method in order to change the resulting product's type.
class ConcreteCreator1 : public Creator
{
    /*
    Note that the signature of the method still uses the abstract product type,
    even though the concrete product is actually returned from the method. This
    way the Creator class can stay independent from concrete product classes.
    */
public:
    Product *FactoryMethod() const override
    {
        return new ConcreteProduct1();
    }
};

class ConcreteCreator2 : public Creator
{
public:
    Product *FactoryMethod() const override
    {
        return new ConcreteProduct2();
    }
};

/*
The client code works with an instance of a concrete creator, albeit through
its base interface. As long as the client keeps working with the creator via
the base interface, you can pass it any creator's subclass.
*/
void Client(const Creator &creator)
{
    cout << "Client: I'm not aware of the creator's class, but it still works."
         << endl << creator.SomeOperation() << endl;
}

// The Apllication picks a creator's type depending on the configuraiton or environment.
int main(int argc, char const *argv[])
{
    cout << "Factory Method" << endl;
    cout << endl << "App: Launched with Concrete Creator 1." << endl;
    Creator *creator1 = new ConcreteCreator1();
    Client(*creator1);
    cout << endl << "App: Launched with Concrete Creator 2." << endl;
    Creator *creator2 = new ConcreteCreator2();
    Client(*creator2);
    delete creator1;
    delete creator2;
    return 0;
}

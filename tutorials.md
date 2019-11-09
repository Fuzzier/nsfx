# Tutorials of NSFX

## What is NSFX?

Network Simulation Frameworks (NSFX) is a header-only library that provides
a component-based architecture for creating network simulations.

The library is written in C++, and depends upon the Boost library.
Many features of the C++11 standard are used.
The library is designed to support both Windows and Linux platforms, as well as
both x86 and x64 environments.

## The basics

A component exposes its functions via a set of interfaces.
The behavior of a component is defined by the interfaces it provides and uses.

The object model is inspired by Microsoft Component Object Model (COM).
The root interface is `IObject`.
Every interface must virtually derive from `IObject`, directly or indirectly.
Each component must implement `IObject`.

~~~
class IObject
{
public:
    virtual ~IObject(void) BOOST_NOEXCEPT {}

    virtual refcount_t AddRef(void) = 0;
    virtual refcount_t Release(void) = 0;
    virtual void*      QueryInterface(const Uid& iid) = 0;
};
~~~

The first two functions provide the functionality of reference counting based
lifetime management.
The last function provides the ability to query any interfaces exposed by
a component.

Since the concrete type of a component is usually invisible, it is impossible
to use type casts to obtain the interfaces exposes by a component.
For this purpose, a component implements `QueryInterface()` function to
allow users to obtain its interfaces.
The argument of the function is the unique ID of an interface.
In fact, the unique ID is just a string literal.
It is more human-readable compared to a 128-bit UUID.

The class template `Ptr` is provided as a smart pointer.
It uses `AddRef()` and `Release()` to manage the lifetime of a component.
Moreover, it calls `QueryInterface()` to obtain interfaces exposed by
a component automatically.

## The first interface and component

### Define an interface

For simplicity, a common header file named `config.h` is written that includes
all necessary header files.

~~~
// config.h
#include <nsfx/all.h>

using namespace nsfx;
~~~

All NSFX functions and classes are placed in the `nsfx` namespace.
For convenience, the namespace is imported.

Then we define our first interface `IEcho` as follows.

~~~
// i-echo.h
#include "config.h"
#include <string>

class IEcho :
    virtual public IObject
{
public:
    virtual ~IEcho(void) BOOST_NOEXCEPT {}

    virtual std::string Shout(const std::string& msg) = 0;
};
~~~

The interface is named `IEcho`.
It is virtually derived from `IObject`, and defines its own method `Shout()`.

After the definition, a unique ID **must** be defined for the interface.
It is done by using the macro `NSFX_DEFINE_CLASS_UID()`.
The first argument is the interface, and the second argument is a string
literal that identifies the interface.
It is call an interface ID, or IID.
It is recommended to use hierarchically structured IID, as shown in the
following example.

~~~
NSFX_DEFINE_CLASS_UID(IEcho, "edu.uestc.nsfx.tutorial.IEcho");
~~~

A platform independent IID can be used for cross platform interoperability.
However, users do not have to remember or use the IID of an interface.
Users use an interface via its type, and NSFX uses its IID internally.

### Write a component

Now, we are going to make an `Echo` component that exposes the `IEcho`
interface.

~~~
// echo.h
#include "config.h"
#include "i-echo.h"

class Echo :
    public IEcho
{
public:
    Echo(void);
    virtual ~Echo(void);

    // IEcho
    virtual std::string Shout(const std::string& msg) NSFX_OVERRIDE;

private:
    // The interface map.
    NSFX_INTERFACE_MAP_BEGIN(Echo)
        NSFX_INTERFACE_ENTRY(IEcho)
    NSFX_INTERFACE_MAP_END()
};
~~~

The example shows the basic structure of writing a component.
The component implements `IEcho` by itself.

Each component **must** have an interface map.
An interface map starts with the macro `NSFX_INTERFACE_MAP_BEGIN()`, and ends
with the macro `NSFX_INTERFACE_MAP_END()`.
The argument of `NSFX_INTERFACE_MAP_BEGIN()` is the type of the component class.
By defining an interface map, the `IObject` interface is exposed by default.

To expose an interface that is directly implemented by a component, users
*shall* use the macro `NSFX_INTERFACE_ENTRY()`.
The argument of the macro is the type of the interface.
So, in the above example, the component `Echo` also exposes the `IEcho`
interface.

A component *shall* be registered.
It is done by using the macro `NSFX_REGISTER_CLASS()`.
For example,

~~~
NSFX_REGISTER_CLASS(Echo, "edu.uestc.nsfx.tutorial.Echo");
~~~

The first argument of the macro is the type of the component class, and the
second argument is the ID of the component.
It is called a class ID, or CID.
The CID of a component is also a string literal.
It is also recommend to use hierarchically structured CIDs for components.

The implementation can be written in a source file.

~~~
// echo.cpp
#include "config.h"
#include "echo.h"

Echo::Echo(void)
{
}

Echo::~Echo(void)
{
}

std::string Echo::Shout(const std::string& msg)
{
    return msg;
}
~~~

The `Shout()` method just returns the input message.

Note that, the component class is abstract as it **does not** implement
the virtual functions of the `IObject` interface.
NSFX comes up with several class templates that provide different
implementations to the `IObject` interface to suit different requirements.
A component shall focus upon implementing the task-specific virtual functions
of the provided interfaces.

### Use a component

Once a component is registered, it can be created via the `CreateObject()`
function template.
For example,

~~~
#include "config.h"
#include "i-echo.h"

void Foo(void)
{
    Ptr<IEcho> o = CreateObject<IEcho>("edu.uestc.nsfx.tutorial.Echo");
    o->Shout("Hello!");
}
~~~

The template argument of `CreateObject()` is the type of an interface that is
exposed by the component.
It **must** be explicitly specified.
The function argument is the CID of the component.

It is called *black-box reuse*, since the actually type of the component class
is hidden away from the users.
The component can be used only via its interfaces.

The return value of `CreateObject()` is a smart pointer.
The smart pointer is a specialized `Ptr` class template.
In the above example, the type of the smart pointer is `Ptr<IEcho>`.
The smart pointer can be used to invoke the methods on the `IEcho` interface.

`Ptr` can query interfaces of a component automatically.
For example,

~~~
void Bar(void)
{
    Ptr<IObject> o = CreateObject<IObject>("edu.uestc.nsfx.tutorial.Echo");
    Ptr<IEcho> p = o;
    p->Shout("Hello!");
}
~~~

In the second line, the `IEcho` interface is queried from `o`, and held by `p`.
Thus, each smart pointer holds a reference count of the component.

NSFX uses <a href='https://www.boost.org/doc/libs/release/libs/exception/doc/boost-exception.html'>
Boost.Exception</a> library to support exception handling.
Thus, it would be better to guard the code within a `try-catch` block.
For example,

~~~
try
{
    Foo();
    Bar();
}
catch (boost::exception& e)
{
    std::cout << diagnostic_information(e) << std::endl;
}
catch (std::exception& e)
{
    std::cout << e.what() << std::endl;
}
~~~

### Exercise

1. Set up an environment to build the tutorial, and execute the generated
   executable.

2. Write an interface and a component that exposes it.
   The simplest way is to duplicate the provided header and source files as
   a starting point, and modify them as needed.

## Testing

NSFX provides an easy-to-use testing framework.
To use the testing framework, one shall include `<nsfx/test.h>`.

### Test suite

A source file can have several test suites.
Each test suit can contain nested test suites.
A test suit can be declared by the macro `NSFX_TEST_SUITE()`.
For example,

~~~
NSFX_TEST_SUITE(Echo)
{
}
~~~

Actually, the macro opens a new namespace named `S_Echo`.
Thus, the name of the test suite can be any C++ identifiers.

### Test case

A test suit can also contain several test cases.
A test case can be declared by the macro `NSFX_TEST_CASE()`.

~~~
NSFX_TEST_SUITE(Echo)
{
    NSFX_TEST_CASE(FooBar)
    {
    }
}
~~~

Actually, the macro `NSFX_TEST_CASE()` defines a C++ function named `C_FooBar`,
which is invoked during the test running process.
Test cases are leaves in the hierarchy.
So, a test case cannot contain test suites or test cases.

### Test assertion

A test case can contain several test assertions.
There are two kinds of test assertions: non-critical and critical.
When a non-fatal test assertion fails, the error message is collected, and
the test running process continues.
When a critical test assertion fails, the error message is also collected, but
the test running process is terminated.

The following macros declare non-critical test assertions:
* `NSFX_TEST_EXPECT(p)`: `p` is a boolean expression that is expected to be true.
* `NSFX_TEST_EXPECT_EQ(a, b)`: `a == b` is expected.
* `NSFX_TEST_EXPECT_NE(a, b)`: `a != b` is expected.
* `NSFX_TEST_EXPECT_LT(a, b)`: `a < b` is expected.
* `NSFX_TEST_EXPECT_LE(a, b)`: `a <= b` is expected.
* `NSFX_TEST_EXPECT_GT(a, b)`: `a > b` is expected.
* `NSFX_TEST_EXPECT_GE(a, b)`: `a >= b` is expected.
* `NSFX_TEST_EXPECT_AC(a, b, e)`: `|a - b| < e` is expected (absolute closeness).
* `NSFX_TEST_EXPECT_RC(a, b, e)`: <code>|a - b| < e&middot;b</code> is expected(relative closeness).

The following macros declare critical test assertions:
* `NSFX_TEST_ASSERT(p)`: `p` is a boolean expression that is expected to be true.
* `NSFX_TEST_ASSERT_EQ(a, b)`: `a == b` is expected.
* `NSFX_TEST_ASSERT_NE(a, b)`: `a != b` is expected.
* `NSFX_TEST_ASSERT_LT(a, b)`: `a < b` is expected.
* `NSFX_TEST_ASSERT_LE(a, b)`: `a <= b` is expected.
* `NSFX_TEST_ASSERT_GT(a, b)`: `a > b` is expected.
* `NSFX_TEST_ASSERT_GE(a, b)`: `a >= b` is expected.
* `NSFX_TEST_ASSERT_AC(a, b, e)`: `|a - b| < e` is expected (absolute closeness).
* `NSFX_TEST_ASSERT_RC(a, b, e)`: <code>|a - b| < e&middot;b</code> is expected(relative closeness).

When a critical test assertion fails, the test process is terminated.

All arguments **must** support output stream operator.
The following example shows how to use test assertions.

~~~
NSFX_TEST_SUITE(Echo)
{
    void Foo(void)
    {
        Ptr<IEcho> o = CreateObject<IEcho>("edu.uestc.nsfx.tutorial.Echo");
        NSFX_TEST_ASSERT(o);
        NSFX_TEST_EXPECT_EQ(o->Shout("Hello!"), "Hello!");
    }

    NSFX_TEST_CASE(Foo)
    {
        Foo();
    }
}
~~~

All test assertion supports output stream operator to provide additional
information when the test assertion fails.
For example,
~~~
NSFX_TEST_CASE(Bar)
{
  int a = 0;
  NSFX_TEST_EXPECT_EQ(a, 1) << "The value of a is " << a;
}
~~~

### Run tests

To run tests, the users shall specify one or more output streams to receive
the message generated during the test running.
Typical output streams include `std::cerr` and text files.
For example,

~~~
int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}
~~~

### Exercise

1. Write a test suit that contains several test cases.
   Write some test assertions.

## Reusing components

### Combination

Combination is a common method of reusing a component.
An outer component exposes interfaces that is implemented by reusing inner
components.
For example, let us define an interface `INameable`.

~~~
// i-nameable.h
#include "config.h"
#include <string>

class INameable :
    virtual public IObject
{
public:
    virtual ~INameable(void) BOOST_NOEXCEPT {}

    virtual void SetName(const std::string& name) = 0;
    virtual std::string GetName(void) = 0;
};

NSFX_DEFINE_CLASS_UID(INameable, "edu.uestc.nsfx.tutorial.INameable");
~~~

Then we write a component `Nameable` that provides the interface.

~~~
// nameable.h
#include "config.h"
#include "i-nameable.h"

class Nameable :
    public INameable
{
public:
    Nameable(void);
    virtual ~Nameable(void);

    // INameable
    virtual void SetName(const std::string& name) NSFX_OVERRIDE;
    virtual std::string GetName(void) NSFX_OVERRIDE;

public:
    // A public function.
    // However, this function is not exposed by an interface.
    void SetDefault(void);

private:
    NSFX_INTERFACE_MAP_BEGIN(Nameable)
        NSFX_INTERFACE_ENTRY(INameable)
    NSFX_INTERFACE_MAP_END()

private:
    std::string name_;
};

NSFX_REGISTER_CLASS(Nameable, "edu.uestc.nsfx.tutorial.Nameable");
~~~

The implementation of `Nameable` is written in the following example.

~~~
// nameable.cpp
#include "config.h"
#include "nameable.h"

Nameable::Nameable(void)
{
}

Nameable::~Nameable(void)
{
}

void Nameable::SetName(const std::string& name)
{
    name_ = name;
}

std::string Nameable::GetName(void)
{
    return name_;
}

void Nameable::SetDefault(void)
{
    name_ = "A default name";
}
~~~

Suppose that another component wants to provide `INameable`.
It can create `Nameable` to hold the name.
For example,

~~~
// combination.h
#include "config.h"
#include "nameable/i-nameable.h"

class Combination :
    public INameable
{
public:
    Combination(void);
    virtual ~Combination(void);

    // INameable
    virtual void SetName(const std::string& name) NSFX_OVERRIDE;
    virtual std::string GetName(void) NSFX_OVERRIDE;

private:
    NSFX_INTERFACE_MAP_BEGIN(Combination)
        NSFX_INTERFACE_ENTRY(INameable)
    NSFX_INTERFACE_MAP_END()

private:
    // The INameable interface of the inner component is stored.
    Ptr<INameable> nameable_;
};

NSFX_REGISTER_CLASS(Combination, "edu.uestc.nsfx.tutorial.Combination");
~~~

In the constructor of `Combination`, a component with a CID of
`"edu.uestc.nsfx.tutorial.Nameable"`is created, and its interface `INameable`
is stored in `nameable_`.
For example,

~~~
// combination.cpp
#include "config.h"
#include "combination.h"

Combination::Combination(void) :
    // Black-box reuse.
    // The underlying type of "edu.uestc.nsfx.Nameable" is unknown, and
    // the component is accessed via its interfaces.
    nameable_(CreateObject<INameable>("edu.uestc.nsfx.tutorial.Nameable"))
{
}
~~~

To implement the `INameable` interface, `Combination` delegates the call to
the inner component.
For example,

~~~
void Combination::SetName(const std::string& name)
{
    // Delegate to the inner component.
    nameable_->SetName(name);
}

std::string Combination::GetName(void)
{
    // Delegate to the inner component.
    return nameable_->GetName();
}
~~~

### Aggregation

In combination, a virtual call to an interface is delegated to an inner
component.
It costs two virtual function calls.
If the combination hierarchy is deeper, the costs would grow proportionally.

If the outer component just want to expose an interface of an inner component
as is, aggregation can be used to reduce the number of virtual calls, at the
cost of slower querying of interfaces.

The following example shows how to aggregate a component.

~~~
// aggregation.h
#include "config.h"
#include "nameable/i-nameable.h"

class Aggregation :
    virtual public IObject // Do not implement INameable directly.
{
public:
    Aggregation(void);
    virtual ~Aggregation(void);

private:
    NSFX_INTERFACE_MAP_BEGIN(Aggregation)
        // Provide an interface via aggregation.
        // The second argument MUST be the IObject* interface
        // exposed by the aggregated component.
        NSFX_INTERFACE_AGGREGATED_ENTRY(INameable, &*nameableObj_)
    NSFX_INTERFACE_MAP_END()

private:
    // The IObject interface of the aggregated component is stored.
    Ptr<IObject> nameableObj_;
};

NSFX_REGISTER_CLASS(Aggregation, "edu.uestc.nsfx.tutorial.Aggregation");
~~~

`Aggregation` does *not* implement `INameable` by itself.
Thus, it does not derived from the interface.
Instead, it derives from `IObject` directly.

In the interface map, the macro `NSFX_INTERFACE_AGGREGATED_ENTRY()` *shall* be
used to provide an interface exposed by an aggregated component.
The second argument **must** be the `IObject` interface exposed by
the aggregated component.

`Aggregation` creates the aggregated component in its constructor.
For example,

~~~
// aggregation.cpp
#include "config.h"
#include "aggregation.h"

Aggregation::Aggregation(void) :
    // This component is the controller of the aggregated component.
    nameableObj_(CreateObject<IObject>("edu.uestc.nsfx.tutorial.Nameable",
                                       /* controller = */this))
{
    // The implementation of IObject on the aggregate component depends upon
    // the implemenation of IObject on its controller.
    // However, IObject on this component has not been implemented yet.
    //
    // Thus, neither query interface, nor modify the reference counter of
    // the aggregated component in the contructor.
}
~~~

`CreateObject()` is provided with a second argument, i.e., `this` pointer of
`Aggregation`.
It is also called the *controller* of the aggregated component.

All interfaces exposed by a component must share the same lifetime, and be able
to query each other.
Thus, an interface, that is implemented by an aggregated component,
must be able to manage the reference counter of the controller, and
query the controller's interfaces.

### Gray-box reuse

For end users, a component can only be manipulated via its interfaces.
However, internal to a program, a component is not a black box, and it may
provide internal functions that are not exposed to the end users.
Thus, the internal functions of a component can be reused, which usually
provide better efficiency.
This kind of reuse is called gray-box reuse.

A component is gray-box re-usable if its underlying class is visible.
For gray-box reuse, a component is reused by its type, so there is no need to
use `NSFX_REGISTER_CLASS()` macro to define a CID for it.

For example, suppose the definition of `Nameable` class is visible, we can
reuse it by using the `MemberAggObject` class template.

~~~
// member-aggregation.h
#include "config.h"
#include "nameable/nameable.h"

class MemberAggregation :
    virtual public IObject
{
public:
    MemberAggregation(void);
    virtual ~MemberAggregation(void);

private:
    NSFX_INTERFACE_MAP_BEGIN(MemberAggregation)
        // The second argument MUST be the IObject* interface
        // exposed by the aggregated component.
        NSFX_INTERFACE_AGGREGATED_ENTRY(INameable, &nameable_)
    NSFX_INTERFACE_MAP_END()

private:
    // MemberAggObject implements the IObject interface of the Nameable class,
    // which supports aggregation.
    MemberAggObject<Nameable>  nameable_;
};

NSFX_REGISTER_CLASS(MemberAggregation,
                    "edu.uestc.nsfx.tutorial.MemberAggregation");
~~~

Unlike the black-box aggregation method, member variable based gray-box
aggregation method does not allocate the aggregated component dynamically.
Instead, the aggregated component is defined as a member variable of
the controller component.
Moreover, gray-box aggregation allows the access to any public functions of
the aggregated component.

In the constructor of the controller component, the member variable is
initialized.
For example,

~~~
// member-aggregation.cpp
#include "config.h"
#include "member-aggregation.h"

MemberAggregation::MemberAggregation(void) :
    // Specify the controller of the aggregation component.
    nameable_(/* controller = */this)
{
    // The implementation of IObject on the aggregate component depends upon
    // the implemenation of IObject on its controller.
    // However, IObject on this component has not been implemented yet.
    //
    // Thus, neither query interface, nor modify the reference counter of
    // the aggregated component in the contructor.

    // Use GetImpl() to access the aggregated component class.
    // Any public functions can be accessed.
    nameable_.GetImpl()->SetDefault();
}
~~~

The `nameable_` component is initialized as a member variable by specifying
its controller.
Similar to the black-box aggregation method, the `IObject` interface cannot be
used during construction, as the methods on the interface have not been fully
implemented.

To enable gray-box reuse, `MemberAggObject<T>` provides a public function
`GetImpl()` that returns a pointer of type `T*` to expose the wrapped component.
For example, `nameable_.GetImpl()` returns a pointer to the wrapped `Nameable`
component, and its public function `SetDefault()` can be called.

Indeed, NSFX comes up with several class templates that provides different
implementations to the `IObject` interface to suit different requirements.

#### `Object<T>`

* A standalone component.
* It has a reference counter itself.
* It must be allocated on heap.
* It does not support aggregation.

For example,

~~~
Ptr<Nameable> o = new Object<Nameable>();
o->SetDefault();
~~~

#### `StaticObject<T>`

* A standalone component with static lifetime.
* The component *shall* be defined as a static variable.
* Its reference counter is always 1, thus it will never be deallocated.
* It does not support aggregation.

For example,

~~~
static StaticObject<Nameable> o();
o.GetImpl()->SetDefault();
~~~

#### `AggObject<T>`

* An aggregable component that must be dynamically allocated.
* It has a reference counter itself.
* It must be used in aggregation.
* The wrapped component shares the reference counter with the controller.
* The wrapped component delegates interface querying to the controller.

For example,

~~~
Ptr<Nameable> o = new AggObject<Nameable>(controller);
o->SetDefault();
~~~

#### `MemberAggObject<T>`

* An aggregable component that must be defined as a member variable.
* Its reference counter is always 1, and is destructed along with the controller.
* It must be used in aggregation.
* The wrapped component shares the reference counter with the controller.
* The wrapped component delegates interface querying to the controller.

For example,

~~~
// In class definition.
MemberAggObject<Nameable> o;

// In the initialization list of the controller's constructor.
o(controller);

// Public member functions of the aggregated component can be accessed.
o.GetImpl()->SetDefault();
~~~

#### `MutualObject<T>`

* An aggregable component that must be defined as a member variable.
* Its reference counter is always 1, and is destructed along with the controller.
* It must be used in aggregation.
* The wrapped component shares the same lifetime with the controller.
* However, the wrapped component and the controller expose *separate* sets of
  interfaces.
* Thus, it **must not** be exposed in the interface map of the controller.

For example,

~~~
// In class definition.
MutualObject<Nameable> o;

// In the initialization list of the controller's constructor.
o(controller);

// Public member functions of the aggregated component can be accessed.
o.GetImpl()->SetDefault();
~~~

## Wiring and initialization

The behavior of a component is defined by the interfaces it provides and uses.
In block-box reuse, a component is represented by its `IObject` interface.
This interface is the entry point to access the functions of a component.
For this reason, a component that supports black-box reuse must be default
constructible (have a default constructor).
And the initialization, configuration and utilization of a component must
be done via its interfaces.

To allow a component to use other interfaces, a special kind of *user*
interfaces are defined.
For example,

~~~
class IEchoUser :
    virtual public IObject
{
public:
    virtual ~IEchoUser(void) BOOST_NOEXCEPT {}

    virtual void Use(Ptr<IEcho> echo) = 0;
};

NSFX_DEFINE_CLASS_UID(IEchoUser, "edu.uestc.nsfx.tutorial.IEchoUser");
~~~

A *user* interface has a type name that looks like `IXxxUser`, and provides
a single virtual function `Use()` that accepts a single parameter of the used
interface.

To simplify the definition of *user* interfaces, NSFX provides a macro named
`NSFX_DEFINE_USER_INTERFACE()`.
The following definition is equivalent to the above example.

~~~
NSFX_DEFINE_USER_INTERFACE(IEchoUser, "edu.uestc.nsfx.tutorial.IEchoUser",
                           IEcho);
~~~

For a component that uses/requires the `IEcho` interface, it *shall* expose
the `IEchoUser` interface to allow the users of the component to provide it
with the required interface.
For example,

~~~
class SomeComponent :
    public IEchoUser
{
public:
    ...

    virtual void Use(Ptr<IEcho> echo) NSFX_OVERRIDE
    {
        echo_ = echo;
    }

private:
    NSFX_INTERFACE_MAP_BEGIN(SomeComponent)
        NSFX_INTERFACE_ENTRY(IEchoUser)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<IEcho> echo_;
};

NSFX_REGISTER_CLASS(SomeComponent, "edu.uestc.nsfx.tutorial.SomeComponent");
~~~

In the above example, `SomeComponent` exposes `IEchoUse` in its interface map.
Thus, it *explicitly* states that it requires an `IEcho` interface before it
can work properly.

It is a tightly coupled relationship between a component and an interface
it uses.
The process of providing a component with all the interfaces it uses is called
*wiring*.
This word comes from TinyOS.

Once all of the used interfaces are provided, a component can be *initialized*.
Usually, a component exposes the `IInitializable` interface to allow its users
to initialize it.
The `IInitializable` interface is defined as follows.

~~~
class IInitializable :
    virtual public IObject
{
public:
    virtual ~IInitializable(void) BOOST_NOEXCEPT {};

    virtual void Initialize(void) = 0;
};

NSFX_DEFINE_CLASS_UID(IInitializable, "edu.uestc.nsfx.IInitializable");
~~~

The following example shows how to wire components together and initialize them.

~~~
// Create.
Ptr<IObject> e = CreateObject<IObject>("edu.uestc.nsfx.tutorial.Echo");
Ptr<IObject> o = CreateObject<IObject>("edu.uestc.nsfx.tutorial.SomeComponent");

// Wire.
Ptr<IEchoUser>(o)->Use(e);

// Initialize.
Ptr<IInitializable>(o)->Initialize();
~~~

## Event

Event is an important tool to enable a loosely decoupled relationship among
components.
An event is fired/triggered by an event source, and listened/processed by
zero, one or more event sinks.

### Event sink interface

An event sink interface has a single virtual function named `Fire()`.
This function is implemented by the event listeners.
When an event happens, the event source will invoke this function.

To define an event sink interface, use the macro
`NSFX_DEFINE_EVENT_SINK_INTERFACE()`.
For example,

~~~
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    IAlarmEventSink, "edu.uestc.nsfx.tutorial.IAlarmEventSink",
    ( void(void) ));
~~~

* The first argument is the type name of the event sink interface, which is
`IAlarmEventSink`.
* The second argument is its IID, which is `"edu.uestc.nsfx.tutorial.IAlarmEventSink"`.
* The third argument is the prototype of the callback, which is `void(void)`.
  That is, the interface has a single virtual function with a prototype of
  `void Fire(void)`.

### Event interface

An event interface has two virtual functions: `Connect()` and `Disconnect()`.
They are implemented by event sources.
It allows an event sink to connect (listen) to an event source at runtime, and
disconnect from it when the event is no longer interested.

To define an event interface, use the macro `NSFX_DEFINE_EVENT_INTERFACE()`.
For example,

~~~
NSFX_DEFINE_EVENT_INTERFACE(
    IAlarmEvent, "edu.uestc.nsfx.tutorial.IAlarmEvent",
    IAlarmEventSink);
~~~

* The first argument is the type name of event interface, which is `IAlarmEvent`.
* The second argument is its IID, which is `"edu.uestc.nsfx.tutorial.IAlarmEvent"`.
* The third argument is the associated event sink interface, which is
  `IAlarmEventSink`.

### Write an event source

The following example shows a countdown alarm that fires an alarm event when
its counter reaches zero.
The countdown alarm provides the `ICountdown` interface, which allows users to
set an initial counter, and decrement it until it reaches zero.

~~~
class ICountdown :
    virtual public IObject
{
public:
    virtual ~ICountdown(void) BOOST_NOEXCEPT {}

    // Set the counter.
    virtual void SetCounter(uint32_t c) = 0;

    // Decrement the counter by one until it reaches zeros.
    virtual void Decrement(void) = 0;
};

NSFX_DEFINE_CLASS_UID(ICountdown, "edu.uestc.nsfx.tutorial.ICountdown");
~~~

The countdown alarm is implemented by the `CountdownAlarm` class.
Besides `ICountdown`, it also provide `IAlarmEvent`.
An alarm event will be fired then the counter reaches zero.

~~~
class CountdownAlarm :
    public ICountdown
{
public:
    CountdownAlarm(void);
    virtual ~CountdownAlarm(void);

    // ICountdown
    virtual void SetCounter(uint32_t c) NSFX_OVERRIDE;
    virtual void Decrement(void) NSFX_OVERRIDE;

private:
    NSFX_INTERFACE_MAP_BEGIN(CountdownAlarm)
        NSFX_INTERFACE_ENTRY(ICountdown)
        NSFX_INTERFACE_AGGREGATED_ENTRY(IAlarmEvent, &alarmEvent_)
    NSFX_INTERFACE_MAP_END()

private:
    uint32_t counter_;

    // Reuse Event<> to implement IAlarmEvent.
    MemberAggObject<Event<IAlarmEvent>> alarmEvent_;
};

NSFX_REGISTER_CLASS(CountdownAlarm, "edu.uestc.nsfx.tutorial.CountdownAlarm");
~~~

The `Event<IXxxEvent>` class template provides a standard implementation for
`Connect()` and `Disconnect()` of the `IXxxEvent` interface.
The `Event<IXxxEvent>` can then be aggregated as a member variable.

As shown in the above example, `Event<IAlarmEvent>` is a component that has a
standard implementation of `IAlarmEvent`.
The `CountdownAlarm` component reuses `Event<IAlarmEvent>` by aggregating it as
a member variable.

`Event<IXxxEvent>` provides a public function `Fire()` that has the same
prototype as the virtual function `Fire()` of `IXxxEvent`.
If one or more event sinks are connected to the event source, the function
`Event<IXxxEvent>::Fire()` invokes `IXxxEvent::Fire()` for each event sink.

For example,

~~~
CountdownAlarm::CountdownAlarm(void) :
    counter_(0),
    alarmEvent_(/* controller = */this)
{
}

void CountdownAlarm::Decrement(void)
{
    if (--counter_ == 0)
    {
        alarmEvent_.GetImpl()->Fire();
    }
}
~~~

In the `Decrement()` function, when the counter reaches zero, an alarm event
is fired.

### Write an event sink

There are several ways to create a event sink.
The easiest way is to use the `CreateEventSink()` function template.
For example,

~~~
Ptr<IAlarmEventSink> s = CreateEventSink<IAlarmEventSink>(nullptr, [] {
                             std::cout << "Alarm fired!" << std::endl;
                         });
~~~

* The template argument is the type of the event sink interface.
* The first argument is the controller.
  If the event sink is standalone (not aggregated), `nullptr` shall be specified.
* The second argument is a callable object.
  It can be a function pointer, a lambda expression, or a functor (an object
  that has an overloaded function call operator `operator()`).

In the above example, a lambda express is specified.
When an alarm event is fired, the event sink outputs "Alarm fired!" to the
standard output stream.

Another way to implement an event sink is to reuse the `EventSink` class
template.
For example,

~~~
class AlarmListener :
    virtual public IObject
{
public:
    AlarmListener(void);
    virtual ~AlarmListener(void);

private:
    // The function to process alarm event.
    void OnAlarm(void);

    NSFX_INTERFACE_MAP_BEGIN(AlarmListener)
        NSFX_INTERFACE_AGGREGATED_ENTRY(IAlarmEventSink, &alarmEventSink_)
    NSFX_INTERFACE_MAP_END()

private:
    MemberAggObject<MemberFunctionBasedEventSink<
        IAlarmEventSink, AlarmListener>>
             alarmEventSink_;
};

NSFX_REGISTER_CLASS(AlarmListener, "edu.uestc.nsfx.tutorial.AlarmListener");
~~~

The `MemberFunctionBasedEventSink<>` class template provides a standard
implementation for the virtual function `Fire()` of an event sink interface.
As the name implies, the `Fire()` function is implemented by invoking a member
function of an object.

* The first template argument is the event sink interface.
* The second template argument is the type of the object whose member function
  is going to process the event.

The member function *must* have the same signature as the `Fire()` function
of the event sink.
In the above example, `OnAlarm()` is used to process the event, so it has
the same signature as the `Fire()` function of `IAlarmEventSink`.
That is, the return type is `void`, and it has no parameters.

To initialize `alarmEventSink_`, a pointer to the controller shall be provided,
as it is aggregated as a member variable.
Besides, an object and a pointer to its member function shall also be specified.
For example,

~~~
AlarmListener::AlarmListener(void) :
    alarmEventSink_(/* controller = */this, /* o = */this,
                    /* ptmf = */&AlarmListener::OnAlarm)
{
}
~~~

Any non-default constructors of a gray-box reused component can be called.
The constructor of `MemberAggObject` requires a pointer to the controller, and
any other arguments are passed to the constructor of the wrapped component.
Thus, the first `this` specifies the controller of the aggregated event sink;
the second `this` and the last pointer to member function
`&AlarmListener::OnAlarm` are used to construct the wrapped
`MemberFunctionBasedEventSink` component.
This piece of code states that the alarm event is processed by the `OnAlarm()`
function of this object.

The alarm event is processed by `OnAlarm()` as follows.

~~~
void AlarmListener::OnAlarm(void)
{
    std::cout << "Alarm event is processed by OnAlarm()!" << std::endl;
}
~~~

### Connect an event sink

Once the event source and event sink are prepared, they can be connected and
interact with each other.
For example,

~~~
Ptr<ICountdown> alarm = CreateObject<ICountdown>(
            "edu.uestc.nsfx.tutorial.CountdownAlarm");

Ptr<IAlarmEventSink> sink1 =
    CreateEventSink<IAlarmEventSink>(nullptr, [] {
        std::cout << "Alarm fired!" << std::endl;
    });
cookie_t cookie1 = Ptr<IAlarmEvent>(alarm)->Connect(sink1);

Ptr<IAlarmEventSink> sink2 = CreateObject<IObject>(
            "edu.uestc.nsfx.tutorial.AlarmListener");
cookie_t cookie2 = Ptr<IAlarmEvent>(alarm)->Connect(sink2);
~~~

Two event sinks are created and connected to an event sink.
For an event source, each connect is identified by a cookie value of type
`cookie_t`, which is actually `uint32_t`.
For a valid connection, the cookie is a *non-zero* value.
That is, the `Connect()` function of an event interface will never return `0`.

When the counter is decremented to zero, the alarm event is fired.
For example,

~~~
alarm->SetCounter(1);
alarm->Decrement();
~~~

The cookie can be used to disconnect the event sink when it is no longer
interested in the event.
For example,

~~~
Ptr<IAlarmEvent>(alarm)->Disconnect(cookie1);
cookie1 = 0;
~~~

After disconnection, the cookie value **shall** be reset to `0`.
When the event is fired again, only the second event sink will output message.

### Exercise

1. Define an event sink interface and an associated event interface.
2. Write an event source component and an event sink component.
3. Create the components and connect them together.
4. Fire the event and observe the result.

## Timing

### Time system

Timing is a fundamental factor of a communication protocol.
To enable timing, a time system have to be specified.
A time system shall provide the follow specifications.

* Time point.
  A time point is a location in time.
  For example, 2019-05-23 09:12:27.

* Time duration.
  A free length of time.
  For example, 3 seconds.
  It is also called a time period.

* Time resolution.
  The smallest representable time.
  It is also called the fundamental time period.

* Time epoch.
  The origin of time point.
  A time point is represented by the duration from the epoch.

* Clock.
  A hardware/software component that provides the current time point with
  respect to a time system.

For example, in a simulation, the epoch usually refers to the time point when
the simulation starts.
In the time system of a simulation, the time point at the epoch is usually
defined as `0`.
Thus, the current time point in a simulation is equal to the duration the
simulation has elapsed.
That is, when the simulation has has run for 10 seconds, the current time point
of the simulation is also 10 seconds.
This is why most current simulation systems do not distinguish between a time
point and a time duration.
However, NSFX does regard them as different concepts.

In NSFX, the time system has a resolution of *1/10 nanoseconds*.
* The number of fundamental periods of duration is human-readable.
* Represent multiples of *1/1024* seconds without loss of precision.

### Duration

A time duration is represented by the number of fundamental time periods.
This number is defined as a signed integer of type `count_t`, which is currently
defined as `int64_t`.
For example, for a fundamental time period of 1/10 nanoseconds, the range of
representable duration is about -29 to 29 years, which is a fairly enough range
for common network simulations.

Common operations has been defined for `Duration`.

#### Maker functions

* `Days(n)`
* `Hours(n)`
* `Minutes(n)`
* `Seconds(n)`
* `MilliSeconds(n)`
* `MicroSeconds(n)`
* `NanoSeconds(n)`
* `PicoSeconds(n)`
* `FemtoSeconds(n)`
* `AttoSeconds(n)`

The parameter `n` is of type `count_t` (integer).
If the duration to make is smaller than a fundamental period, its rounded
down to `0`.

#### Comparison

~~~
Seconds(1) == Seconds(1);
Seconds(1) != Seconds(2);
Seconds(1) <  Seconds(2);
Seconds(1) <= Seconds(1);
Seconds(2) >  Seconds(1);
Seconds(1) >= Seconds(1);
~~~

#### Arithmetic

~~~
auto dt = Seconds(1) + MilliSeconds(1);   // 1001 ms
dt = Seconds(1) - MilliSeconds(1);        // 999 ms
dt = 2 * Seconds(1);                      // 2 s
dt = Seconds(1) * 2;                      // 2 s
count_t k = Seconds(1) / MilliSeconds(3); // 333
dt = MilliSeconds(1000) / 3;              // 333 ms
dt = Seconds(1) / 3;                      // 0 s
dt = -Seconds(1);                         // -1 s
bool f = !Seconds(0);                     // true
f = !Seconds(1);                          // false
~~~

All calculations are within the integral domain, and the resulted duration
has the smallest fundamental period of all operands.

### TimePoint

The `TimePoint` class template models the concept of time point.
To specialize the class template, a clock type and a duration type shall be
specified.

The clock type argument is used to specify various properties of a time system:
its resolution, duration type and epoch.
The duration type argument gives the position of a time point relative to
the epoch.

A time point is constructed from a duration.
For example,

~~~
TimePoint t0(Seconds(1));  // A time point that is 1 second later than the epoch.

TimePoint t1 = t0 + MilliSeconds(1); // A time point that is 1 ms later than t0.

auto dt = t1 - t0; // 1 ms.
~~~

## Scheduler & Simulator

### Scheduler

A scheduler is a component commonly used in discreet event simulation.
The essential function of a scheduler is to provide timeout, that is, fires an
event at a specified time point.

A scheduler provides `IScheduler` interface.
The interface provides three methods `ScheduleNow()`, `ScheduleAt()` and
`ScheduleIn()` for scheduling events.
The prototype of a scheduler event is always `void Fire(void)`.
For example,

~~~
Ptr<IEventSink<>> s = CreateEventSink<IEventSink<>>(nullptr, [] {
                          std::cout << "An event is fired!" << std::endl;
                      });

// Fires an event at the current time point.
scheduler->ScheduleNow(s);

// Fires an event at the time point of 1 second (1 second later than the epoch).
TimePoint t0(Seconds(1));
scheduler->ScheduleAt(t0, s);

// Fires an event in 2 seconds.
Duration dt = Seconds(2);
scheduler->ScheduleIn(dt, s);
~~~

The scheduling functions return an event handle.
An event handle is interface that can be used to manipulate the scheduled event.
For example,

~~~
Ptr<IEventHandle> h0 = scheduler->ScheduleNow(s);

TimePoint t0(Seconds(1));
Ptr<IEventHandle> h1 = scheduler->ScheduleAt(t0, s);

Duration dt = Seconds(2);
Ptr<IEventHandle> h2 = scheduler->ScheduleIn(dt, s);

// If the event handle is not null and is not fired yet.
if (h0 && h0->IsPending())
{
  // Cancel the scheduled event.
  h0->Cancel();
}
~~~

Internally, a scheduler stores an ordered list of events.
The events are kept in ascending order of their firing time points.

A scheduled event also has a monotonically increasing ID, which is an unsigned
64-bit integer value that records how many events has been added into the
scheduler.
That is, the event IDs record the order of the events when they are added into
the scheduler.

When several events have the same firing time point, their IDs are used to
break the tie, and they are kept in ascending order of the IDs.

Different schedulers provide different underlying data structures and different
algorithms for insertion and deletion.
The library provides several schedulers.
For example,

~~~
Ptr<IScheduler> scheduler1 = CreateObject<IScheduler>("edu.uestc.nsfx.HeapScheduler");
Ptr<IScheduler> scheduler2 = CreateObject<IScheduler>("edu.uestc.nsfx.SetScheduler");
Ptr<IScheduler> scheduler3 = CreateObject<IScheduler>("edu.uestc.nsfx.ListScheduler");
~~~

To prevent users from scheduling events that fire before the current time point,
the scheduler requires an `IClock` interface.
This interface provides `Now()` methods for querying the current simulation
time point.

### Simulator

A scheduler stores events, while a simulator controls the process of dequeuing
and firing the events.

A simulator provides `ISimulator` interface, and requires a `IScheduler`
interface.
The `ISimulator` interface provides `Run()`, `RunUntil()`
and `RunFor()` methods for running the simulation.

The default simulator is `"edu.uestc.nsfx.Simulator"`.
It also provides a `IClock` interface.
For example,

~~~
Ptr<ISimulator> simulator = CreateObject<ISimulator>("edu.uestc.nsfx.Simulator");
Ptr<IScheduler> scheduler = CreateObject<IScheduler>("edu.uestc.nsfx.HeapScheduler");

Ptr<IClock> clock(simulator);
Ptr<IClockUser>(scheduler)->Use(clock);
Ptr<ISchedulerUser>(simulator)->Use(scheduler);

Ptr<IEventSink<>> s = CreateEventSink<IEventSink<>>(nullptr, [&] {
        std::cout << clock->Now() << std::endl;
        if (clock->Now() <= TimePoint(Seconds(3)))
        {
            scheduler->ScheduleIn(Seconds(1), s);
        }
    });

// Schedule one event.
scheduler->ScheduleNow(s);

// Run until time point 1 seconds, and fires two events.
simulator->RunUntil(TimePoint(Seconds(1)));

// Run for 1 second, and fires one event.
simulator->RunFor(Seconds(1));

// Run until the scheduler becomes empty, and fires two events.
simulator->Run();
~~~

### Wall clock and local clocks

A time point is represented by a 64-bit integer value.
To compare two time points in the same time system, one can just compare their
underlying integer values.
However, for time points defined in different time systems, their comparison
can be complicated.

For simplicity, the time points of all scheduled events in the same scheduler
**shall** be defined in the same time system.

In a simulation, one usually creates a system-wide scheduler.
The time system used by this scheduler is a system-wide reference time system
for all components.
Therefore, a clock device that provides time points with respect to this time
system is usually called a *wall clock*.

A wall clock in a simulation usually simulates the physical time flow of all
objects in the system.

However, a component may run in a different time system.
For example, a node has an adjustable local clock that runs at a different pace
with the wall clock.
For this purpose, a component **shall** have a *local clock*.

Usually, the component **shall** have a *local scheduler* as well, in order to
separate different time points in different time systems.
It is especially useful when the local clock is adjustable.
The adjustment of the local clock changes the local time points in the local
scheduler, but the order of the events is usually unchanged.
Thus, the local events does not have to be reordered with the events in the
system-wide scheduler.

## Network Utilities

### Address

The library provides `Address` class template to model network address.
Essentially, `Address` class template represents a fixed-length unsigned
integer value.

The template argument of `Address` is the bit length of the address.
For example,

~~~
// Defaults to 0.
Address<8>   a1;

// Assign an initial value.
Address<16>  a2(0x2dfe);

// Assign an initial value via a big-endian buffer.
const uint8_t bytes[] = { 0xc0, 0xab, 0x01, 0x64 };
Address<32>  a4(bytes, sizeof (bytes), big_endian);

// Assign an initial value via byte sequence.
Address<48>  a6(0xac, 0xb1, 0x07, 0xae, 0x31, 0xcd, big_endian);

// Assign an initial value via uint32 sequence.
Address<128> a8(0x92c3fe81, 0xeb81630b, 0x7d6bdac9, 0x35da031a, big_endian);
~~~

The bit size of an address can be obtained by the static member function
`GetBitSize()`,
the byte size can be obtained by the static member function `GetSize()`.
For example,
~~~
size_t bitSize  = Address<48>::GetBitSize();
size_t byteSize = Address<48>::GetSize();
~~~

### Packet

#### Buffer

In NSFX, a *packet* is modeled as a byte sequence.
Thus, the major part of a packet is a *buffer*.
It is easy to convert between a simulation packet and a physical packet.

As a convention of networking, a packet is modified by adding/removing a header
or trailer.

For memory efficiency, a packet may be shared by several components.
When several components holds a packet, if the packet is modified by one of the
components, the modification is **not** visible to any other holders.
Internally, if the modification would be visible to other components, the packet
is copied for the modifying component, and the modification is only applied to
the new copy.
For example,

~~~
// Creates an empty packet.
Packet packet;

// Add a header to the packet by 8 bytes.
PacketBuffer header = packet.AddHeader(8);

// Add a trailer to the packet by 4 bytes.
PacketBuffer trailer = packet.AddTrailer(4);
~~~

To write data into a buffer, users shall use a *buffer iterator*.
For example,

~~~
// Obtain a writable buffer iterator from the header.
PacketBufferIterator it = header.begin();

uint32_t value = 0x1a23ce2d;
// Write 4 bytes in big-endian order.
WriteB(it, value);

// Write 4 bytes in little-endian order.
WriteL(it, value);

// Write the trailer.
it = trailer.begin();
WriteB(it, value);
~~~

The byte size of a packet can be obtained via the `GetSize()` member
function.
For example,

~~~
size_t byteSize = packet.GetSize();
~~~

To read from a buffer, users shall use a *read-only* buffer iterator.
For example,

~~~
// Obtain a read-only buffer.
ConstPacketBuffer buffer = packet.GetBuffer();

// Obtain a read-only buffer iterator.
ConstPacketBufferIterator cit = buffer.begin();

uint32_t value;
// Read 4 bytes in big-endian order.
ReadB(it, &value);

// Read 4 bytes in little-endian order.
ReadL(it, &value);

// Remove 8 bytes from the start of the packet.
packet.RemoveHeader(8);

// Remove 4 bytes from the end of the packet.
packet.RemoveTrailer(4);
~~~

A packet can be fragmented and reassembled.
For example,

~~~
// Create a packet.
Packet packet;
packet.AddHeader(8);

// Make two fragments of the packet.
Packet frag1 = packet.MakeFragment(0, 4);
Packet frag2 = packet.MakeFragment(4, packet.GetSize());

// Reassemble the packet.
Packet copy1 = frag1;
copy1.AddTrailer(frag2);

// Reassemble the packet, the other way around.
Packet copy2 = frag2;
copy2.AddHeader(frag1);
~~~

#### Tag

Besides a buffer, the other part of a packet is a list of tags.
Tags carry side information that **does not** exists in physical packets.
For example, the sender can add a tag that carries a timestamp, and the
receiver can use the timestamp to calculate the end-to-end delay of the packet.

A tag has an ID, and is associated with a range of bytes of a packet.
When the packet is fragmented, if the fragment contains the associated bytes,
the tag is preserved.
When a packet is reassembled, all tags from the fragments are collectively
restored for the packet.

##### Byte tag

There are two kinds of tags.
The most commonly used type is *byte tag*.
A byte tag has a fixed-size buffer (a sequence of bytes) to carry side
information.
For example,

~~~
/////////////
// The sender
/////////////
// Create a packet.
Packet packet;
packet.AddHeader(4);

// Create a tag buffer to carry a timestamp.
TagBuffer buffer(TimePoint::GetSize());

// Obtain a tag buffer iterator.
TagBufferIterator it = buffer.begin();

// Write the tag buffer.
TimePoint t0 = clock->Now();
WriteB(it, t0);

// Add a byte tag to the packet.
// The byte tag is associated with the current header, i.e., byte 0, 1, 2 and 3.
enum { TAGID_TIMESTAMP = 0 };
packet.AddByteTag(TAGID_TIMESTAMP, buffer, 0, 4);

// Add another header to the packet.
// The byte tag is still associated with the inner header, i.e., byte 4, 5, 6 and 7.
packet.AddHeader(4);

///////////////
// The receiver
///////////////
// Remove the outer header.
packet.RemoveHeader(4);

// Obtain the buffer of the byte tag associated with the byte at specified offset.
size_t offset = 0; // Since the byte tag is associated with byte 0, 1, 2 and 3,
                   // offset can be either 0, 1, 2 or 3.
ConstTagBuffer buffer = packet.GetByteTag(TAGID_TIMESTAMP, offset);

// Obtain a read only tag buffer iterator.
ConstTagBufferIterator it = buffer.cbegin();

// Read the timestamp.
TimePoint t0;
ReadB(it, &t0);

// Calculate the end-to-end delay.
Duration eteDelay = clock->Now() - t0;
~~~

##### Tag ID

Tags **shall** be used for *peer* protocol entities to transfer side information
along with a packet.
An entity shall only manipulate the tags associated with its own protocol
header.
Thus, a tag ID only have local meanings within the same type of entities, and
a tag ID can be reused by different types of entities, since they will not
manipulate tags on other protocols' headers.
For example,

~~~
//////////////////////////
// Protocol at upper layer
//////////////////////////
// Create a packet.
Packet packet;
packet.AddHeader(4);

// Create a tag buffer to carry a timestamp.
TagBuffer buffer(TimePoint::GetSize());

// Obtain a tag buffer iterator.
TagBufferIterator it = buffer.begin();

// Write the tag buffer.
TimePoint t0 = clock->Now();
WriteB(it, t0);

// Add a byte tag to the packet.
// The byte tag is associated with the current header, i.e., byte 0, 1, 2 and 3.
enum { TAGID_TIMESTAMP = 0 };
packet.AddByteTag(TAGID_TIMESTAMP, buffer, 0, 4);

//////////////////////////
// Protocol at lower layer
//////////////////////////
// Add another header to the packet.
packet.AddHeader(4);

// Create a tag buffer to carry a 4-byte value.
TagBuffer buffer(4);

// Obtain a tag buffer iterator.
TagBufferIterator it = buffer.begin();

// Write the tag buffer.
uint32_t value = 0x1a23ce2d
WriteB(it, value);

// Add a byte tag to the packet.
// The tag ID is also 0.
enum { TAGID_VALUE = 0 };
packet.AddByteTag(TAGID_VALUE, buffer, 0, 4);
~~~

It is **not** recommended to use tags to transfer interface control information
(ICI) across protocol layers.
Because these protocols have to agree upon the tag IDs.
And it causes tighter coupling among protocols.

##### Packet tag

The other type of tag is *packet tag*.
A packet tag carries a packet, instead of a byte sequence.
Sometimes, a sender needs to encode a packet, and send the encoded bytes.
The receiver needs to decode the bytes and recover the packet.
However, encoding and decoding are usually complex operations.
In a simulation, one may not have to perform encoding or decoding.
In this case, the original packet can be carried by a packet tag.
For example,
~~~
/////////////
// The sender
/////////////
Packet packet;
packet.AddHeader(8);

// Suppose the encoded content should have 16 bytes.
Packet encoded;
encoded.AddHeader(16);

// Simplify the encoding process by carrying the packet by a tag.
enum { TAGID_RAW_PACKET = 0 };
encoded.AddPacketTag(TAGID_RAW_PACKET, packet, 0, 16);

///////////////
// The receiver
///////////////
// Simplify the decoding process by recovering the packet from a tag.
Packet decoded = encoded.GetPacketTag(TAGID_RAW_PACKET, 0);
~~~

## Random

The library provides several pseudo random number generators, and many typical
random distributions.

### Pseudo random engines

A pseudo random engine provides `IPseudoRandomEngine` interface.
This interface provides `Seed()` method to initialize the engine.
The same seed value generates the same sequence of pseudo random numbers.

Mersenne Twister may be the most commonly used algorithm.

~~~
Ptr<IPseudoRandomEngine> mt19937 = CreateObject<IPseudoRandomEngine>(
        "edu.uestc.nsfx.Mt19937Engine");

uint32_t seed = 0x12345678;
mt19937->Seed(seed);
~~~

The library also provides several high-performance small-state random number
engines.
One recommended engine is perhaps the *xoshiro256+* engine discovered by
David Blackman and Sebastiano Vigna in 2018.
It passes most performance tests.

~~~
Ptr<IPseudoRandomEngine> xoshiro256p = CreateObject<IPseudoRandomEngine>(
        "edu.uestc.nsfx.Xoshiro256Plus01Engine");
~~~

Another recommended engines is perhaps the *xoshiro256*** engine discovered by
David Blackman and Sebastiano Vigna in 2018.
It passes all performance tests, but is slightly slower.

~~~
Ptr<IPseudoRandomEngine> xoshiro256ss = CreateObject<IPseudoRandomEngine>(
        "edu.uestc.nsfx.Xoshiro256StarstarEngine");
~~~

### Random distributions

A pseudo random engine also provides the `IRandom` interface.
This interface provides methods to create typical random distributions.
For example,

~~~
Ptr<IRandom> random(xoshiro256p);

// A uniform random integer within {0, 1, ..., 10}.
Ptr<IUniformUint32Distribution> u32 = random->CreateUniformUint32Distribution(/*min*/0, /*max*/10);
uint32_t u32r = u32->Generate();

// A uniform random integer within {-1, 0, 1, ..., 10}.
Ptr<IUniformInt32Distribution>  i32 = random->CreateUniformInt32Distribution(/*min*/-1, /*max*/10);
int32_t i32r = i32->Generate();

// A uniform random integer within {0, 1, ..., 100}.
Ptr<IUniformUint64Distribution> u64 = random->CreateUniformUint64Distribution(/*min*/0, /*max*/100);
uint64_t u64r = u64->Generate();

// A uniform random integer within {-1, 0, 1, ..., 100}.
Ptr<IUniformInt64Distribution>  i64 = random->CreateUniformInt64Distribution(/*min*/-1, /*max*/100);
int64_t i64r = i64->Generate();

// A uniform double value within [0.0, 1.0).
Ptr<IUniformDoubleDistribution> ud  = random->CreateUniformDoubleDistribution(/*min*/0.0, /*max*/1.0);
double udr = ud->Generate();

// A uniform float value within [0.0, 1.0).
Ptr<IUniformFloatDistribution>  uf  = random->CreateUniformFloatDistribution(/*min*/0.0, /*max*/1.0);
float ufr = uf->Generate();

// The probability of generating true value is 0.9.
Ptr<IBernoulliDistribution>     b   = random->CreateBernoulliDistribution(/*probility*/0.9);
bool br = b->Generate();

// Perform 5 independent trials, and the probability that a trial succeeds is 0.7.
Ptr<IBinomialDistribution>      bi  = CreateBinomialDistribution(/*numTrials*/5, /*prob*/0.7);
uint32_t bir = bi->Generate();  // The number of successful trials.

// The expected number of events is 10.0.
Ptr<IPoissonDistribution>       p   = random->CreatePoissonDistribution(/*mean*/10.0);
uint32_t pr = p->Generate();    // The number of events occurred.

// The average rate of occurrence is 0.1.
Ptr<IExponentialDistribution>   e   = random->CreateExponentialDistribution(/*lambda*/0.1);
double er = e->Generate();

// A standard Normal distribution.
Ptr<INormalDistribution>        n   = random->CreateNormalDistribution(/*mean*/0.0, /*stddev*/1.0);
double nr = n->Generate();
~~~

## Statistics

The library provides tools to collect statistics.
The statistics collection is implemented by the event model.
A component that provides/generates statistics is an event source, while
a component that collects/consumes statistics is an event sink.
Each sample point of a statistic is a *double* value.
Statistics related interfaces and components are defined in
the `nsfx::statistics` namespace.

### Provide statistics

A generator component may provide several statistics.
i.e., load, throughput, delay, et al.
For consumer components to query and obtain the provided statistics,
a generator component **shall** provide the `IProbeContainer` interface.
This interface provides `GetEnumerator()` and `GetProbe()` methods for
querying and obtaining the provided statistics.

To implement the `IProbeContainer` interface, the library provides
the `ProbeContainer` class for *gray-box* reuse.
This class provides `Add()` method to add probes into the container.
For example,

~~~
class StatisticsProvider :
    public ISchedulerUser,
    public IInitializable
{
    typedef StatisticsProvider  ThisClass;

public:
    StatisticsProvider(void) :
        probes_(/* container = */ this),
        generateSampleEventSink_(/* controller = */this, /* o = */this,
                                 /* ptmf = */&ThisClass::OnGenerateSample)
    {}

    virtual ~StatisticsProvider(void) {}

    void Use(Ptr<IScheduler> scheduler)
    {
        scheduler_ = scheduler;
    }

    void Initialize(void)
    {
        // Add a probe.
        sample_ = probes_.GetImpl()->Add("sample size");

        random_ = CreateObject<IRandom>("edu.uestc.nsfx.Xoshiro256Plus01Engine");
        exp_ = random_->CreateExponentialDistribution(/* lambda = */1.0);

        scheduler_->ScheduleIn(Seconds(1),
            generateSampleEventSink_.GetImpl()->GetIntf());
    }

private:
    void OnGenerateSample(void)
    {
        double sample = exp_->Generate();

        // Generate a sample point.
        sample_->Fire(sample);

        scheduler_->ScheduleIn(Seconds(1),
            generateSampleEventSink_.GetImpl()->GetIntf());
    }

    NSFX_INTERFACE_MAP_BEGIN(StatisticsProvider)
        NSFX_INTERFACE_ENTRY(ISchedulerUser)
        NSFX_INTERFACE_ENTRY(IInitializable)
        NSFX_INTERFACE_AGGREGATED_ENTRY(IProbeContainer, &probes_)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<IScheduler> scheduler_;

    Ptr<IRandom> random_;
    Ptr<IExponentialDistribution> exp_;

    MemberAggObject<ProbeContainer> probes_;
    Ptr<Probe> sample_;

    // -> OnGenerateSample().
    MutualObject<MemberFunctionBasedEventSink<IEventSink<>, ThisClass>>
        generateSampleEventSink_;
};

NSFX_REGISTER_CLASS(StatisticsProvider, "edu.uestc.nsfx.tutorial.StatisticsProvider");
~~~

### Collect statistics

A statistics collector can use the `IProbeContainer` interface to obtain
the provided probes.
For example,

~~~
Ptr<IProbeContainer> probes = CreateObject<IProbeContainer>(
        "edu.uestc.nsfx.tutorial.StatisticsProvider");

Ptr<ISchedulerUser>(probes)->Use(scheduler);
Ptr<IInitializable>(probes)->Initialize();

// Obtain a probe.
Ptr<IProbeEvent> sizeProbe = probes->GetProbe("sample size");
// If the probe does not exit, ProbeNotRegistered exception will be thrown.
~~~

When the statistics provider generates a sample point, it fires the probe event
and provides a double value.
The `IProbeEvent` interface provides `Connect()` and `Disconnect()`
methods.

Statistics collectors implement the `IProbeEventSink` interface and connect
to the probe.
For example,

~~~
double totalSize = 0;
Ptr<IProbeEventSink> summation = CreateEventSink<IProbeEventSink>(nullptr, [&] (double v) {
        totalSize += v;
    });

double totalCount = 0;
Ptr<IProbeEventSink> counter = CreateEventSink<IProbeEventSink>(nullptr, [&] (double ) {
        ++totalCount;
    });

sizeProbe->Connect(summation);
sizeProbe->Connect(counter);
~~~

### Summary collector

For simplicity, the library provides the `Summary` component for *black-box*
reuse.
This component provides several commonly used statistics via the `ISummary`
interface.

* `Min()`
* `Max()`
* `Count()`
* `Sum()`
* `Mean()`
* `Stddev()`

`ISummary` also provides the `Reset()` method, which resets the summary to
initial state.

For example,

~~~
Ptr<ISummary> summary = CreateObject<ISummary>("edu.uestc.nsfx.statistics.Summary");
sizeProbe->Connect(summary);

simulator->RunFor(Seconds(100));

// Summary provides several commonly used statistics.
double minValue = summary->Min();
double maxValue = summary->Max();
double count    = summary->Count();
double sum      = summary->Sum();
double mean     = summary->Mean();
double stddev   = summary->Stddev();

// Reset the statistics.
summary->Reset();

simulator->RunFor(Seconds(20));
~~~

## Logging

Logging is to trace the process of an execution.
A log source generates log records, and log sinks process the log records.
A log sink can filter, forward, transform or record the log records.

The library provides tools for logging.
The logging is based upon the event model.

A log source provides the `ILogEvent` interface.
The associated event sink interface is `ILogEvent`.
The prototype of the event callback is `void Fire(LogRecord record)`.

### Record and value

A log record is an instance of the `LogRecord` class.
This is a simple C++ class, which is not a component class.

Essentially, a log record is a set of *named log values*.
A log value is held by an instance of the `LogValue` class.
In a log record, a log value is identified a string name.

* `MakeConstantLogValue()` is a free function template which creates constant
  log values.

* `MakeLogValue()` is a free function template which creates functor-based log
  values.

* `MakeCstrLogValue()` is a free function which creates log values that offer
  C-style strings, i.e., `const char*`.

For example,

~~~
// A constant log value.
LogValue v1 = MakeConstantLogValue<int>(10)
NSFX_TEST_ASSERT(v1.GetTypeId() == boost::typeindex::type_id<int>());
NSFX_TEST_EXPECT_EQ(v1.Get<int>, 10);

// A functor-based log value.
LogValue v2 = MakeLogValue<TimePoint>([&] { return clock->Now(); });
NSFX_TEST_ASSERT(v2.GetTypeId() == boost::typeindex::type_id<TimePoint>());
NSFX_TEST_EXPECT_EQ(v2.Get<TimePoint>, clock->Now());

// A C-style string based log value.
LogValue v3 = MakeCstrLogValue("C string");
NSFX_TEST_ASSERT(v3.GetTypeId() == boost::typeindex::type_id<const char*>());
NSFX_TEST_EXPECT(!std::strncmp(v2.Get<const char*>, "C string", 8));
~~~

The `LogRecord` class provides several member functions.
* `Add()` member function adds an attribute value to a log record.
* `Exists()` member function checks whether an attribute value exists.
* `Get()` member function template obtains an attribute value via its name.

For example,

~~~
// Create an emtpy log record.
LogRecord rec;

// Add log values to the log record.
rec.Add("Level",  v1);
rec.Add("Now",    v2);
rec.Add("String", v3);

// Check the value.
NSFX_TEST_EXPECT(rec.Exists("Level"));
int level = rec.Get<int>("Level");
NSFX_TEST_EXPECT_EQ(level, 10);

NSFX_TEST_EXPECT(rec.Exists("Now"));
TimePoint t0 = rec.Get<TimePoint>("Now");
NSFX_TEST_EXPECT_EQ(t0, clock->Now());

NSFX_TEST_EXPECT(rec.Exists("String"));
const char* str = rec.Get<const char*>("String");
NSFX_TEST_EXPECT(!std::strncmp(str, "C string", 8));
~~~

### Logger

The log library provides the `Logger` component class.
The kind of component can be used as *intermediate* loggers.
That is, it is both a log source and a log sink, which receives log records,
and forwards the log records to downstream log sinks after the log records
have been processed.

The CID of `Logger` is `"edu.uestc.nsfx.Logger"`.
It provides `ILoggerEvent` and `ILogEventSinkEx` interfaces.

`Logger` provides `ILoggerEvent` interface.
Thus, it can forward log records to other log sinks.

It also provides the `ILogEventSinkEx` interface.
`ILogEventSinkEx` extends `ILogEventSink`, which provides extra features.

#### On-demand logging

`ILogEventSinkEx` provides `RegisterSource()` and `UnregisterSource()` methods.

For example, a component may combine/aggregate several inner components.
The inner components provide `ILogEvent`.
The outer component may act as an intermediate logger that receives log records
from the inner components and forwards the records to users' log sinks.
The inner components act as upstream log sources for the outer component.
The outer component does not have to connect to the upstream log sources
directly.

~~~
upstream log source 1 --\
upstream log source 2 ---|-> intermediate logger -> downstream log sink
upstream log source 3 --/
~~~

* `RegisterSource()` is used to register upstream log sources.
  It gives a cookie value, so the log source can be unregistered later.
* `UnregisterSource()` is used to unregister an upstream log source via
  the cookie value.

For example,

~~~
Ptr<ILogEventSinkEx> src1  = CreateObject<ILogEventSinkEx>("edu.uestc.nsfx.Logger");
Ptr<ILogEventSinkEx> src2  = CreateObject<ILogEventSinkEx>("edu.uestc.nsfx.Logger");
Ptr<ILogEventSinkEx> relay = CreateObject<ILogEventSinkEx>("edu.uestc.nsfx.Logger");
Ptr<ILogEventSink>   sink  = CreateEventSink<ILogEventSink>(nullptr, [] (LogRecord rec) {
                                 std::cout << r.Get<std::string>("LogMessage") << std::endl; });

// Register upstream log sources.
cookie_t c1 = relay->RegisterSource(src1);
cookie_t c2 = relay->RegisterSource(src2);

// Connect to a terminal log sink.
cookie_t c0 = Ptr<ILogEvent>(relay)->Connect(sink);

// Do some logging.
NSFX_LOG(src1) << "Info 1";
NSFX_LOG(src2) << "Info 2";

// Make some dynamic adjustment.
relay->UnregisterSource(c1);

// Register a log source, which will be connected to the log sink.
c1 = relay->RegisterSource(src1);

// Disposal.
Ptr<ILogEvent>(relay)->Disconnect(c0);
relay->UnregisterSource(c1);
relay->UnregisterSource(c2);
~~~

Under the hood, the logger does not connect to the register upstream log sources
initially.
The connection is established if and only if the logger has one or more
downstream log sinks.

For efficiency considerations, the `Logger` component only supports
**at most one** log sink.
If more log sinks are required, users can create a log sink that distributes
the log records to multiple log sinks.

`ILogEventSinkEx` also provides a method called `IsEnabled()`.
It returns `true` if and only if the logger has at one or more downstream log
sinks.

When the users are not interested to the log sources, they do not connect any
terminal log sinks at the downstream of the logger.
In such cases, the logger is disabled.

All log macros, such as `NSFX_LOG()`, will not create create log records
if the logger is disabled.
This improves the efficiency of the program when some log sources are not
interested.
Users do not have to modify the source code of the program, in order to avoid
performance degradation due to nonsense logging.

#### Pending values

`ILogEventSinkEx` provides `AddValue()`, `UpdateValue()` and `RemoveValue()`
methods to add pending log values.

Pending log values will be added to each received log record.
So users do not have to manually add named log values to the log records.
For example,

~~~
LogRecord record;
Ptr<ILogEventSink> sink = CreateEventSink<ILogEventSink>(nullptr, [&] (LogRecord r) {
                              record = r; });

// Create a log value.
LogValue v = MakeCstrLogValue("C string");

// Add a pending value to the logger.
sink->AddValue("String", v);

// Do some logging.
NSFX_LOG(sink);

// Check the value.
NSFX_TEST_ASSERT(record.Exists("String"));
const char* str = record.Get<const char*>("String");
NSFX_TEST_EXPECT_EQ(str, std::string("C string"));
~~~

To add more dynamic log values, such as timestamps, *second-order* log values
shall be used.
A second-order log value is a log value that stores a log value.
When a pending second-order log value is added, the `Logger` will recognize them.
For example,

~~~
Ptr<IClock> clock(simulator);
TimePoint t0(Seconds(1));
TimePoint t1(Seconds(2));

LogRecord record;
Ptr<ILogEventSink> sink = CreateEventSink<ILogEventSink>(nullptr, [&] (LogRecord r) {
                              record = r; });

// Create second-order log value to generate timestamps.
LogValue ts = MakeLogValue<LogValue>([&] {
                  MakeConstantLogValue<TimePoint>(clock->Now()); });

// Add the pending value to the logger.
sink->AddValue("Timestamp", ts);

// Do some logging at t0.
simulator->RunUntil(t0);
NSFX_LOG(sink);

// Examined the timestamp at t1.
simulator->RunUntil(t1);
TimePoint timestamp = record.Get<TimePoint>("Timestamp");
NSFX_TEST_EXPECT_EQ(timestamp, t0);
~~~

#### Filter

`ILogEventSinkEx` provides `SetFilter()` method.
Users can specify a filter for the logger.
A log filter examines a log record, and gives a decision.

A log filter is a components that provides `ILogFilter` interface.
The free function template `CreateLogFilter()` can be used to create
a functor-based log filter.
The functor accepts a log record, and returns `LOG_ACCEPT` or `LOG_DISCARD`.
For example,

~~~
Ptr<ILogEventSinkEx> logger = CreateObject<ILogEventSinkEx>("edu.uestc.nsfx.Logger");

// Create a log filter that only accepts positive levels.
Ptr<ILogFilter> filter = CreateLogFilter([] (const LogRecord& r) {
                             return (r.Exists("Level") && r.Get<int>("Level") > 0) ?
                                    LOG_ACCEPT : LOG_DISCARD;
                         });

logger->SetFilter(filter);

// Add a pending value.
logger->AddValue("Level", MakeConstantLogValue<int>(1));

// The logger outputs the log record, since the level is positive.
NSFX_LOG(logger) << "Accept";

// Update the pending value.
logger->UpdateValue("Level", MakeConstantLogValue<int>(0));

// The logger discards the log record, since the level is 0.
NSFX_LOG(logger) << "Discard";

// Remove the log filter.
logger->SetFilter(nullptr);

// The logger outputs the log record, since the filter is removed.
NSFX_LOG(logger) << "Accept";
~~~

### Stream sink

Terminal log sinks are the end points of log records.
The library provides `LogStreamSink` component class.
The CID is `"edu.uestc.nsfx.LogStreamSink"`.
This kind of log sinks are stream-based terminal log sinks.
The component provides the `ILogStreamSink` interface, which extends
`ILogEventSink`.
It does not provide `ILogEvent`.

`ILogEventSink` supports pending log value and log filter.
That is, the interface also provides `AddValue()`, `UpdateValue()`,
`RemoveValue()` and `SetFilter()` methods.

#### Destination

The `ILogStreamSink` provides `AddStream()` and `AddFile()` methods to specify
output destinations.
For example,

~~~
Ptr<ILogStreamSink> sink = CreateObject<ILogStreamSink>("edu.uestc.nsfx.LogStreamSink");

// Output to standard output stream.
sink->AddStream(std::cout);

// Output to a file.
sink->AddFile("simulation.log");
~~~

#### Formatter

Since a log record can carry arbitrary log values, `LogStreamSink` does not
know how to format and output a log record.
The stream sink depends upon a formatter to work.

A log formatter provides the `ILogFormatter` interface.
The free function template `CreateLogFormatter()` can be used to create
a functor-based log formatter.
For example,

~~~
void Format(std::ostream& os, const LogRecord& r)
{
    os << "The level is " << r->Get<int>("Level");
}

// Create a log formatter.
Ptr<ILogFormatter> fmtr = CreateLogFormatter(&Format);

Ptr<ILogFormatterUser>(sink)->Use(fmtr);

// Add a pending value.
sink->AddValue("Level", 1);

// Add an output stream.
std::ostringstream oss;
sink->AddStream(oss);

// Do some logging.
NSFX_LOG(logger) << "Hello world!";

// Check the formatted output.
NSFX_TEST_EXPECT_EQ(oss.str(), "The level is 1");
~~~

### Tools

The library provides several tools to ease the use of logging.

#### Severity levels

The library defines several severity levels for log records.
The following severity levels are defined in the enumeration `LogSeverity`.

* `LOG_FATAL`: the program is corrupted, and forced to terminate.
* `LOG_ERROR`: the program cannot resolve an error without user's intervention.
* `LOG_WARN` : the program is recovered from a potentially erroneous state, and
  not guaranteed to work as expected.
* `LOG_INFO` : general information about the state of the program.
* `LOG_DEBUG`: information that is diagnostically helpful.
* `LOG_TRACE`: detailed information about the performed operations.

#### Macros

The library provides several macros for creating and committing log records.

* `NSFX_LOG(logger)`      : log plainly.
* `NSFX_LOG_FATAL(logger)`: log with a severity level.
* `NSFX_LOG_ERROR(logger)`: log with a severity level.
* `NSFX_LOG_WARN(logger)` : log with a severity level.
* `NSFX_LOG_INFO(logger)` : log with a severity level.
* `NSFX_LOG_DEBUG(logger)`: log with a severity level.
* `NSFX_LOG_TRACE(logger)`: log with a severity level.

If the logger is `Ptr<ILogEventSinkEx>`, the enabled/disabled state of
the logger is examined before a log record is created.

Otherwise, the logger is `Ptr<ILogEventSink>`, it is assumed that the logger
is always enabled, and a log record is created and committed to the logger.

Each macro accepts stream input operators to generate a textual message.
For example,

~~~
NSFX_LOG_DEBUG(logger) << "A packet has been sent.";
~~~

The log records created by the macros carry the following named log values:

* `"LogMessage"` : `const char*`
* `"LogSeverity"`: `LogSeverity`
* `"LogFunction"`: `const char*` (if `NSFX_LOG_FUNCTION_NAME` is defined)
* `"LogFile"`    : `const char*` (if `NSFX_LOG_FILE_NAME` is defined)
* `"LogLine"`    : `uint32_t`    (if `NSFX_LOG_LINE_NUMBER` is defined)


/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-11
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_EVENT_SINK_H__CE5ABEF5_7C7B_4D87_ABA6_27E053F3A8F1
#define I_EVENT_SINK_H__CE5ABEF5_7C7B_4D87_ABA6_27E053F3A8F1


#include <nsfx/component/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/exception.h>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/tuple/enum.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_function.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 * @brief Define a custom event sink interface that derives from \c IEventSink.
 *
 * @param ISink The name of the interface.
 * @param Proto The prototype of the event callback.<br/>
 *              It <b>must</b> be placed within parenthesis.<br/>
 *
 * The event sink interface conforms to \c IEventSinkConcept.
 *
 * @remarks The \c Proto must be placed <b>within parenthesis</b>, so commas
 *          can be used in the \c Proto.<br/>
 *          For example,
 *          @code
 *          NSFX_DEFINE_EVENT_SINK_INTERFACE(IMyEventSink, ( char(short, int) ));
 *          @endcode
 */
#define NSFX_DEFINE_EVENT_SINK_INTERFACE(ISink, Proto)         \
    class ISink :                                              \
        public ::nsfx::IEventSink<BOOST_PP_TUPLE_ENUM(Proto)>  \
    {                                                          \
    public:                                                    \
        virtual ~ISink(void) BOOST_NOEXCEPT {}                 \
    }


////////////////////////////////////////////////////////////////////////////////
// IEventSink.
/**
 * @ingroup Component
 * @brief The event sink class template.
 *
 * @tparam Proto The prototype of a callable object that looks like
 *               <code>Ret(A0, A1, ...)</code>.
 *
 * Users shall <b>derive</b> their own event sink interfaces from this class
 * template, and associate the interfaces with UUIDs.<br/>
 * Users shall not use multiple inheritance to derive their own event sink
 * interfaces.<br/>
 *
 * An event sink interface has a nested \c Prototype that can be used to choose
 * the correct template specialization.<br/>
 *
 * ## Event interface definition and implementation.
 *    The library provides several tools for event sink interface definition
 *    and implementation.<br/>
 *
 * ### 1. Define an event sink interface.
 *     The macro \c NSFX_DEFINE_EVENT_SINK_INTERFACE() is provided.<br/>
 *
 * ### 2. Implement an event sink interface.
 *     The library provides three tiers of tools.
 *
 * #### 2.1 Class templates.
 *      The library provides three class templates that implements \c IEventSink:
 *      \c FunctorBasedEventSink, \c FunctionPointerBasedEventSink and
 *      \c MemberFunctionBasedEventSink.
 *
 *      Users can specialize these class templates to fit their own needs.<br/>
 *      The specialized templates conform to \c EnvelopableConcept, thus they
 *      can be enveloped in \c Object and \c AggObject.
 *
 *      For example
 *      @code
 *      NSFX_DEFINE_EVENT_SINK_INTERFACE(IMyEventSink, ( char(short, int) ));
 *
 *      // Functor based event sink.
 *      struct Functor
 *      {
 *          char operator()(short, int)
 *          {
 *              return '0';
 *          }
 *      };
 *      typedef FunctorBasedEventSink<IMyEventSink, Functor>  MyEventSink1;
 *      typedef Object<MyEventSink1>     MyEventSinkType1;
 *      Ptr<IMyEventSink> s1(new MyEventSinkType1(nullptr, Functor()));
 *
 *      // Function pointer based event sink.
 *      char Bar(short, int)
 *      {
 *          return '0';
 *      }
 *      typedef FunctionPointerBasedEventSink<IMyEventSink>  MyEventSink2;
 *      typedef Object<MyEventSink2>     MyEventSinkType2;
 *      Ptr<IMyEventSink> s2(new MyEventSinkType2(nullptr, &Bar));
 *
 *      // Member function based event sink.
 *      struct Obj
 *      {
 *          char Qux(short, int)
 *          {
 *              return '0';
 *          }
 *      };
 *      static Obj o;
 *      typedef MemberFunctionBasedEventSink<IMyEventSink>  MyEventSink3;
 *      typedef Object<MyEventSink3>     MyEventSinkType3;
 *      Ptr<IMyEventSink> s3(new MyEventSinkType3(nullptr, &o, &Obj::Qux));
 *      @endcode
 *
 * #### 2.2 Maker class.
 *      The library provides \c EventSinkCreator class template.<br/>
 *      The class template supplies three overloaded function call operators
 *      that create event sink objects.<br/>
 *
 *      For example
 *      @code
 *      NSFX_DEFINE_EVENT_SINK_INTERFACE(IMyEventSink, ( char(short, int) ));
 *
 *      // Functor based event sink.
 *      Ptr<IMyEventSink> s1 = EventSinkCreator<IMyEventSink>()(nullptr,
 *                              [] (short, int) { return '0'; });
 *
 *      // Function pointer based event sink.
 *      char Bar(short, int)
 *      {
 *          return '0';
 *      }
 *      Ptr<IMyEventSink> s2 = EventSinkCreator<IMyEventSink>()(nullptr, &Bar);
 *
 *      // Member function based event sink.
 *      struct Obj
 *      {
 *          char Qux(short, int)
 *          {
 *              return '0';
 *          }
 *      };
 *      static Obj o;
 *      Ptr<IMyEventSink> s3 = EventSinkCreator<IMyEventSink>()(nullptr,
 *                              &f, &Functor::Foo));
 *      @endcode
 *
 * #### 2.3 Maker function.
 *      The library provides \c CreateEventSink() function template.<br/>
 *      The function template supplies three overloaded functions that create
 *      event sink objects.<br/>
 *
 *      For example
 *      @code
 *      NSFX_DEFINE_EVENT_SINK_INTERFACE(IMyEventSink, ( char(short, int) ));
 *
 *      // Functor based event sink.
 *      Ptr<IMyEventSink> s1 = CreateEventSink<IMyEventSink>(nullptr,
 *                              [] (short, int) { return '0'; });
 *
 *      // Function pointer based event sink.
 *      char Bar(short, int)
 *      {
 *          return '0';
 *      }
 *      Ptr<IMyEventSink> s2 = CreateEventSink<IMyEventSink>(nullptr, &Bar);
 *
 *      // Member function based event sink.
 *      struct Obj
 *      {
 *          char Qux(short, int)
 *          {
 *              return '0';
 *          }
 *      };
 *      static Obj o;
 *      Ptr<IMyEventSink> s3 = CreateEventSink<IMyEventSink>(nullptr,
 *                              &f, &Functor::Foo));
 *      @endcode
 */
template<class Proto = void(void)>
class IEventSink :
    virtual public IObject
{
public:
    typedef void(Prototype)(void);

    virtual ~IEventSink(void) BOOST_NOEXCEPT {}

    virtual void Fire(void) = 0;
};


////////////////////////////////////////
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

template<class Ret, class... Args>
class IEventSink<Ret(Args...)> :
    virtual public IObject
{
public:
    typedef Ret(Prototype)(Args...);

    virtual ~IEventSink(void) BOOST_NOEXCEPT {}

    virtual Ret Fire(Args... args) = 0;
};


////////////////////////////////////////
#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

#define BOOST_PP_ITERATION_PARAMS_1  \
    (3, (0, NSFX_MAX_ARITY, <nsfx/component/i-event-sink.h>))

#include BOOST_PP_ITERATE()

#endif // defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)


////////////////////////////////////////////////////////////////////////////////
// IEventSinkConcept.
/**
 * @ingroup Component
 * @brief The event sink interface concept.
 *
 * A class is an event sink if it satisfies the following conditions.<br/>
 * 1. It is derived from <code>IEventSink<></code> class template.<br/>
 * 2. It is has nested type \c Prototype that is a function type.<br/>
 * 3. It conforms to \c HasUuidConcept.<br/>
 *
 * Since <code>IEventSink<></code> class template is derived from \c IObject,
 * the class also conforms to \c IObjectConcept.
 */
template<class ISink>
class IEventSinkConcept
{
public:
    BOOST_CONCEPT_ASSERT((HasUuidConcept<ISink>));

    // Has a nested type: Prototype.
    typedef char yes[1];
    typedef char no[2];

    template<class T>
    static yes& HasPrototype(typename T::Prototype*);
    template<class >
    static no&  HasPrototype(...);
    BOOST_STATIC_CONSTANT(bool, hasPrototype =
                          sizeof (HasPrototype<ISink>(nullptr)) == sizeof (yes));

    static_assert(hasPrototype,
                  "The event sink interface does not have a nested 'Prototype'.");

    static_assert(boost::is_function<typename ISink::Prototype>::value,
                  "The event sink interface has an invalid nested 'Prototype' "
                  "which should be a function type.");

    // Derived from IEventSink<> class template.
    typedef IEventSink<typename ISink::Prototype>  BaseType;
    static_assert(boost::is_base_of<BaseType, ISink>::value,
                  "The event sink interface is not derived from IEventSink<> "
                  "class template.");

};


NSFX_CLOSE_NAMESPACE


#endif // I_EVENT_SINK_H__CE5ABEF5_7C7B_4D87_ABA6_27E053F3A8F1


////////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_PP_IS_ITERATING) /*{{{*/

// template<class Ret, class A0, class A1, ...>
// class IEventSink<Ret(A0, A1, ...)>
template<class Ret  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())  BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
class IEventSink<Ret(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A))> :
    virtual public IObject
{
public:
    // typedef Ret(Prototype)(A0, A1, ...);
    typedef Ret(Prototype)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A));

    virtual ~IEventSink(void) BOOST_NOEXCEPT {}

    // virtual Ret Fire(A0 a0, A1 a1, ...) = 0;
    virtual Ret Fire(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, a)) = 0;
};


#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/


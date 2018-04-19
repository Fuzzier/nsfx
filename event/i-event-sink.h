/**
 * @file
 *
 * @brief Event support for Network Simulation Frameworks.
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


#include <nsfx/event/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/exception.h>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/tuple/enum.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_function.hpp>
#include <boost/concept_check.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Event
 * @brief Define a custom event sink interface that derives from \c IEventSink.
 *
 * @param ISink The name of the user-defined event sink interface.
 * @param iid   The UID of the user-defined event sink interface.
 * @param Proto The prototype of the event callback.<br/>
 *              It <b>must</b> be placed within parenthesis.<br/>
 *
 * The event sink interface conforms to \c IEventSinkConcept.
 *
 * @remarks \c Proto must be placed <b>within parenthesis</b>, so commas can be
 *          used in the \c Proto.<br/>
 *          For example
 *          @code
 *          NSFX_DEFINE_EVENT_SINK_INTERFACE(
 *              IMyEventSink, "edu.uestc.nsfx.example.IMyEventSink",
 *              ( char(short, int) ));
 *          @endcode
 */
#define NSFX_DEFINE_EVENT_SINK_INTERFACE(ISink, iid, Proto)    \
    NSFX_DEFINE_CLASS_UID(class ISink, iid);                   \
    class ISink :                                              \
        public ::nsfx::IEventSink<BOOST_PP_TUPLE_ENUM(Proto)>  \
    {                                                          \
    public:                                                    \
        virtual ~ISink(void) BOOST_NOEXCEPT {}                 \
    }


////////////////////////////////////////////////////////////////////////////////
// IEventSink.
/**
 * @ingroup Event
 * @brief The event sink class template.
 *
 * @tparam Proto The prototype of a callable object that looks like
 *               \c Ret(A0, A1, ...).
 *
 * The responsibility of an event sink interface is to hold a callback that can
 * be invoked by an event source when the event is fired.<br/>
 *
 * An event sink interface has a single method <code>Ret Fire(A0, A1, ...)</code>.<br/>
 * It also has a nested \c Prototype that is the function type
 * <code>Ret(A0, A1, ...)</code>, and can be used to choose the correct template
 * specialization.<br/>
 *
 * <code>IEventSink<void(void)></code> is the <b>only</b> event sink interface
 * that has an associated UID defined by the library.<br/>
 * Users shall <b>derive</b> their own event sink interfaces from this class
 * template, and associate the interfaces with UIDs to enable <code>Ptr<></code>
 * to manage their pointers.<br/>
 * Users shall not use multiple inheritance to derive their own event sink
 * interfaces.<br/>
 *
 * @code
 * Proto IEventSink<> ---> user-defined event sink interface
 *                           |
 *                           V
 *                         EventSinkCreator<>, CreateEventSink<>()
 * @endcode
 *
 * ## Event sink interface definition and implementation.
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
 *      #include <nsfx/event/event-sink.h>
 *
 *      // Define an event sink interface.
 *      NSFX_DEFINE_EVENT_SINK_INTERFACE(
 *          IMyEventSink, "edu.uestc.nsfx.example.IMyEventSink"
 *          ( char(short, int) ));
 *
 *      // Create event sink object:
 *      // Functor based event sink.
 *      struct Functor
 *      {
 *          char operator()(short, int)
 *          {
 *              return '0';
 *          }
 *      };
 *      typedef FunctorBasedEventSink<IMyEventSink, Functor>  MyEventSink1;
 *      typedef Object<MyEventSink1>     MyEventSinkClass1;
 *      Ptr<IMyEventSink> s1(new MyEventSinkClass1(nullptr, Functor()));
 *
 *      // Function pointer based event sink.
 *      char Bar(short, int)
 *      {
 *          return '0';
 *      }
 *      typedef FunctionPointerBasedEventSink<IMyEventSink>  MyEventSink2;
 *      typedef Object<MyEventSink2>     MyEventSinkClass2;
 *      Ptr<IMyEventSink> s2(new MyEventSinkClass2(nullptr, &Bar));
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
 *      typedef MemberFunctionBasedEventSink<IMyEventSink, Obj>  MyEventSink3;
 *      typedef Object<MyEventSink3>     MyEventSinkClass3;
 *      Ptr<IMyEventSink> s3(new MyEventSinkClass3(nullptr, &o, &Obj::Qux));
 *      @endcode
 *
 * #### 2.2 Maker class.
 *      The library provides \c EventSinkCreator class template.<br/>
 *      The class template supplies three overloaded function call operators
 *      that create event sink objects.<br/>
 *
 *      For example
 *      @code
 *      // Functor based event sink.
 *      Ptr<IMyEventSink> s4 = EventSinkCreator<IMyEventSink>()(nullptr,
 *                              [] (short, int) { return '0'; });
 *
 *      // Function pointer based event sink.
 *      char Bar(short, int)
 *      {
 *          return '0';
 *      }
 *      Ptr<IMyEventSink> s5 = EventSinkCreator<IMyEventSink>()(nullptr, &Bar);
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
 *      Ptr<IMyEventSink> s6 = EventSinkCreator<IMyEventSink>()(nullptr,
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
 *      // Functor based event sink.
 *      Ptr<IMyEventSink> s7 = CreateEventSink<IMyEventSink>(nullptr,
 *                              [] (short, int) { return '0'; });
 *
 *      // Function pointer based event sink.
 *      char Bar(short, int)
 *      {
 *          return '0';
 *      }
 *      Ptr<IMyEventSink> s8 = CreateEventSink<IMyEventSink>(nullptr, &Bar);
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
 *      Ptr<IMyEventSink> s9 = CreateEventSink<IMyEventSink>(nullptr,
 *                              &f, &Functor::Foo));
 *      @endcode
 */
template<class Proto = typename boost::remove_pointer<void(void)>::type>
class IEventSink;


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 * @brief Define the UID of <code>IEventSink<></code>.
 */
NSFX_DEFINE_CLASS_UID(IEventSink<>, "edu.uestc.nsfx.IEventSink");


////////////////////////////////////////
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

/**
 * @ingroup Event
 * @brief The event sink interface (specialized class template).
 *
 * @tparam Ret  The return type of the callback method of the event sink.
 * @tparam Args The types of arguments of the callback method of the event sink.
 *
 * This class template provides template argument deduction the primary class
 * template <code>IEventSink<></code>.
 *
 * @see <code>IEventSink<></code> (the primary class template)
 */
template<class Ret, class... Args>
class IEventSink<Ret(Args...)> :
    virtual public IObject
{
public:
    typedef Ret(Prototype)(Args...);

    virtual ~IEventSink(void) BOOST_NOEXCEPT {}

    /**
     * @brief The callback method of the event sink.
     */
    virtual Ret Fire(Args... args) = 0;
};


////////////////////////////////////////
#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

#define BOOST_PP_ITERATION_PARAMS_1  \
    (3, (0, NSFX_MAX_ARITY, <nsfx/event/i-event-sink.h>))

#include BOOST_PP_ITERATE()

#endif // defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)


////////////////////////////////////////////////////////////////////////////////
// IEventSinkConcept.
/**
 * @ingroup Event
 * @brief The event sink interface concept.
 *
 * A class is an event sink if it satisfies the following conditions.<br/>
 * 1. It is <code>IEventSink<void(void)></code> or it is derived from
 *    <code>IEventSink<></code> class template.<br/>
 * 2. It is has nested type \c Prototype that is a function type.<br/>
 * 3. It conforms to \c HasUidConcept.<br/>
 *
 * Since <code>IEventSink<></code> class template is derived from \c IObject,
 * the class also conforms to \c IObjectConcept.
 */
template<class ISink>
struct IEventSinkConcept
{
    BOOST_CONCEPT_ASSERT((HasUidConcept<ISink>));

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
                  "The type does not conform to IEventSinkConcept, since it "
                  "does not have a nested 'Prototype'.");

    // The nested Prototype is a function type.
    static_assert(boost::is_function<typename ISink::Prototype>::value,
                  "The type does not conform to IEventSinkConcept, since it has "
                  "an invalid nested 'Prototype' that is not a function type.");

    // Is IEventSink<void(void)> or derived from IEventSink<> class template.
    typedef IEventSink<typename ISink::Prototype>  BaseType;
    static_assert(
        boost::is_same<IEventSink<>, ISink>::value ||
        boost::is_base_of<BaseType, ISink>::value,
        "The type does not conform to IEventSinkConcept since, it is neither "
        "IEventSink<void(void)>, nor derived from IEventSink<> class template.");
};


/**
 * @ingroup Event
 * @brief The event sink interface with prototype concept.
 *
 * 1. The \c ISink confoms to \c IEventSinkConcept.<br/>
 * 2. The nested \c Prototype of \c ISink is the same type as \c Proto.<br/>
 */
template<class ISink, class Proto>
struct IEventSinkPrototypeConcept
{
    BOOST_CONCEPT_ASSERT((IEventSinkConcept<ISink>));

    static_assert(boost::is_same<typename ISink::Prototype, Proto>::value,
                  "The type does not conform to IEventSinkPrototypeConcept "
                  "since it has a matching 'Prototype'.");
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


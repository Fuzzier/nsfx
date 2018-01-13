/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-12
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef EVENT_SINK_CREATOR_H__816486D9_8AE8_4FE9_BFA3_5D2B657E9D64
#define EVENT_SINK_CREATOR_H__816486D9_8AE8_4FE9_BFA3_5D2B657E9D64


#include <nsfx/component/config.h>
#include <nsfx/component/i-event-sink.h>
#include <nsfx/component/object.h>
#include <nsfx/component/ptr.h>
#include <nsfx/component/exception.h>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
/**
 * @ingroup Component
 * @brief Implement event sink interface by a funtor.
 *
 * @tparam ISink The type of an event sink interface that conforms to
 *               \c IEventSinkConcept.
 * @tparam F     The type of a functor.
 */
template<class ISink, class F, class Proto = typename ISink::Prototype>
class FunctorBasedEventSink;

/**
 * @ingroup Component
 * @brief Implement event sink interface by a function pointer.
 *
 * @tparam ISink The type of an event sink interface that conforms to
 *               \c IEventSinkConcept.
 */
template<class ISink, class Proto = typename ISink::Prototype>
class FunctionPointerBasedEventSink;

/**
 * @ingroup Component
 * @brief Implement event sink interface by an object's member function.
 *
 * @tparam ISink The type of an event sink interface that conforms to
 *               \c IEventSinkConcept.
 * @tparam O     The type of an object.
 */
template<class ISink, class O, class Proto = typename ISink::Prototype>
class MemberFunctionBasedEventSink;


////////////////////////////////////////
/**
 * @ingroup Component
 * @brief The event sink maker.
 *
 * @tparam ISink A user-defined event sink interface that conforms to
 *               \c IEventSinkConcept.
 * @tparam Proto The prototype of the event callback method that looks like
 *               <code>Ret(A0, A1, ...)</code>.
 *
 * @see \c IEventSink,
 *      \c NSFX_DEFINE_EVENT_SINK_INTERFACE().
 *
 * @example For example
 *   @code
 *   // Define a custom event sink interface.
 *   NSFX_DEFINE_EVENT_SINK_INTERFACE(IMyEventSink, ( char(short, int) ));
 *
 *   // Associate the interface with a UUID.
 *   NSFX_DEFINE_CLASS_UUID4(IMyEventSink, 0x3DD34BEB, 0x2F32, 0x410B, 0x9BAD92146CBAE911LL);
 *
 *   ////////// Use EventSinkCreator<> class template. //////////
 *   // Via a lambda expression (a functor).
 *   Ptr<IMyEventSink> s1 = EventSinkCreator<IMyEventSink>()(
 *                              [] (short, int) -> char { return '0'; });
 *
 *   // Via a function pointer.
 *   char Foo(short, int);
 *   Ptr<IMyEventSink> s2 = EventSinkCreator<IMyEventSink>()(&Foo);
 *
 *   // Via an object and a pointer to its member function.
 *   struct Sink { char Foo(short, int); };
 *   Sink s;
 *   Ptr<IMyEventSink> s3 = EventSinkCreator<IMyEventSink>()(&s, &Sink::Foo);
 *
 *   ////////// Use CreateEventSink<> function template. //////////
 *   // Via a lambda expression (a functor).
 *   Ptr<IMyEventSink> s4 = CreateEventSink<IMyEventSink>(
 *                              [] (short, int) -> char { return '0'; });
 *
 *   // Via a function pointer.
 *   Ptr<IMyEventSink> s5 = CreateEventSink<IMyEventSink>(&Foo);
 *
 *   // Via an object and a pointer to its member function.
 *   Ptr<IMyEventSink> s6 = CreateEventSink<IMyEventSink>(&s, &Sink::Foo);
 *   @endcode
 */
template<class ISink, class Proto = typename ISink::Prototype>
class EventSinkCreator;


////////////////////////////////////////////////////////////////////////////////
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

template<class ISink, class F, class Ret, class... Args>
class FunctorBasedEventSink<ISink, F, Ret(Args...)> : public ISink/*{{{*/
{
    BOOST_CONCEPT_ASSERT((IEventSinkConcept<ISink>));

public:
    explicit FunctorBasedEventSink(typename boost::remove_reference<F>::type const& f) :
        f_(f)
    {}

    explicit FunctorBasedEventSink(typename boost::remove_reference<F>::type && f) :
        f_(std::move(f))
    {}

    virtual ~FunctorBasedEventSink(void) {}

    virtual Ret Fire(Args... args) NSFX_OVERRIDE
    {
        return f_(std::forward<Args>(args)...);
    }

    NSFX_INTERFACE_MAP_BEGIN(FunctorBasedEventSink)
        NSFX_INTERFACE_ENTRY(ISink)
    NSFX_INTERFACE_MAP_END()

private:
    F f_;
};/*}}}*/

template<class ISink, class Ret, class... Args>
class FunctionPointerBasedEventSink<ISink, Ret(Args...)> : public ISink/*{{{*/
{
    BOOST_CONCEPT_ASSERT((IEventSinkConcept<ISink>));

public:
    explicit FunctionPointerBasedEventSink(Ret(* fn)(Args...)) :
        fn_(fn)
    {
        if (!fn)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
    }

    virtual ~FunctionPointerBasedEventSink(void) BOOST_NOEXCEPT {}

    virtual Ret Fire(Args... args) NSFX_OVERRIDE
    {
        return (*fn_)(std::forward<Args>(args)...);
    }

    NSFX_INTERFACE_MAP_BEGIN(FunctionPointerBasedEventSink)
        NSFX_INTERFACE_ENTRY(ISink)
    NSFX_INTERFACE_MAP_END()

private:
    Ret(* fn_)(Args...);
};/*}}}*/

template<class ISink, class O, class Ret, class... Args>
class MemberFunctionBasedEventSink<ISink, O, Ret(Args...)> : public ISink/*{{{*/
{
    BOOST_CONCEPT_ASSERT((IEventSinkConcept<ISink>));

public:
    MemberFunctionBasedEventSink(O* o, Ret(O::* ptmf)(Args...)) :
        o_(o),
        ptmf_(ptmf)
    {
        if (!o || !ptmf)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
    }

    virtual ~MemberFunctionBasedEventSink(void) BOOST_NOEXCEPT {}

    virtual Ret Fire(Args... args) NSFX_OVERRIDE
    {
        return (o_->*ptmf_)(std::forward<Args>(args)...);
    }

    NSFX_INTERFACE_MAP_BEGIN(MemberFunctionBasedEventSink)
        NSFX_INTERFACE_ENTRY(ISink)
    NSFX_INTERFACE_MAP_END()

private:
    O* o_;
    Ret(O::* ptmf_)(Args...);
};/*}}}*/


////////////////////////////////////////
template<class ISink, class Ret, class... Args>
class EventSinkCreator<ISink, Ret(Args...)>/*{{{*/
{
    BOOST_CONCEPT_ASSERT((IEventSinkConcept<ISink>));

public:
    // For functors.
    template<class F>
    Ptr<ISink> operator()(IObject* controller, F&& f) const
    {
        typedef FunctorBasedEventSink<ISink, F>  EventSink;
        typedef Object<EventSink>     EventSinkType;
        typedef AggObject<EventSink>  AggEventSinkType;
        return !controller
            ? static_cast<ISink*>(new EventSinkType(std::forward<F>(f)))
            : Ptr<IObject>(new AggEventSinkType(controller, std::forward<F>(f)));
    }

    // For funtion pointers.
    template<class Ret, class... Args>
    Ptr<ISink> operator()(IObject* controller, Ret(* fn)(Args...)) const
    {
        typedef FunctionPointerBasedEventSink<ISink>  EventSink;
        typedef Object<EventSink>     EventSinkType;
        typedef AggObject<EventSink>  AggEventSinkType;
        return !controller
            ? static_cast<ISink*>(new EventSinkType(fn))
            : Ptr<IObject>(new AggEventSinkType(controller, fn));
    }

    // For objects and pointers to member funtions.
    template<class O, class Ret, class... Args>
    Ptr<ISink> operator()(IObject* controller, O* o, Ret(O::* ptmf)(Args...)) const
    {
        typedef MemberFunctionBasedEventSink<ISink, O>  EventSink;
        typedef Object<EventSink>     EventSinkType;
        typedef AggObject<EventSink>  AggEventSinkType;
        return !controller
            ? static_cast<ISink*>(new EventSinkType(o, ptmf))
            : Ptr<IObject>(new AggEventSinkType(controller, o, ptmf));
    }

};/*}}}*/

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)


////////////////////////////////////////////////////////////////////////////////
// Free functions.
/**
 * @ingroup Component
 * @brief Make an event sink via a functor.
 *
 * @tparam ISink A user-defined event sink interface that conforms to
 *               \c IEventSinkConcept.
 * @tparam F     The type of the functor.
 *
 * @param[in] f  A functor.
 */
template<class ISink, class F>
inline Ptr<ISink> CreateEventSink(IObject* controller, F&& f)
{
    return EventSinkCreator<ISink>()(controller, std::forward<F>(f));
}


#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

/**
 * @ingroup Component
 * @brief Make an event sink via a function pointer.
 *
 * @tparam ISink A user-defined event sink interface that conforms to
 *               \c IEventSinkConcept.
 *
 * @param[in] fn The function pointer.
 */
template<class ISink, class Ret, class... Args>
inline Ptr<ISink> CreateEventSink(IObject* controller, Ret(* fn)(Args...))
{
    return EventSinkCreator<ISink>()(controller, fn);
}

/**
 * @ingroup Component
 * @brief Make an event sink via an object and a pointer to its member function.
 *
 * @tparam ISink A user-defined event sink interface that conforms to
 *               \c IEventSinkConcept.
 *
 * @param[in] fn The function pointer.
 */
template<class ISink, class O, class Ret, class... Args>
inline Ptr<ISink> CreateEventSink(IObject* controller, O* o, Ret(O::* ptmf)(Args...))
{
    return EventSinkCreator<ISink>()(controller, o, ptmf);
}


////////////////////////////////////////
#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

#define BOOST_PP_ITERATION_PARAMS_1  (3, (0, NSFX_MAX_ARITY, <nsfx/component/event-sink-creator.h>))

#include BOOST_PP_ITERATE()

#endif // defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)


NSFX_CLOSE_NAMESPACE


#endif // EVENT_SINK_CREATOR_H__816486D9_8AE8_4FE9_BFA3_5D2B657E9D64


////////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_PP_IS_ITERATING)/*{{{*/

#define NSFX_PP_FORWARD(z, n, d)  std::forward<A ## n>(a ## n)

////////////////////////////////////////
// template<class ISink, class F, class Ret, class A0, class A1, ...>
// class FunctorBasedEventSink<ISink, F, Ret(A0, A1, ...)>
template<class ISink, class F, class Ret  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                               BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
class FunctorBasedEventSink<ISink, F, Ret(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A))> :/*{{{*/
    public ISink
{
    BOOST_CONCEPT_ASSERT((IEventSinkConcept<ISink>));

public:
    explicit FunctorBasedEventSink(typename boost::remove_reference<F>::type const& f) :
        f_(f)
    {}

    explicit FunctorBasedEventSink(typename boost::remove_reference<F>::type && f) :
        f_(std::move(f))
    {}

    virtual ~FunctorBasedEventSink(void) {}

    // virtual Ret Fire(A0 a0, A1 a1, ...) NSFX_OVERRIDE
    virtual Ret Fire(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, a)) NSFX_OVERRIDE
    {
        // return f_(std::forward<A0>(a0), std::forward<A1>(a1), ...);
        return f_(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ));
    }

    NSFX_INTERFACE_MAP_BEGIN(FunctorBasedEventSink)
        NSFX_INTERFACE_ENTRY(ISink)
    NSFX_INTERFACE_MAP_END()

private:
    F f_;
};/*}}}*/

// template<class ISink, class Ret, class A0, class A1, ...>
// class FunctionPointerBasedEventSink<ISink, Ret(A0, A1, ...)> : public ISink
template<class ISink, class Ret  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                      BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
class FunctionPointerBasedEventSink<ISink, Ret(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A))> :/*{{{*/
    public ISink
{
    BOOST_CONCEPT_ASSERT((IEventSinkConcept<ISink>));

public:
    // explicit FunctionPointerBasedEventSink(Ret(* fn)(A0, A1, ...)) :
    explicit FunctionPointerBasedEventSink(Ret(* fn)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A))) :
        fn_(fn)
    {
        if (!fn)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
    }

    virtual ~FunctionPointerBasedEventSink(void) BOOST_NOEXCEPT {}

    // virtual Ret Fire(A0 a0, A1 a1, ...) NSFX_OVERRIDE
    virtual Ret Fire(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, a)) NSFX_OVERRIDE
    {
        // return (*fn_)(std::forward<A0>(a0), std::forward<A1>(a1), ...);
        return (*fn_)(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ));
    }

    NSFX_INTERFACE_MAP_BEGIN(FunctionPointerBasedEventSink)
        NSFX_INTERFACE_ENTRY(ISink)
    NSFX_INTERFACE_MAP_END()

private:
    // Ret(* fn_)(A0, A1, ...);
    Ret(* fn_)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A));
};/*}}}*/

// template<class ISink, class Ret, class A0, class A1, ...>
// class MemberFunctionBasedEventSink<ISink, Ret(A0, A1, ...)> : public ISink
template<class ISink, class O, class Ret  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                               BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
class MemberFunctionBasedEventSink<ISink, O, Ret(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A))> :/*{{{*/
    public ISink
{
    BOOST_CONCEPT_ASSERT((IEventSinkConcept<ISink>));

public:
    // MemberFunctionBasedEventSink(O* o, Ret(* fn)(A0, A1, ...)) :
    MemberFunctionBasedEventSink(O* o, Ret(O::* ptmf)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A))) :
        o_(o),
        ptmf_(ptmf)
    {
        if (!o || !ptmf)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
    }

    virtual ~MemberFunctionBasedEventSink(void) BOOST_NOEXCEPT {}

    // virtual Ret Fire(A0 a0, A1 a1, ...) NSFX_OVERRIDE
    virtual Ret Fire(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, a)) NSFX_OVERRIDE
    {
        // return (o_->*ptmf_)(std::forward<A0>(a0), std::forward<A1>(a1), ...);
        return (o_->*ptmf_)(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ));
    }

    NSFX_INTERFACE_MAP_BEGIN(MemberFunctionBasedEventSink)
        NSFX_INTERFACE_ENTRY(ISink)
    NSFX_INTERFACE_MAP_END()

private:
    O* o_;
    // Ret(O::* ptmf_)(A0, A1, ...);
    Ret(O::* ptmf_)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A));
};/*}}}*/


////////////////////////////////////////
// template<class ISink, class Ret, class A0, class A1, ...>
// class EventSinkCreator<class ISink, Ret(A0, A1, ...)>
template<class ISink, class Ret  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                      BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
class EventSinkCreator<ISink, Ret(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A))>/*{{{*/
{
    BOOST_CONCEPT_ASSERT((IEventSinkConcept<ISink>));

public:
    // For functors, lambda expressions
    template<class F>
    Ptr<ISink> operator()(IObject* controller, F&& f) const
    {
        typedef FunctorBasedEventSink<ISink, F>  EventSink;
        typedef Object<EventSink>     EventSinkType;
        typedef AggObject<EventSink>  AggEventSinkType;
        return !controller
            ? static_cast<ISink*>(new EventSinkType(std::forward<F>(f)))
            : Ptr<IObject>(new AggEventSinkType(controller, std::forward<F>(f)));
    }

    // For funtion pointers.
    // template<class Ret, class A0, class A1, ...>
    // Ptr<ISink> operator()(Ret(* fn)(A0, A1, ...)) const
    template<class Ret  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())  BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
    Ptr<ISink> operator()(IObject* controller, Ret(* fn)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A))) const
    {
        typedef FunctionPointerBasedEventSink<ISink>  EventSink;
        typedef Object<EventSink>     EventSinkType;
        typedef AggObject<EventSink>  AggEventSinkType;
        return !controller
            ? static_cast<ISink*>(new EventSinkType(fn))
            : Ptr<IObject>(new AggEventSinkType(controller, fn));
    }

    // For objects and pointers to member funtions.
    // template<class O, class Ret, class A0, class A1, ...>
    // Ptr<ISink> operator()(O* o, Ret(O::* ptmf)(A0, A1, ...)) const
    template<class O, class Ret  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())  BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
    Ptr<ISink> operator()(IObject* controller, O* o, Ret(O::* ptmf)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A))) const
    {
        typedef MemberFunctionBasedEventSink<ISink, O>  EventSink;
        typedef Object<EventSink>     EventSinkType;
        typedef AggObject<EventSink>  AggEventSinkType;
        return !controller
            ? static_cast<ISink*>(new EventSinkType(o, ptmf))
            : Ptr<IObject>(new AggEventSinkType(controller, o, ptmf));
    }

};/*}}}*/


////////////////////////////////////////
// Free functions.
// template<class ISink, class Ret, class A0, class A1, ...>
// inline Ptr<ISink> CreateEventSink(Ret(* fn)(A0, A1, ...))
template<class ISink, class Ret  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                      BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
inline Ptr<ISink> CreateEventSink(IObject* controller, Ret(* fn)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A)))
{
    return EventSinkCreator<ISink>()(controller, fn);
}

// template<class ISink, class O, class Ret, class A0, class A1, ...>
// inline Ptr<ISink> CreateEventSink(O* o, Ret(O::* ptmf)(A0, A1, ...))
template<class ISink, class O, class Ret  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                               BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
inline Ptr<ISink> CreateEventSink(IObject* controller, O* o, Ret(O::* ptmf)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A)))
{
    return EventSinkCreator<ISink>()(controller, o, ptmf);
}

#undef NSFX_PP_FORWARD

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/


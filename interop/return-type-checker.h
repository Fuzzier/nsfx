/**
 * @file
 *
 * @brief Interoperability support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-21
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef RETURN_TYPE_CHECKER_H__A02628FD_4543_4EA8_80E0_AF3FC6E8D173
#define RETURN_TYPE_CHECKER_H__A02628FD_4543_4EA8_80E0_AF3FC6E8D173


#include <nsfx/interop/config.h>
#include <nsfx/interop/exception.h>
#include <nsfx/interop/variant/variant.h>
#include <nsfx/component/object.h>
#include <nsfx/component/ptr.h>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/size_t.hpp>
#include <type_traits> // decay, conditional, is_same, is_pointer, remove_pointer


NSFX_OPEN_NAMESPACE


namespace aux { /*{{{*/


template<size_t index, class Sequence>
inline const char* GetTypeName(void) BOOST_NOEXCEPT
{
    static_assert(boost::mpl::is_sequence<Sequence>::value,
                  "The 'Sequence' type must be a BOOST MPL sequence.");
    static_assert(index < boost::mpl::size<Sequence>::value,
                  "Cannot access the type in the Sequence, "
                  "since the index is out of bound.");
    typedef typename
        boost::mpl::at<Sequence, boost::mpl::size_t<index> >::type  Type;
    return VariantTraits<Type>::GetTypeName();
}

template<class Sequence, size_t size = boost::mpl::size<Sequence>::value>
struct GetTypeNameTable;

template<class Sequence>
struct GetTypeNameTable<Sequence, 0>
{
    typedef const char* (* FunctionType)(void);
    static FunctionType GetTable(size_t index)
    {
        return nullptr;
    }
};

template<class Sequence>
struct GetTypeNameTable<Sequence, 1>
{
    typedef const char* (* FunctionType)(void);
    static FunctionType Get(size_t index)
    {
        static const FunctionType table[] = {
            &GetTypeName<0, Sequence>
        };
        return table[index];
    }
};

template<class Sequence>
struct GetTypeNameTable<Sequence, 2>
{
    typedef const char* (* FunctionType)(void);
    static FunctionType GetTable(size_t index)
    {
        static const FunctionType table[] = {
            &GetTypeName<0, Sequence>,
            &GetTypeName<1, Sequence>
        };
        return table[index];
    }
};

template<class T>
inline void CheckReturnType(const Variant& ret)
{
    typedef typename std::decay<T>::type  RetType;

    // If the variant is not empty.
    if (ret.GetType() != VT_EMPTY)
    {
        // The variant must be a reference.
        if (!(ret.GetType() & VT_BYREF))
        {
            BOOST_THROW_EXCEPTION(
                InvalidReturnVariant() <<
                ErrorMessage("The variant of the return value is not "
                             "a reference."));
        }
        // Is Ptr<Intf>?
        else if (IsPtr<RetType>::value)
        {
            if (!(ret.GetType() & VT_OBJECT))
            {
                BOOST_THROW_EXCEPTION(
                    InvalidReturnVariant() <<
                    ErrorMessage("The type of the variant mismatches "
                                 "the type of the return value."));
            }
        }
        // Other types.
        else if (ret.GetType() != (VariantTraits<RetType>::type | VT_BYREF))
        {
            BOOST_THROW_EXCEPTION(
                InvalidReturnVariant() <<
                ErrorMessage("The type of the variant mismatches "
                             "the type of the return value."));
        }
    }
}

template<size_t index, class ArgTypeSequence>
inline void CheckArgumentType(const Variant& arg)
{
    typedef typename
        boost::mpl::at<ArgTypeSequence, boost::mpl::size_t<index> >::type  T;
    typedef typename std::decay<T>::type  ArgType;
    if (arg.GetType() != VariantTraits<ArgType>::type)
    {
        BOOST_THROW_EXCEPTION(
            InvalidArgumentVariant() <<
            InvalidArgumentIndex(index) <<
            ErrorMessage("The type of the variant mismatches "
                         "the type of the argument."));
    }
}

template<class ArgTypeSequence,
         size_t numArgs = boost::mpl::size<ArgTypeSequence>::value >
struct CheckArgumentTypeTable;

template<class ArgTypeSequence>
struct CheckArgumentTypeTable<ArgTypeSequence, 0>
{
    typedef void (* FunctionType)(const Variant& arg);
    static const FunctionType* GetTable()
    {
        return nullptr;
    }
};

template<class ArgTypeSequence>
struct CheckArgumentTypeTable<ArgTypeSequence, 1>
{
    typedef void (* FunctionType)(const Variant& arg);
    static const FunctionType* Get()
    {
        static FunctionType table[] = {
            &CheckArgumentType<0, ArgTypeSequence>,
        };
        return table;
    }
};

template<class ArgTypeSequence>
struct CheckArgumentTypeTable<ArgTypeSequence, 2>
{
    typedef void (* FunctionType)(const Variant& arg);
    static const FunctionType* Get()
    {
        static FunctionType table[] = {
            &CheckArgumentType<0, ArgTypeSequence>,
            &CheckArgumentType<1, ArgTypeSequence>,
        };
        return table;
    }
};

template<class Ret, class ArgTypeSequence>
inline void CheckTypes(const Variant& ret, size_t numArgs, Variant* args)
{
    // Check the return value.
    CheckReturnType<Ret>(ret);
    // Check the number of arguments.
    if (numArgs != boost::mpl::size<ArgTypeSequence>::value)
    {
        BOOST_THROW_EXCEPTION(
            InvalidNumberOfArguments() <<
            ExpectedNumberOfArguments(boost::mpl::size<ArgTypeSequence>::value) <<
            SuppliedNumberOfArguments(numArgs) <<
            ErrorMessage("Invalid number of the arguments."));
    }
    // Check the type of each argument.
    for (size_t i = 0; i < numArgs; ++i)
    {
        typedef CheckArgumentTypeTable<ArgTypeSequence>  TableType;
        typename TableType::FunctionType  check = TableType::Get()[i];
        (*check)(args[i]);
    }
}

struct DoReturnTag {};
struct NoReturnTag {};

template<size_t numArgs>
struct Invoker;

template<>
struct Invoker<0>
{
    template<class I, class Ret>
    static void Invoke(I* o, Ret (I::* ptmf)(void),
                       Variant& ret, Variant* args)
    {
        typedef typename std::conditional<std::is_same<Ret, void>::value,
                         NoReturnTag, DoReturnTag>::type  R;
        Invoke(o, ptmf, ret, args, R());
    }

    template<class I, class Ret>
    static void Invoke(I* o, Ret (I::* ptmf)(void),
                       Variant& ret, Variant* args, NoReturnTag)
    {
        (o->*ptmf)();
    }

    template<class I, class Ret>
    static void Invoke(I* o, Ret (I::* ptmf)(void),
                       Variant& ret, Variant* args, DoReturnTag)
    {
        typedef typename std::decay<Ret>::type  RetType;
        if (ret.GetType() != VT_EMPTY)
        {
            Ret r = (o->*ptmf)();
            ret.Carry(r);
        }
        else
        {
            Invoke(o, ptmf, ret, args, NoReturnTag());
        }
    }

};

template<>
struct Invoker<1>
{
    template<class I, class Ret, class A0>
    static void Invoke(I* o, Ret (I::* ptmf)(A0),
                       Variant& ret, Variant* args)
    {
        typedef typename std::conditional<std::is_same<Ret, void>::value,
                         NoReturnTag, DoReturnTag>::type  R;
        Invoke(o, ptmf, ret, args, R());
    }

    template<class I, class Ret, class A0>
    static void Invoke(I* o, Ret (I::* ptmf)(A0),
                       Variant& ret, Variant* args, NoReturnTag)
    {
        typedef typename std::decay<A0>::type   Arg0;
        (o->*ptmf)(args[0].GetValue<Arg0>());
    }

    template<class I, class Ret, class A0>
    static void Invoke(I* o, Ret (I::* ptmf)(A0),
                       Variant& ret, Variant* args, DoReturnTag)
    {
        typedef typename std::decay<Ret>::type  RetType;
        typedef typename std::decay<A0>::type   Arg0;
        if (ret.GetType() != VT_EMPTY)
        {
            Ret r = (o->*ptmf)(args[0].GetValue<Arg0>());
            ret.Carry(r)
        }
        else
        {
            Invoke(o, ptmf, ret, args, NoReturnTag());
        }
    }
};

template<>
struct Invoker<2>
{
    template<class I, class Ret, class A0, class A1>
    static void Invoke(I* o, Ret (I::* ptmf)(A0, A1),
                       Variant& ret, Variant* args)
    {
        typedef typename std::conditional<std::is_same<Ret, void>::value,
                         NoReturnTag, DoReturnTag>::type  R;
        Invoke(o, ptmf, ret, args, R());
    }

    template<class I, class Ret, class A0, class A1>
    static void Invoke(I* o, void (I::* ptmf)(A0, A1),
                       Variant& ret, Variant* args, NoReturnTag)
    {
        typedef typename std::decay<A0>::type   Arg0;
        typedef typename std::decay<A1>::type   Arg1;
        (o->*ptmf)(args[0].GetValue<Arg0>(),
                   args[1].GetValue<Arg1>());
    }

    template<class I, class Ret, class A0, class A1>
    static void Invoke(I* o, Ret (I::* ptmf)(A0, A1),
                       Variant& ret, Variant* args, DoReturnTag)
    {
        typedef typename std::decay<Ret>::type  RetType;
        typedef typename std::decay<A0>::type   Arg0;
        typedef typename std::decay<A1>::type   Arg1;
        if (!std::is_same<Ret, void>::value &&
            ret.GetType() != VT_EMPTY)
        {
            Ret r = (o->*ptmf)(args[0].GetValue<Arg0>(),
                               args[1].GetValue<Arg1>());
            ret.Carry(r);
        }
        else
        {
            Invoke(o, ptmf, ret, args, NoReturnTag());
        }
    }
};


} // namespace aux /*}}}*/


////////////////////////////////////////////////////////////////////////////////
class IMethodInfo :
    public IObject
{
public:
    virtual ~IMethodInfo(void) BOOST_NOEXCEPT {}

    virtual const char* GetName(void) = 0;
    virtual size_t GetNumArguments(void) = 0;
    virtual const char* GetReturnType(void) = 0;
    virtual const char* GetArgumentType(size_t index) = 0;
    virtual void Invoke(Variant ret, size_t numArgs, Variant* args) = 0;
};

NSFX_DEFINE_CLASS_UID(IMethodInfo, "edu.uestc.nsfx.IMethodInfo");


////////////////////////////////////////////////////////////////////////////////
template<class T>
class MethodInfo;


////////////////////////////////////////////////////////////////////////////////
template<class I, class Ret, class A0>
class MethodInfo<Ret (I::*)(A0)> :
    public IMethodInfo
{
    typedef MethodInfo  ThisClass;
    typedef typename boost::mpl::vector<A0>::type  ArgVector;

public:
    MethodInfo(Ptr<I> o, Ret (I::* ptmf)(A0)) :
        o_(o), ptmf_(ptmf)
    {
    }

    virtual const char* GetName(void) NSFX_OVERRIDE
    {
        return "Foo";
    }

    virtual size_t GetNumArguments(void) NSFX_OVERRIDE
    {
        return 1;
    }

    virtual const char* GetReturnType(void) NSFX_OVERRIDE
    {
        return VariantTraits<Ret>::GetTypeName();
    }

    virtual const char* GetArgumentType(size_t index) NSFX_OVERRIDE
    {
        typedef aux::GetTypeNameTable<ArgVector>  Table;
        return (*Table::Get(index))();
    }

    virtual void Invoke(Variant ret, size_t numArgs, Variant* args) NSFX_OVERRIDE
    {
        aux::CheckTypes<Ret, ArgVector>(ret, numArgs, args);
        typedef aux::Invoker<boost::mpl::size<ArgVector>::value>  Invoker;
        Invoker::Invoke(o_.Get(), ptmf_, ret, args);
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IMethodInfo)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<I> o_;
    Ret (I::* ptmf_)(A0);
};


////////////////////////////////////////////////////////////////////////////////
class ITry :
    public IObject
{
public:
    virtual ~ITry(void) {}

    virtual void Foo(bool i) = 0;
};

NSFX_DEFINE_CLASS_UID(ITry, "edu.uestc.nsfx.ITry");

class Try : public ITry
{
public:
    virtual ~Try(void) {};
    virtual void Foo(bool i) NSFX_OVERRIDE
    {
    }

    NSFX_INTERFACE_MAP_BEGIN(Try)
        NSFX_INTERFACE_ENTRY(ITry)
    NSFX_INTERFACE_MAP_END()
};


template<class I, class Ret, class A0>
inline Ptr<IMethodInfo> CreateMethodInfo(Ptr<I> o, Ret (I::* ptmf)(A0))
{
    typedef MethodInfo<Ret (I::*)(A0)>  MethodInfoType;
    return Ptr<IMethodInfo>(new Object<MethodInfoType>(o, ptmf));
}



NSFX_CLOSE_NAMESPACE


#endif // RETURN_TYPE_CHECKER_H__A02628FD_4543_4EA8_80E0_AF3FC6E8D173


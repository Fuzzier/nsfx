/**
 * @file
 *
 * @brief Interoperability support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-16
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef CLASS_META_H__D77C068A_E047_4900_A567_B167FCB62514
#define CLASS_META_H__D77C068A_E047_4900_A567_B167FCB62514


#include <nsfx/config.h>
#include <nsfx/exception/exception.h>
#include <nsfx/component/object.h>
#include <nsfx/component/ptr.h>
#include <nsfx/variant/variant.h>
#include <boost/type_traits/decay.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/size_t.hpp>


NSFX_OPEN_NAMESPACE


namespace detail {

template<size_t index, class ArgTypeSequence>
inline const char* GetTypeName(void) BOOST_NOEXCEPT
{
    static_assert(boost::mpl::is_squence<ArgTypeSequence>::value,
                  "The 'ArgTypeSequence' type must be a BOOST MPL sequence.");
    static_assert(index < boost::mpl::size<ArgTypeSequence>::value,
                  "Cannot access the type in the ArgTypeSequence, "
                  "since the index is out of bound.");
    typedef typename boost::mpl::at<ArgTypeSequence, boost::mpl::size_t<index>
                                   >::type  Type;
    return VariantTraits<Type>::GetTypeName();
}

template<class T>
inline void CheckReturnType(const Variant& ret)
{
    typedef typename boost::decay<T>::type  Type;

    // If the variant is not empty.
    if (ret.GetType() != VT_EMPTY)
    {
        // The variant must be a reference.
        if (!(ref.GetType() & VT_BYREF))
        {
            BOOST_THROW_EXCEPTION(
                InvalidArgument() <<
                ErrorMessage("The variant of the return value is not "
                             "a reference."));
        }
        // Is Ptr<Intf>?
        else if (IsPtr<Type>::value)
        {
            if (!(ret.GetType() & VT_OBJECT) &&
                !(ret.GetType() & VT_OBJECT_INFO))
            {
                BOOST_THROW_EXCEPTION(
                    InvalidArgument(),
                    ErrorMessage("The type of the variant mismatches "
                                 "the type of the return value."));
            }
        }
        // Other types.
        else if (ret.GetType() != VariantTraits<Type>::type | VT_BYREF)
        {
            BOOST_THROW_EXCEPTION(
                InvalidArgument(),
                ErrorMessage("The type of the variant mismatches "
                             "the type of the return value."));
        }
    }
}

template<size_t index, class ArgTypeSequence>
inline void CheckArgumentType(const Variant& arg)
{
    typedef typename boost::mpl::at<ArgTypeSequence, index>::type  T;
    typedef typename boost::decay<T>::type  Type;
    if (arg.GetType() != VariantTraits<Type>::type)
    {
        BOOST_THROW_EXCEPTION(
            InvalidArgument() <<
            ErrorMessage("The type of the variant mismatches "
                         "the type of the corresponding argument."));
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
inline void CheckInvokeTypes(Variant ret, size_t numArgs, Variant* args)
{
    // Check the return value.
    CheckReturnType<Ret>(ret);
    // Check the number of arguments.
    if (numArgs != boost::mpl::size<ArgTypeSequence>::value)
    {
        BOOST_THROW_EXCEPTION(
            InvalidArgument(),
            ErrorMessage("The number of the arguments is different than "
                         "that of the the method."));
    }
    // Check the type of each argument.
    for (size_t i = 0; i < numArgs; ++i)
    {
        typedef CheckArgumentTypeTable<ArgTypeSequence>  TableType;
        typename TableType::FunctionType  check = TableType::Get()[i];
        (*check)(args[i]);
    }
}

template<size_t numArgs>
struct Invoker;

template<>
struct Invoker<0>
{
    template<class Intf, class Ret>
    static void Invoke(Intf* intf, Ret (I::* ptmf)(void),
                       Variant ret, Variant* args)
    {
        ret.GetValue
        (intf->*ptmf)();
    }
};

template<>
struct Invoker<1>
{
    template<class Intf, class Ret, class A0>
    static void Invoke(Intf* intf, Ret (I::* ptmf)(A0),
                       Variant ret, Variant* args)
    {
        ret.GetValue
        (intf->*ptmf)(args[1].Get<A0>());
    }
};

} // namespace detail


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
template<class Intf, class Ret, class A0>
class MethodInfo<Ret (Intf::*)(A0)> :
    public IMethodInfo
{
    typedef MethodInfo  ThisClass;
    typedef typename boost::mpl::vector<A0>::type  ArgVector;

public:
    MethodInfo(Ptr<Intf> intf, Ret (Intf::* ptmf)(A0)) :
        intf_(intf), ptmf_(ptmf)
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
        typedef const char* (* FunctionType)(void);
        static const FunctionType table[1] = {
            &InternalGetArgumentType<0>
        };
        if (index < 1)
        {
            FunctionType f = table[index];
            return f();
        }
        else
        {
            return "void";
        }
    }

    virtual void Invoke(Variant ret, size_t numArgs, Variant* args) NSFX_OVERRIDE
    {
        if (numArgs != 1)
        {
            BOOST_THROW_EXCEPTION(InvalidArgument());
        }
        (*intf_.*ptmf_)(args[0].GetValue<A0>());
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IMethodInfo)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<Intf> intf_;
    Ret (Intf::* ptmf_)(A0);
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


template<class Intf, class Ret, class A0>
inline Ptr<IMethodInfo> CreateMethodInfo(Ptr<Intf> intf, Ret (Intf::* ptmf)(A0))
{
    typedef MethodInfo<Ret (Intf::*)(A0)>  MethodInfoType;
    return Ptr<IMethodInfo>(new Object<MethodInfoType>(intf, ptmf));
}



NSFX_CLOSE_NAMESPACE


#endif // CLASS_META_H__D77C068A_E047_4900_A567_B167FCB62514


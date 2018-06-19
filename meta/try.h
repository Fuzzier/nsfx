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
#include <boost/mpl/vector.hpp>


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Interop
 */
class ITry
{
public:
    virtual ~ITry(void) {}

    virtual double Foo(int i) = 0;

    NSFX_METHOD_MAP_BEGIN(ITry)
        NSFX_DECLARE_METHOD(Foo)
    NSFX_METHOD_MAP_END()
};


template<class T>
struct TypeInfo
{
    static const boost::typeindex::type_info&
    GetTypeInfo(void)
    {
        return boost::typeindex::type_id<T>().type_info();
    }
};


#define NSFX_DECLARE_METHOD(name)
    template<class I, class Ret, class... Args>
    class MethodInfo :
        public IMethodInfo
    {
        typedef typename boost::mpl::vector<Args...>::type  ArgVector;

    public:
        virtual const char* GetName(void)
        {
            return ##name;
        }

        virtual size_t GetNumArguments(void)
        {
            return sizeof...(Args);
        }

        virtual const boost::typeindex::type_info&
        GetReturnType(void)
        {
            return boost::typeindex::type_id<Ret>().type_info();
        }

        virtual const boost::typeindex::type_info&
        GetArgumentType(size_t index)
        {
            typedef const boost::typeindex::type_info& (*Func)(void);
            static const Func func[sizeof...(Args)] =
            {
                &TypeInfo<Args>::GetTypeInfo...
            };

            if (index < sizeof...(Args))
            {
                return (*func[index])();
            }
            else
            {
                return boost::typeindex::type_id<void>().type_info();
            }
        }

        virtual void Invoke(Variant returnValue,
                            size_t numArguments,
                            Variant* arguments)
        {
            if (numArguments != sizeof...(Args))
            {
                BOOST_THROW_EXCEPTION(InvalidArgument());
            }
        }

    private:
    };


class ITryInfo : public InterfaceInfo
{
public:
    virtual const char* GetUid(void)
    {
        return "edu.uestc.nsfx.ITry";
    }

    template<class Visitor>
    bool InternalVisit(Visitor&& visitor)
    {
        bool result = false;
        const char* name = "Foo";
        result = visitor(name,
                         const boost::typeindex::type_info& returnType,
                         size_t numArguments,
                         const boost::typeindex::type_info& args[MAX_NUM_ARGS],
                         Ptr<IMethodMetadata> method);
    }

    virtual void VisitMethods(Ptr<IMethodMetadataVisitor> visitor)
    {
    }

    virtual void Invoke(const char* name,
                        Variant& returnValue = emptyVariant,
                        Variant& arg1 = emptyVariant,
                        Variant& arg2 = emptyVariant,
                        Variant& arg3 = emptyVariant,
                        Variant& arg4 = emptyVariant,
                        Variant& arg5 = emptyVariant,
                        Variant& arg6 = emptyVariant)
    {
        if (name && strcmp(name, "Foo") == 0)
        {
            returnValue.GetValue<double>() =
                intf_->Foo(arg1.GetValue<int>());
        }
    }

private:
    Ptr<ITry>  intf_;
};


#endif // CLASS_META_H__D77C068A_E047_4900_A567_B167FCB62514


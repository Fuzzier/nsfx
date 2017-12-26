/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-13
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef CONFIG_H__5B879679_8DC6_42C4_A3F8_5CDF1E3F302B
#define CONFIG_H__5B879679_8DC6_42C4_A3F8_5CDF1E3F302B


#include <nsfx/config.h>


/**
 * @defgroup Component
 * @brief Support for component-based programming.
 *
 * A component is an object that exposes various interfaces.<br/>
 *
 * # What component model should be used?<br/>
 * ## 1. How to obtain different interfaces on a object?<br/>
 *
 * ### 1.1 Use concrete class directly.<br/>
 *     The primary form of interface implementation is to create a concrete
 *     class that inherits from pure abstract bases, and implements the methods.<br/>
 *     Such classes can be large and heavy, since a class inherits from all
 *     interfaces it implements, and presents to users a huge table of virtual
 *     functions, as long as a set of non-virtual functions.<br>
 *     It becomes unclear how interfaces are cohesive with each other, but they
 *     are just coupled tightly together within a class.<br/>
 *
 *     Interfaces are generally not aware of each other.<br/>
 *     The users generally have to know the concrete type of the object and use
 *     member functions or type cast to obtain the interfaces.<br/>
 *     When other objects want to use the object, they either have to know its
 *     concrete type, or the users have to extract and pass all necessary
 *     interfaces.<br/>
 *
 * ### 1.2 Hide object behind a navigator interface.<br/>
 *     Use a dictated navigator interface to query interfaces.<br/>
 *     The navigator interface can be considered as the object itself.<br/>
 *     Use an object factory to obtain the navigator interface, so there is no
 *     need to know the concrete type of an object.<br/>
 *     Pass the navigator interface to other objects.<br/>
 *     Let other objects query the required interfaces.<br/>
 *
 * ### 1.3 Make all interfaces navigable.<br/>
 *     All interfaces provide navigability around the object, such as Microsoft
 *     COM.<br/>
 *     Virtual inheritance has to be used, as all interfaces extends the root
 *     navigator interface.<br/>
 *
 * ## 2. How to manage object lifetime?<br/>
 *    Only deterministic methods are considered, since a garbage collector is
 *    not considered right now.<br/>
 *
 * ### 2.1 Implicit transfer of responsibility.<br/>
 *     Transfer the responsibility of deallocating an object when a raw-pointer
 *     is transferred.<br/>
 *     Use \c std::unique_ptr to enforce the transfer of responsibility.<br/>
 *
 * ### 2.2 Creator-based management.<br/>
 *     When an object is created, the creator is responsible to deallocate the
 *     object.<br/>
 *     The creator must not be deallocated until the created object is no longer
 *     used.<br/>
 *     It requires a careful pre-design of the software system to properly
 *     layout the objects.<br/>
 *     The allocation relationships must form directed trees, where the vertices
 *     are objects, and an edge starts from an allocator to an allocated.<br/>
 *     This way, a clear deallocation path is formed so that no object is
 *     deallocated more than once or leaked, since an allocated object has one
 *     and only one allocator (parent) that is responsible to deallocate it.<br/>
 *
 * ### 2.3 Reference counting.<br/>
 *     Use a reference count to manage object lifetime, either intrusively or
 *     non-intrusively.<br/>
 *     It eliminates the need to transfer the responsibility of deallocation,
 *     and formulates a distributed lifetime management scheme.<br/>
 *     Users can use \c std::shared_ptr and \c std::weak_ptr for non-intrusive
 *     reference counting.<br/>
 *
 *     It has a problem of cyclic reference counting, since deferencing is
 *     passive.<br/>
 *     i.e., the intent to deallocate an object is not given to all holders of
 *     the object.<br/>
 *     It can be solved by giving a signal explicitly to every holder of an
 *     object to deallocate it.<br/>
 *
 *     To manage the lifetime of <i>different</i> interfaces on a single object,
 *     a smart pointer must either know the concrete type of the object, or
 *     have a way to access the reference count.<br/>
 *     If the interface held by a smart pointer doesn't provide lifetime
 *     management itself, the smart pointer has to store a variable or function
 *     pointer to access the reference count (cost memory).<br/>
 *     And even has to query a dictated lifetime manager interface at runtime
 *     (cost CPU cycles).<br/>
 *     If every interface provides lifetime management, such as microsoft COM,
 *     virtual inheritance has to be used.<br/>
 *
 * ### 2.4 Mixed schemes.<br/>
 *     Mixing lifetime management schemes could be beneficial, but could cause
 *     more headaches for users to use them properly.<br/>
 *
 * ### 2.5. Transfer of reference counts across functions.<br/>
 *     When a pointer to an object is passed into a function call, the caller
 *     must guarantee that the object remains valid until the function returns.<br/>
 *     Usually, the caller has to increment the reference count by one before
 *     calling the function.<br/>
 *
 *     Should that one reference count be transferred to the callee?<br/>
 *     Microsoft COM rules specify that there is no transferring, and the caller
 *     is responsible to release this reference count.<br/>
 *     Microsoft RCW (Runtime Caller Wrapper) rules specify that this reference
 *     count is transferred, and the callee is responsible to release this
 *     reference count.<br/>
 *
 *     When a pointer to an object is passed out of a function call, the callee
 *     must guarantee that the caller obtains a valid object.<br/>
 *     Usually, the callee has to increment the reference count by one, and
 *     transfer that one reference count to the caller.<br/>
 *
 *     In NSFX, the intention whether to transfer a reference count is stated
 *     explicitly.<br/>
 *     1) If a <b>smart pointer</b> is used, a reference count is transferred
 *        across the function call.<br/>
 *     2) If a <b>non-void raw pointer</b> is used, a reference count is <b>not</b>
 *        transferred across the function call.<br/>
 *     3) If a <b>void raw pointer</b> is used, a reference count is transferred
 *        out of the function call.<br/>
 *
 *     The 3rd case is only applied to interface acquisition and object
 *     creation methods.<br/>
 *     Since a virtual function cannot have multiple return types, and function
 *     templates cannot be mixed with virtual funtions, the return type has to
 *     be \c void*.<br/>
 *     Examples are \c IObject::QueryInterface() and \c IFactory::CreateObject().<br/>
 *     Such cases are handled by library-provided smart pointers and template
 *     functions (e.g., \c CreateObject()), and should not bother the users.<br/>
 *     User-defined methods should avoid using void raw pointers as return
 *     values.<br/>
 *
 *     The 2nd case is commonly used to retrieve an interface pointer on a
 *     singleton object, and the returned pointer can be a non-void raw pointer.<br/>
 *     It is user's responsibility to ensure that the singleton remains valid.<br/>
 *
 * ## 3. Conclusion.<br/>
 *    Performance and flexibility are conflicting aspects.<br/>
 *    To improve performance, more responsibilities are shifted to the users.<br/>
 *    And the code becomes more error-prone, since the users will almost always
 *    forget certain oral rules sometime somewhere.<br/>
 *
 *    The library is not performance hunger.<br/>
 *    Try not to shift burdens to the users.<br/>
 *    Intrusive reference counting is welcome.<br/>
 *    Objects are hidden behind interfaces.<br/>
 *    Who wants an interface, who should query it.<br/>
 */


NSFX_OPEN_NAMESPACE

/**
 * @ingroup Component
 * @brief The type of reference count.
 */
typedef ptrdiff_t  refcount_t;

NSFX_CLOSE_NAMESPACE


#endif // CONFIG_H__5B879679_8DC6_42C4_A3F8_5CDF1E3F302B


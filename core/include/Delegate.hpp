//						Delegate.hpp
//	Efficient delegates in C++ that generate only two lines of asm code!
//  Documentation is found at http://www.codeproject.com/cpp/FastDelegate.asp
//
//						- Don Clugston, Mar 2004.
//		Major contributions were made by Jody Hagins.
//      C++17 support by Amirmohamad Rezaee, Jan 2023.
// History:
// 24-Apr-04 1.0  * Submitted to CodeProject.
// 28-Apr-04 1.1  * Prevent most unsafe uses of evil static function hack.
//				  * Improved syntax for horrible_cast (thanks Paul Bludov).
//				  * Tested on Metrowerks MWCC and Intel ICL (IA32)
//				  * Compiled, but not run, on Comeau C++ and Intel Itanium ICL.
//	27-Jun-04 1.2 * Now works on Borland C++ Builder 5.5
//				  * Now works on /clr "managed C++" code on VC7, VC7.1
//				  * Comeau C++ now compiles without warnings.
//				  * Prevent the virtual inheritance case from being used on
//					VC6 and earlier, which generate incorrect code.
//				  * Improved warning and error messages. Non-standard hacks
//					now have compile-time checks to make them safer.
//				  * implicit_cast used instead of static_cast in many cases.
//				  * If calling a const member function, a const class pointer can be
// used.
//				  * MakeDelegate() global helper function added to simplify
// 					pass-by-value.
//				  * Added fastdelegate.clear()
// 16-Jul-04 1.2.1* Workaround for gcc bug (const member function pointers in
// 					templates) 30-Oct-04 1.3  * Support for (non-void) return values.
//				  * No more workarounds in client code!
//					MSVC and Intel now use a clever hack invented by John Dlugosz:
//				     - The FASTDELEGATEDECLARE workaround is no longer necessary.
//					 - No more warning messages for VC6
//				  * Less use of macros. Error messages should be more comprehensible.
//				  * Added include guards
//				  * Added FastDelegate::empty() to test if invocation is safe (Thanks
// 					Neville Franks).
//				  * Now tested on VS 2005 Express Beta, PGI C++
// 24-Dec-04 1.4  * Added DelegateMemento, to allow collections of disparate
// 					delegates.
//                * <,>,<=,>= comparison operators to allow storage in ordered
//                	containers.
//				  * Substantial reduction of code size, especially the 'Closure'
// 					class.
//				  * Standardised all the compiler-specific workarounds.
//                * MFP conversion now works for CodePlay (but not yet supported in
//                	the full code).
//                * Now compiles without warnings on _any_ supported compiler,
//                	including BCC 5.5.1
//				  * New syntax: FastDelegate< int (char *, double) >.
// 14-Feb-05 1.4.1* Now treats =0 as equivalent to .clear(), ==0 as equivalent to
// 					.empty(). (Thanks elfric).
//				  * Now tested on Intel ICL for AMD64, VS2005 Beta for AMD64 and
// 					Itanium.
// 30-Mar-05 1.5  * Safebool idiom: "if (dg)" is now equivalent to "if (!dg.empty())"
//				  * Fully supported by CodePlay VectorC
//                * Bugfix for Metrowerks: empty() was buggy because a valid MFP can
//                	be 0 on MWCC!
//                * More optimal assignment,== and != operators for static function
//                	pointers.
// 16-Jan-23 2.0  * Old header guard and MSVC check for supporting #pragma once
//                  removed.
//                * <memory.h> changed to <memory>.
//                * Check for function-type-syntax support removed. It's
//                  available by default in modern compilers.
//                * DOS compiler support removed. Too old.
//                * Static function hack is enabled by default and its config macro
//                  removed. It's not evil in C++11 and newer standards.
//                * VC6 compiler support removed. It's too old!
//                * GCC bug #8271 workaround removed. It's fixed a long time ago.
//                * Compile-time-error tricks replaced by static_assert.
//                * Removed old Digital Mars compiler support.
//                * Removed old CodePlay compiler support.
//                * Const variables changed to constexpr.
//                * Comments updated and cleaned.
//                * typedef replaced by using for better readability.
//                * memcmp changed to std::memcmp and cstring header included.
//                * Added Hash function to DelegateMemento.
//                * ClosurePtr bindconstmemfunc renamed to bindmemfunc. (bindmemfunc
//                  overloaded)
//                * Added variadic templates.
//                * some functions made const.
//                * MakeDelegate() overloaded for global-function.
//                * Added object refrence overloaded versions of ctors, bind() and
//                	MakeDelegate().
//                * fastdelegate namespace removed.
//                * bind(), clear() and empty() changed to PascalCase.
//                * Part 3 and 4 combined.
//                * FastDelegate Renamed to Delegate for simplicity.
//                * std::forward and std::invoke used.
//                * replaced some 0's by nullptr.
#pragma once

#include <cstring>     // for std::memcmp
#include <functional>  // for std::invoke
#include <memory>      // to allow <,> comparisons
#include <utility>     // for std::forward<>

////////////////////////////////////////////////////////////////////////////////
//						Compiler identification for workarounds
////////////////////////////////////////////////////////////////////////////////

// Compiler identification. It's not easy to identify Visual C++ because
// many vendors fraudulently define Microsoft's identifiers.
#if defined(_MSC_VER) && !defined(__MWERKS__) && !defined(__VECTOR_C) && \
        !defined(__ICL) && !defined(__BORLANDC__)
	#define FASTDLGT_ISMSVC
#endif

// Does the compiler uses Microsoft's member function pointer structure?
// If so, it needs special treatment.
// Metrowerks CodeWarrior, Intel, and CodePlay fraudulently define Microsoft's
// identifier, _MSC_VER. We need to filter Metrowerks out.
#if defined(_MSC_VER) && !defined(__MWERKS__)
	#define FASTDLGT_MICROSOFT_MFP
#endif

////////////////////////////////////////////////////////////////////////////////
//						General tricks used in this code
//
// (a) Warning messages on MSVC are generated by declaring unused variables, and
//	    enabling the "variable XXX is never used" warning.
// (b) Unions are used in a few compiler-specific cases to perform illegal casts.
// (c) For Microsoft and Intel, when adjusting the 'this' pointer, it's cast to
//     (char *) first to ensure that the correct number of *bytes* are added.
////////////////////////////////////////////////////////////////////////////////
//						Helper templates
////////////////////////////////////////////////////////////////////////////////

namespace detail
{  // we'll hide the implementation details in a nested namespace.

//		implicit_cast< >
// I believe this was originally going to be in the C++ standard but
// was left out by accident. It's even milder than static_cast.
// I use it instead of static_cast<> to emphasize that I'm not doing
// anything nasty.
// Usage is identical to static_cast<>
template<class OutputClass, class InputClass>
inline OutputClass implicit_cast(InputClass input)
{
	return input;
}

//		horrible_cast< >
// This is truly evil. It completely subverts C++'s type system, allowing you
// to cast from any class to any other class. Technically, using a union
// to perform the cast is undefined behaviour (even in C). But we can see if
// it is OK by checking that the union is the same size as each of its members.
// horrible_cast<> should only be used for compiler-specific workarounds.
// Usage is identical to reinterpret_cast<>.

// This union is declared outside the horrible_cast because BCC 5.5.1
// can't inline a function with a nested class, and gives a warning.
template<class OutputClass, class InputClass>
union horrible_union
{
	OutputClass out;
	InputClass  in;
};

template<class OutputClass, class InputClass>
inline OutputClass horrible_cast(const InputClass input)
{
	horrible_union<OutputClass, InputClass> u;
	// Cause a compile-time error if in, out and u are not the same size.
	// If the compile fails here, it means the compiler has peculiar
	// unions which would prevent the cast from working.
	static_assert(sizeof(InputClass) == sizeof(u) &&
	                      sizeof(InputClass) == sizeof(OutputClass),
	              "Cannot use horrible_cast<>");
	u.in = input;
	return u.out;
}

////////////////////////////////////////////////////////////////////////////////
//						Workarounds
////////////////////////////////////////////////////////////////////////////////

using DefaultVoid = void;

// Translate from 'DefaultVoid' to 'void'.
// Everything else is unchanged
template<class T>
struct DefaultVoidToVoid
{
	using type = T;
};

template<>
struct DefaultVoidToVoid<DefaultVoid>
{
	using type = void;
};

// Translate from 'void' into 'DefaultVoid'
// Everything else is unchanged
template<class T>
struct VoidToDefaultVoid
{
	using type = T;
};

template<>
struct VoidToDefaultVoid<void>
{
	using type = DefaultVoid;
};

////////////////////////////////////////////////////////////////////////////////
//						Fast Delegates, part 1:
//
//		Conversion of member function pointer to a standard form
////////////////////////////////////////////////////////////////////////////////

// GenericClass is a fake class, ONLY used to provide a type.
// It is vitally important that it is never defined, so that the compiler doesn't
// think it can optimize the invocation. For example, Borland generates simpler
// code if it knows the class only uses single inheritance.

// Compilers using Microsoft's structure need to be treated as a special case.
#ifdef FASTDLGT_MICROSOFT_MFP
// For Microsoft and Intel, we want to ensure that it's the most efficient type
// of MFP (4 bytes), even when the /vmg option is used. Declaring an empty class
// would give 16 byte pointers in this case....
class __single_inheritance GenericClass;
class GenericClass
{
};
#else
class GenericClass;
#endif

// The size of a single inheritance member function pointer.
constexpr int SINGLE_MEMFUNCPTR_SIZE = sizeof(void(GenericClass::*)());

//						SimplifyMemFunc< >::Convert()
//
// A template function that converts an arbitrary member function pointer into
// the simplest possible form of member function pointer, using a supplied
// 'this' pointer.
// According to the standard, this can be done legally with reinterpret_cast<>.
// For (non-standard) compilers which use member function pointers which vary in
// size depending on the class, we need to use knowledge of the internal
// structure of a member function pointer, as used by the compiler. Template
// specialization is used to distinguish between the sizes. Because some
// compilers don't support partial template specialisation, I use full
// specialisation of a wrapper struct.

// general case -- don't know how to convert it. Force a compile failure.
template<int N>
struct SimplifyMemFunc
{
	template<class X, class XFuncType, class GenericMemFuncType>
	inline static GenericClass* Convert(X* pthis, XFuncType function_to_bind,
	                                    GenericMemFuncType& bound_func)
	{
		// Unsupported member function type. force a compile failure.
		static_assert(N - 100, "Unsupported member function type");
		return nullptr;
	}
};

// For compilers where all member func ptrs are the same size, everything goes
// here. For non-standard compilers, only single_inheritance classes go here.
template<>
struct SimplifyMemFunc<SINGLE_MEMFUNCPTR_SIZE>
{
	template<class X, class XFuncType, class GenericMemFuncType>
	inline static GenericClass* Convert(X* pthis, XFuncType function_to_bind,
	                                    GenericMemFuncType& bound_func)
	{
		bound_func = reinterpret_cast<GenericMemFuncType>(function_to_bind);
		return reinterpret_cast<GenericClass*>(pthis);
	}
};

////////////////////////////////////////////////////////////////////////////////
//						Fast Delegates, part 1b:
//					Workarounds for Microsoft and Intel
////////////////////////////////////////////////////////////////////////////////

// Compilers with member function pointers which violate the standard (MSVC, Intel,
// Codeplay), need to be treated as a special case.
#ifdef FASTDLGT_MICROSOFT_MFP

// We use unions to perform horrible_casts. I would like to use #pragma
// pack(push, 1) at the start of each function for extra safety.

// __multiple_inheritance classes go here
// Nasty hack for Microsoft and Intel (IA32 and Itanium)
template<>
struct SimplifyMemFunc<SINGLE_MEMFUNCPTR_SIZE + sizeof(int)>
{
	template<class X, class XFuncType, class GenericMemFuncType>
	inline static GenericClass* Convert(X* pthis, XFuncType function_to_bind,
	                                    GenericMemFuncType& bound_func)
	{
		// We need to use a horrible_cast to do this conversion.
		// In MSVC, a multiple inheritance member pointer is internally defined
		// as:
		union
		{
			XFuncType func;
			struct
			{
				// points to the actual member function
				GenericMemFuncType funcaddress;
				int delta;  // #BYTES to be added to the 'this' pointer
			} s;
		} u;

		// Check that the horrible_cast will work
		static_assert(sizeof(function_to_bind) == sizeof(u.s),
		              "Cannot use horrible_cast<>");

		u.func     = function_to_bind;
		bound_func = u.s.funcaddress;

		return reinterpret_cast<GenericClass*>(reinterpret_cast<char*>(pthis) +
		                                       u.s.delta);
	}
};

// virtual inheritance is a real nuisance. It's inefficient and complicated.
// On MSVC and Intel, there isn't enough information in the pointer itself to
// enable conversion to a closure pointer. Earlier versions of this code didn't
// work for all cases, and generated a compile-time error instead.
// But a very clever hack invented by John M. Dlugosz solves this problem.
// My code is somewhat different to his: I have no asm code, and I make no
// assumptions about the calling convention that is used.

// In VC++ and ICL, a virtual_inheritance member pointer
// is internally defined as:
struct MicrosoftVirtualMFP
{
	void (GenericClass::*codeptr)();  // points to the actual member function
	int delta;                        // #bytes to be added to the 'this' pointer
	int vtable_index;                 // or 0 if no virtual inheritance
};
// The CRUCIAL feature of Microsoft/Intel MFPs which we exploit is that the
// m_codeptr member is *always* called, regardless of the values of the other
// members. (This is *not* true for other compilers, eg GCC, which obtain the
// function address from the vtable if a virtual function is being called).
// Dlugosz's trick is to make the codeptr point to a probe function which
// returns the 'this' pointer that was used.

// Define a generic class that uses virtual inheritance.
// It has a trival member function that returns the value of the 'this' pointer.
struct GenericVirtualClass: virtual public GenericClass
{
	using ProbePtrType = GenericVirtualClass* (GenericVirtualClass::*)();
	GenericVirtualClass* GetThis() { return this; }
};

// __virtual_inheritance classes go here
template<>
struct SimplifyMemFunc<SINGLE_MEMFUNCPTR_SIZE + 2 * sizeof(int)>
{
	template<class X, class XFuncType, class GenericMemFuncType>
	inline static GenericClass* Convert(X* pthis, XFuncType function_to_bind,
	                                    GenericMemFuncType& bound_func)
	{
		union
		{
			XFuncType func;
			GenericClass* (X::*ProbeFunc)();
			MicrosoftVirtualMFP s;
		} u;

		u.func     = function_to_bind;
		bound_func = reinterpret_cast<GenericMemFuncType>(u.s.codeptr);

		union
		{
			GenericVirtualClass::ProbePtrType virtfunc;
			MicrosoftVirtualMFP               s;
		} u2;

		// Check that the horrible_cast<>s will work
		static_assert(sizeof(function_to_bind) == sizeof(u.s) &&
		                      sizeof(function_to_bind) == sizeof(u.ProbeFunc) &&
		                      sizeof(u2.virtfunc) == sizeof(u2.s),
		              "Cannot use horrible_cast<>");

		// Unfortunately, taking the address of a MF prevents it from being
		// inlined, so this next line can't be completely optimised away by the
		// compiler.
		u2.virtfunc = &GenericVirtualClass::GetThis;
		u.s.codeptr = u2.s.codeptr;
		return (pthis->*u.ProbeFunc)();
	}
};

// Nasty hack for Microsoft and Intel (IA32 and Itanium)
// unknown_inheritance classes go here
// This is probably the ugliest bit of code I've ever written. Look at the casts!
// There is a compiler bug in MSVC6 which prevents it from using this code.
template<>
struct SimplifyMemFunc<SINGLE_MEMFUNCPTR_SIZE + 3 * sizeof(int)>
{
	template<class X, class XFuncType, class GenericMemFuncType>
	inline static GenericClass* Convert(X* pthis, XFuncType function_to_bind,
	                                    GenericMemFuncType& bound_func)
	{
		// The member function pointer is 16 bytes long. We can't use a normal
		// cast, but we can use a union to do the conversion.
		union
		{
			XFuncType func;
			// In VC++ and ICL, an unknown_inheritance member pointer
			// is internally defined as:
			struct
			{
				// points to the actual member function
				GenericMemFuncType m_funcaddress;
				int delta;         // #bytes to be added to the 'this' pointer
				int vtordisp;      // #bytes to add to 'this' to find the vtable
				int vtable_index;  // or 0 if no virtual inheritance
			} s;
		} u;

		// Check that the horrible_cast will work
		static_assert(sizeof(XFuncType) == sizeof(u.s),
		              "Cannot use horrible_cast<>");

		u.func            = function_to_bind;
		bound_func        = u.s.funcaddress;
		int virtual_delta = 0;

		if(u.s.vtable_index)
		{  // Virtual inheritance is used
			// First, get to the vtable.
			// It is 'vtordisp' bytes from the start of the class.
			const int* vtable = *reinterpret_cast<const int* const*>(
			        reinterpret_cast<const char*>(pthis) + u.s.vtordisp);

			// 'vtable_index' tells us where in the table we should be looking.
			virtual_delta =
			        u.s.vtordisp + *reinterpret_cast<const int*>(
			                               reinterpret_cast<const char*>(vtable) +
			                               u.s.vtable_index);
		}
		// The int at 'virtual_delta' gives us the amount to add to 'this'.
		// Finally we can add the three components together. Phew!
		return reinterpret_cast<GenericClass*>(reinterpret_cast<char*>(pthis) +
		                                       u.s.delta + virtual_delta);
	};
};
#endif  // MSVC 7 and greater
}  // namespace detail

////////////////////////////////////////////////////////////////////////////////
//						Fast Delegates, part 2:
//
//	Define the delegate storage, and cope with static functions
////////////////////////////////////////////////////////////////////////////////

// DelegateMemento -- an opaque structure which can hold an arbitary delegate.
// It knows nothing about the calling convention or number of arguments used by
// the function pointed to.
// It supplies comparison operators so that it can be stored in STL collections.
// It cannot be set to anything other than null, nor invoked directly:
// it must be converted to a specific delegate.

// Implementation:
// There are two possible implementations: the Safe method and the Evil method.
//				DelegateMemento - Safe version
//
// This implementation is standard-compliant, but a bit tricky.
// A static function pointer is stored inside the class.
// Here are the valid values:
// +-- Static pointer --+--pThis --+-- pMemFunc-+-- Meaning------+
// |   0				|  0       |   0        | Empty          |
// |   !=0              |(dontcare)|  Invoker   | Static function|
// |   0                |  !=0     |  !=0*      | Method call    |
// +--------------------+----------+------------+----------------+
//  * For Metrowerks, this can be 0. (first virtual function in a
//       single_inheritance class).
// When stored stored inside a specific delegate, the 'dontcare' entries are replaced
// with a reference to the delegate itself. This complicates the = and == operators
// for the delegate class.

//				DelegateMemento - Evil version
//
// For compilers where data pointers are at least as big as code pointers, it is
// possible to store the function pointer in the this pointer, using another
// horrible_cast. In this case the DelegateMemento implementation is simple:
// +--pThis --+-- pMemFunc-+-- Meaning---------------------+
// |    0     |  0         | Empty                         |
// |  !=0     |  !=0*      | Static function or method call|
// +----------+------------+-------------------------------+
//  * For Metrowerks, this can be 0. (first virtual function in a
//    single_inheritance class).
// Note that the Sun C++ and MSVC documentation explicitly state that they
// support static_cast between void * and function pointers.

class DelegateMemento
{
protected:
	// the data is protected, not private, because many
	// compilers have problems with template friends.
	// arbitrary MFP.
	using GenericMemFuncType = void (detail::GenericClass::*)();

	detail::GenericClass* m_pthis;
	GenericMemFuncType    m_pFunction;

public:
	DelegateMemento(): m_pthis(nullptr), m_pFunction(nullptr) {};

	void clear()
	{
		m_pthis     = nullptr;
		m_pFunction = nullptr;
	}

public:
	inline bool IsEqual(const DelegateMemento& x) const
	{
		return m_pthis == x.m_pthis && m_pFunction == x.m_pFunction;
	}

	// Provide a strict weak ordering for DelegateMementos.
	inline bool IsLess(const DelegateMemento& right) const
	{
		// deal with static function pointers first
		if(m_pthis != right.m_pthis)
			return m_pthis < right.m_pthis;
		// There are no ordering operators for member function pointers,
		// but we can fake one by comparing each byte. The resulting ordering is
		// arbitrary (and compiler-dependent), but it permits storage in ordered STL
		// containers.
		return std::memcmp(&m_pFunction, &right.m_pFunction, sizeof(m_pFunction)) <
		       0;
	}
	// BUGFIX (Mar 2005):
	// We can't just compare m_pFunction because on Metrowerks,
	// m_pFunction can be zero even if the delegate is not empty!
	inline bool operator!() const  // Is it bound to anything?
	{
		return (m_pthis == nullptr) && (m_pFunction == nullptr);
	}
	inline bool empty() const  // Is it bound to anything?
	{
		return (m_pthis == nullptr) && (m_pFunction == nullptr);
	}

	size_t Hash() const
	{
		return reinterpret_cast<size_t>(m_pthis) ^
		       detail::horrible_cast<size_t>(m_pFunction);
	}

public:
	DelegateMemento& operator=(const DelegateMemento& right)
	{
		SetMementoFrom(right);
		return *this;
	}

	inline bool operator<(const DelegateMemento& right) { return IsLess(right); }
	inline bool operator>(const DelegateMemento& right)
	{
		return right.IsLess(*this);
	}

	DelegateMemento(const DelegateMemento& right)
	        : m_pthis(right.m_pthis), m_pFunction(right.m_pFunction)
	{
	}

protected:
	void SetMementoFrom(const DelegateMemento& right)
	{
		m_pthis     = right.m_pthis;
		m_pFunction = right.m_pFunction;
	}
};

//						ClosurePtr<>
//
// A private wrapper class that adds function signatures to DelegateMemento.
// It's the class that does most of the actual work.
// The signatures are specified by:
// GenericMemFunc: must be a type of GenericClass member function pointer.
// StaticFuncPtr: must be a type of function pointer with the same signature
// as GenericMemFunc.
// UnvoidStaticFuncPtr: is the same as StaticFuncPtr.
// An outer class, FastDelegateN<>, handles the invoking and creates the
// necessary typedefs.
// This class does everything else.

namespace detail
{
template<class GenericMemFunc, class StaticFuncPtr, class UnvoidStaticFuncPtr>
class ClosurePtr: public DelegateMemento
{
public:
	// These functions are for setting the delegate to a member function.

	// Here's the clever bit: we convert an arbitrary member function into a
	// standard form. XMemFunc should be a member function of class X, but I
	// can't enforce that here. It needs to be enforced by the wrapper class.
	template<class X, class XMemFunc>
	inline void bindmemfunc(X* pthis, XMemFunc function_to_bind)
	{
		m_pthis = SimplifyMemFunc<sizeof(function_to_bind)>::Convert(
		        pthis, function_to_bind, m_pFunction);
	}
	// For const member functions, we only need a const class pointer.
	// Since we know that the member function is const, it's safe to
	// remove the const qualifier from the 'this' pointer with a const_cast.
	// VC6 has problems if we just overload 'bindmemfunc', so we give it a
	// different name.
	template<class X, class XMemFunc>
	inline void bindmemfunc(const X* pthis, XMemFunc function_to_bind)
	{
		m_pthis = SimplifyMemFunc<sizeof(function_to_bind)>::Convert(
		        const_cast<X*>(pthis), function_to_bind, m_pFunction);
	}

	// These functions are required for invoking the stored function
	inline GenericClass*  GetClosureThis() const { return m_pthis; }
	inline GenericMemFunc GetClosureMemPtr() const
	{
		return reinterpret_cast<GenericMemFunc>(m_pFunction);
	}

	// There are a few ways of dealing with static function pointers.
	// There's a standard-compliant, but tricky method.
	// There's also a straightforward hack, that won't work on DOS compilers
	// using the medium memory model. It's so evil that I can't recommend it, but
	// I've implemented it anyway because it produces very nice asm code.

	//				ClosurePtr<> - Evil version (not anymore)
	//
	// For compilers where data pointers are at least as big as code pointers, it
	// is possible to store the function pointer in the this pointer, using
	// another horrible_cast. Invocation isn't any faster, but it saves 4 bytes,
	// and speeds up comparison and assignment. If C++ provided direct language
	// support for delegates, they would produce asm code that was almost
	// identical to this. Note that the Sun C++ and MSVC documentation explicitly
	// state that they support static_cast between void * and function pointers.

	// C++11 and newer standards allow casting void* to function pointers.
	// So it's not evil anymore. :)

	template<class DerivedClass>
	inline void CopyFrom(DerivedClass* /*pParent*/, const DelegateMemento& right)
	{
		SetMementoFrom(right);
	}
	// For static functions, the 'static_function_invoker' class in the parent
	// will be called. The parent then needs to call GetStaticFunction() to find
	// out the actual function to invoke.
	// ******** EVIL, EVIL CODE! *******
	template<class DerivedClass, class ParentInvokerSig>
	inline void bindstaticfunc(DerivedClass*    pParent,
	                           ParentInvokerSig static_function_invoker,
	                           StaticFuncPtr    function_to_bind)
	{
		if(function_to_bind == nullptr)
		{  // cope with assignment to nullptr
			m_pFunction = nullptr;
		}
		else
		{
			// We'll be ignoring the 'this' pointer, but we need to make sure we
			// pass a valid value to bindmemfunc().
			bindmemfunc(pParent, static_function_invoker);
		}

		// WARNING! Evil hack. We store the function in the 'this' pointer!
		// Ensure that there's a compilation failure if function pointers
		// and data pointers have different sizes.
		static_assert(sizeof(GenericClass*) == sizeof(function_to_bind),
		              "Cannot use evil method");

		m_pthis = horrible_cast<GenericClass*>(function_to_bind);
		// MSVC, SunC++ and DMC accept the following (non-standard) code:
		//		m_pthis = static_cast<GenericClass *>(static_cast<void
		//*>(function_to_bind));
		// BCC32, Comeau and DMC accept this method. MSVC7.1 needs __int64
		// instead of long
		//		m_pthis = reinterpret_cast<GenericClass
		//*>(reinterpret_cast<long>(function_to_bind));
	}
	// ******** EVIL, EVIL CODE! *******
	// This function will be called with an invalid 'this' pointer!!
	// We're just returning the 'this' pointer, converted into
	// a function pointer!
	inline UnvoidStaticFuncPtr GetStaticFunction() const
	{
		// Ensure that there's a compilation failure if function pointers
		// and data pointers have different sizes.
		static_assert(sizeof(UnvoidStaticFuncPtr) == sizeof(this),
		              "Cannot use evil method");

		return horrible_cast<UnvoidStaticFuncPtr>(this);
	}

	// Does the closure contain this static function?
	inline bool IsEqualToStaticFuncPtr(StaticFuncPtr funcptr) const
	{
		if(funcptr == nullptr)
			return empty();
		// For the Evil method, if it doesn't actually contain a static function,
		// this will return an arbitrary value that is not equal to any valid
		// function pointer.
		else
			return (funcptr == reinterpret_cast<StaticFuncPtr>(GetStaticFunction()));
	}
};
}  // namespace detail

////////////////////////////////////////////////////////////////////////////////
//						Fast Delegates, part 3:
//
//				Wrapper classes to ensure type safety
////////////////////////////////////////////////////////////////////////////////

// Once we have the member function conversion templates, it's easy to make the
// wrapper classes. So that they will work with as many compilers as possible,
// the classes are of the form
//   FastDelegate3<int, char *, double>
// They can cope with any combination of parameters. The max number of parameters
// allowed is 8, but it is trivial to increase this limit.
// Note that we need to treat const member functions seperately.
// All this class does is to enforce type safety, and invoke the delegate with
// the correct list of parameters.

// Because of the weird rule about the class of derived member function pointers,
// you sometimes need to apply a downcast to the 'this' pointer.
// This is the reason for the use of "implicit_cast<X*>(pthis)" in the code below.
// If CDerivedClass is derived from CBaseClass, but doesn't override
// SimpleVirtualFunction, without this trick you'd need to write:
//		MyDelegate(static_cast<CBaseClass *>(&d),
//&CDerivedClass::SimpleVirtualFunction);
// but with the trick you can write
//		MyDelegate(&d, &CDerivedClass::SimpleVirtualFunction);

// RetType is the type the compiler uses in compiling the template. For VC6,
// it cannot be void. DesiredRetType is the real type which is returned from
// all of the functions. It can be void.

// Implicit conversion to "bool" is achieved using the safe_bool idiom,
// using member data pointers (MDP). This allows "if (dg)..." syntax
// Because some compilers (eg codeplay) don't have a unique value for a zero
// MDP, an extra padding member is added to the SafeBool struct.
// Some compilers (eg VC6) won't implicitly convert from 0 to an MDP, so
// in that case the static function constructor is not made explicit; this
// allows "if (dg==0) ..." to compile.

////////////////////////////////////////////////////////////////////////////////
//                      Fast Delegates, part 4:
//
//              Delegate<> class (Original author: Jody Hagins)
//  Allows boost::function style syntax like:
//          Delegate< double (int, long) >
// instead of:
//          Delegate< int, long, double >
////////////////////////////////////////////////////////////////////////////////

template<typename>
class Delegate;

template<typename RetType, typename... Args>
class Delegate<RetType(Args...)>
{
private:
	using DesiredRetType    = typename detail::DefaultVoidToVoid<RetType>::type;
	using StaticFunctionPtr = DesiredRetType (*)(Args...);
	using UnvoidStaticFunctionPtr = RetType (*)(Args...);
	using GenericMemFn            = RetType (detail::GenericClass::*)(Args...);
	using ClosureType = detail::ClosurePtr<GenericMemFn, StaticFunctionPtr,
	                                       UnvoidStaticFunctionPtr>;

	ClosureType m_Closure;

public:
	// Typedefs to aid generic programming
	using type = Delegate;

	// Construction and comparison functions
	Delegate() { Clear(); }
	Delegate(const Delegate& x) { m_Closure.CopyFrom(this, x.m_Closure); }

	void operator=(const Delegate& x) { m_Closure.CopyFrom(this, x.m_Closure); }
	bool operator==(const Delegate& x) const
	{
		return m_Closure.IsEqual(x.m_Closure);
	}

	bool operator!=(const Delegate& x) const
	{
		return !m_Closure.IsEqual(x.m_Closure);
	}

	bool operator<(const Delegate& x) const { return m_Closure.IsLess(x.m_Closure); }
	bool operator>(const Delegate& x) const { return x.m_Closure.IsLess(m_Closure); }

	// Binding to non-const member functions
	template<typename X, typename Y>
	Delegate(Y* pthis, DesiredRetType (X::*function_to_bind)(Args... args))
	{
		m_Closure.bindmemfunc(detail::implicit_cast<X*>(pthis), function_to_bind);
	}

	template<typename X, typename Y>
	inline void Bind(Y* pthis, DesiredRetType (X::*function_to_bind)(Args... args))
	{
		m_Closure.bindmemfunc(detail::implicit_cast<X*>(pthis), function_to_bind);
	}

	template<typename X, typename Y>
	Delegate(Y& pthis, DesiredRetType (X::*function_to_bind)(Args... args))
	{
		m_Closure.bindmemfunc(detail::implicit_cast<X*>(std::addressof(pthis)),
		                      function_to_bind);
	}

	template<typename X, typename Y>
	inline void Bind(Y& pthis, DesiredRetType (X::*function_to_bind)(Args... args))
	{
		m_Closure.bindmemfunc(detail::implicit_cast<X*>(std::addressof(pthis)),
		                      function_to_bind);
	}

	// Binding to const member functions.
	template<typename X, typename Y>
	Delegate(const Y* pthis,
	         DesiredRetType (X::*function_to_bind)(Args... args) const)
	{
		m_Closure.bindmemfunc(detail::implicit_cast<const X*>(pthis),
		                      function_to_bind);
	}

	template<typename X, typename Y>
	inline void Bind(const Y* pthis,
	                 DesiredRetType (X::*function_to_bind)(Args... args) const)
	{
		m_Closure.bindmemfunc(detail::implicit_cast<const X*>(pthis),
		                      function_to_bind);
	}

	template<typename X, typename Y>
	Delegate(const Y& pthis,
	         DesiredRetType (X::*function_to_bind)(Args... args) const)
	{
		m_Closure.bindmemfunc(detail::implicit_cast<const X*>(std::addressof(pthis)),
		                      function_to_bind);
	}

	template<typename X, typename Y>
	inline void Bind(const Y& pthis,
	                 DesiredRetType (X::*function_to_bind)(Args... args) const)
	{
		m_Closure.bindmemfunc(detail::implicit_cast<const X*>(std::addressof(pthis)),
		                      function_to_bind);
	}

	// Static functions. We convert them into a member function call.
	// This constructor also provides implicit conversion
	Delegate(DesiredRetType (*function_to_bind)(Args... args))
	{
		Bind(function_to_bind);
	}

	// for efficiency, prevent creation of a temporary
	void operator=(DesiredRetType (*function_to_bind)(Args... args))
	{
		Bind(function_to_bind);
	}

	inline void Bind(DesiredRetType (*function_to_bind)(Args... args))
	{
		m_Closure.bindstaticfunc(this, &Delegate::InvokeStaticFunction,
		                         function_to_bind);
	}

	// Invoke the delegate
	inline RetType operator()(Args... args) const
	{
		return std::invoke(m_Closure.GetClosureMemPtr(), m_Closure.GetClosureThis(),
		                   std::forward<Args>(args)...);
	}

	inline RetType Invoke(Args... args) const
	{
		return operator()(std::forward<Args>(args)...);
	}

	// Implicit conversion to "bool" using the safe_bool idiom
private:
	struct SafeBoolStruct
	{
		int               a_data_pointer_to_this_is_0_on_buggy_compilers;
		StaticFunctionPtr m_nonzero;
	};

	using UselessTypedef        = SafeBoolStruct;
	using unspecified_bool_type = StaticFunctionPtr SafeBoolStruct::*;

public:
	operator unspecified_bool_type() const
	{
		return Empty() ? 0 : &SafeBoolStruct::m_nonzero;
	}

	// necessary to allow ==nullptr to work despite the safe_bool idiom
	inline bool operator==(StaticFunctionPtr funcptr) const
	{
		return m_Closure.IsEqualToStaticFuncPtr(funcptr);
	}

	inline bool operator!=(StaticFunctionPtr funcptr) const
	{
		return !m_Closure.IsEqualToStaticFuncPtr(funcptr);
	}

	inline bool operator!() const
	{  // Is it bound to anything?
		return !m_Closure;
	}

	inline bool Empty() const { return !m_Closure; }
	void        Clear() { m_Closure.clear(); }

	// Conversion to and from the DelegateMemento storage class
	const DelegateMemento& GetMemento() const { return m_Closure; }
	void SetMemento(const DelegateMemento& any) { m_Closure.CopyFrom(this, any); }

private:  // Invoker for static functions
	RetType InvokeStaticFunction(Args... args) const
	{
		return std::invoke(m_Closure.GetStaticFunction(),
		                   std::forward<Args>(args)...);
	}
};

////////////////////////////////////////////////////////////////////////////////
//                      Fast Delegates, part 5:
//
//              MakeDelegate() helper function
//
//          MakeDelegate(&x, &X::func) returns a fastdelegate of the type
//          necessary for calling x.func() with the correct number of arguments.
//          This makes it possible to eliminate many typedefs from user code.
////////////////////////////////////////////////////////////////////////////////

// Also declare overloads of a MakeDelegate() global function to
// reduce the need for typedefs.
// We need seperate overloads for const and non-const member functions.
// Also, because of the weird rule about the class of derived member function
// pointers, implicit downcasts may need to be applied later to the 'this' pointer.
// That's why two classes (X and Y) appear in the definitions. Y must be implicitly
// castable to X.

template<typename RetType, typename... Args>
Delegate<RetType(Args...)> MakeDelegate(RetType (*func)(Args...))
{
	return Delegate<RetType(Args...)>(func);
}

template<typename RetType, typename X, typename Y, typename... Args>
Delegate<RetType(Args...)> MakeDelegate(Y* x, RetType (X::*func)(Args...))
{
	return Delegate<RetType(Args...)>(x, func);
}

template<typename RetType, typename X, typename Y, typename... Args>
Delegate<RetType(Args...)> MakeDelegate(Y* x, RetType (X::*func)(Args...) const)
{
	return Delegate<RetType(Args...)>(x, func);
}

template<typename RetType, typename X, typename Y, typename... Args>
Delegate<RetType(Args...)> MakeDelegate(Y& x, RetType (X::*func)(Args...))
{
	return Delegate<RetType(Args...)>(std::addressof(x), func);
}

template<typename RetType, typename X, typename Y, typename... Args>
Delegate<RetType(Args...)> MakeDelegate(Y& x, RetType (X::*func)(Args...) const)
{
	return Delegate<RetType(Args...)>(std::addressof(x), func);
}

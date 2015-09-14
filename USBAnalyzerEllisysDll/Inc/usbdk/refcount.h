// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

/// @file RefCount.h
/// @brief
///		USB Analysis SDK reference counting declarations.
/////////////////////////////////////////////////////////////////////////////

#pragma once

namespace usbdk {

/// Base class for reference counting.
class RefCount
{
private:
	long m_refCount;

public:
	/// @brief 
	///		Constructs a RefCount object.
	RefCount() :
		m_refCount(0)
	{
	}

	/// @brief 
	///		Constructs a RefCount object from a copy.
	/// @param
	///		copy - An existing RefCount object to be copied into this object.
	RefCount(const RefCount& copy)
	{
		// Don't copy refCount
	}

	/// @brief 
	///		Assignment operator.
	/// @param
	///		copy - An existing RefCount object to be copied into this object.
	RefCount& operator=(const RefCount& copy)
	{
		// Don't copy refCount
		return *this;
	}

	/// @brief 
	///		Destructs a RefCount object.
	virtual ~RefCount()
	{
	}

public:
	/// @brief 
	///		Increments the reference count on the object.
	/// @remarks 
	///		It is usually better to use RefCountPtr to automatically 
	///		manage reference counting instead of doing it manually.
	/// @return
	///		This function returns the new incremented reference count 
	///		on the object. This value may be useful for diagnostics or testing.
	/// @seealso
	///		RefCountPtr
	long AddRef()  
	{
		return ++m_refCount; 
	}

	/// @brief 
	///		Decrements the reference count on the object.
	/// @remarks 
	///		It is usually better to use RefCountPtr to automatically 
	///		manage reference counting instead of doing it manually.
	/// @return
	///		This function returns the new decremented reference count 
	///		on the object. In debug builds, the return value may be 
	///		useful for diagnostics or testing. 
	/// @seealso
	///		RefCountPtr
	long Release() 
	{
		if(m_refCount == 0) 
		{
			return 0;
		}

		long refCount = --m_refCount;

		if(refCount == 0) 
		{
			delete this;
		}

		return refCount; 
	}
};

/// @brief 
///		Creates an instance of a reference counted object.
/// @return
///		This function returns a new object of type T with a reference count of 1.
/// @seealso
///		RefCount
template<class T>
T* CreateInstance()
{
	T* p = new T();
	p->AddRef();
	return p;
}

/// Smart pointer of reference counted object.
template<class T>
class RefCountPtr
{
private:
	T* m_p;

public:
	/// @brief 
	///		Constructs a new RefCountPtr object.
	RefCountPtr() throw() : 
		m_p(NULL)
	{
	}

	/// @brief 
	///		Constructs a new RefCountPtr object.
	/// @param 
	///		p - Used to initialize the internal referenced counted pointer.
	/// @param 
	///		addRef - True if an AddRef is needed on the pointer, false otherwise (default).
	RefCountPtr(T* p, bool addRef=false) throw() : 
		m_p(p)
	{
		if(addRef)
		{
			AddRef();
		}
	}

	/// @brief 
	///		Constructs a new RefCountPtr object.
	/// @param 
	///		copy - An existing RefCountPtr object to be copied into this object.
	RefCountPtr(const RefCountPtr<T>& copy) throw() : 
		m_p(copy.m_p)
	{
		AddRef();
	}

	/// @brief 
	///		Destructs a RefCountPtr object.
	~RefCountPtr() throw()
	{
		Release();
	}

public:
	/// Reference operator.
	T** operator&() throw()
	{
		ASSERT(m_p==NULL);
		return &m_p;
	}

	/// Pointer operator.
	T& operator*() const throw()
	{
		ASSERT(m_p != NULL);
		return *m_p;
	}

	/// pointer-to-members operator.
	T* operator->() const throw()
	{
		ASSERT(m_p != NULL);
		return m_p;
	}

	/// Cast operator.
	operator T*() const throw()
/////	
	{
		return m_p;
	}

	/// Template type conversion operator.
	template<class Other>
	operator RefCountPtr<Other>() throw()
	{
		return (RefCountPtr<Other>(*this));
	}

	/// Assignment operator.
	T* operator=(T* p) throw()
	{
		if(p != NULL)
		{
			p->AddRef();
		}
		Release();
		m_p = p;
		return m_p;
	}

	/// Assignment operator.
	T* operator=(RefCountPtr<T>& copy) throw()
	{
		copy.AddRef();
		Release();
		m_p = copy.m_p;
		return m_p;
	}

	/// Assignment operator.
	template<class Other>
	RefCountPtr<T>& operator=(RefCountPtr<Other>& copy) throw()
	{
		copy.AddRef();
		Release();
		m_p = static_cast<T*>((Other*) copy);
		return (*this);
	}

public:
	/// @brief 
	///		Increments the reference count on the pointer.
	void AddRef() throw()
	{
		if(m_p != NULL)
		{
			m_p->AddRef();
		}
	}

	/// @brief 
	///		Decrements the reference count on the pointer.
	void Release() throw()
	{
		if(m_p != NULL)
		{
			m_p->Release();
			m_p = NULL;
		}
	}

	/// @brief 
	///		Takes ownership of an existing pointer.
	/// @param 
	///		p - The object will take ownership of this pointer. 
	void Attach(T* p) throw()
	{
		Release();
		m_p = p;
	}

	/// @brief 
	///		Releases ownership of the pointer.
	/// @return
	///		Returns a copy of the pointer.
	T* Detach() throw()
	{
		T* p = m_p;
		m_p = NULL;
		return p;
	}
};

} // End of the usbdk namespace

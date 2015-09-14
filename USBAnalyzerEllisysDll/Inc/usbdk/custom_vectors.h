// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//

// custom_vectors.h
/////////////////////////////////////////////////////////////////////////////

#include <cstddef>
#include <stdexcept>
#include <xmemory>

/////////////////////////////////////////////////////////////////////////////
// small_vector

template<typename T>
class small_vector
{
public:
	typedef T                value_type;
	typedef T*               iterator;
	typedef const T*         const_iterator;
	typedef T&               reference;
	typedef const T&         const_reference;
	typedef T*               pointer;
	typedef const T*         const_pointer;
	typedef size_t           size_type;
	typedef ptrdiff_t        difference_type;
	typedef small_vector<T>  my_type;

public:
	enum { max_embedded_size = sizeof(T*)/sizeof(T) };

private:
#pragma pack(push, 1)
	size_type m_size;

	union
	{
		T* m_data;
		T  m_embeddedData[max_embedded_size];
	};
#pragma pack(pop)

public:
	// constructors
	small_vector() :
		m_size(0),
		m_data(NULL)
	{
	}

	explicit small_vector(size_type size) :
		m_size(size),
		m_data(NULL)
	{
		buy(size);
	}

	small_vector(size_type size, const T* buffer) :
		m_size(0),
		m_data(NULL)
	{
		buy(size);
		copy(buffer);
	}

	small_vector(const my_type& right) :
		m_size(0),
		m_data(NULL)
	{
		buy(right.size());
		copy(&right[0]);
	}

	template<class Iterator>
	small_vector(Iterator first, Iterator last) :
		m_size(0),
		m_data(NULL)
	{
		buy(last-first);
		copy(&(*first));
	}

	~small_vector()
	{
		tidy();
	}

	// iterator support
	iterator begin() { return ptr(); }
	const_iterator begin() const { return ptr(); }
	iterator end() { return ptr()+size(); }
	const_iterator end() const { return ptr()+size(); }

	// operator[]
	reference operator[](size_type i) { return ref(i); }
	const_reference operator[](size_type i) const { return ref(i); }

	// at() with range check
	reference at(size_type i) { rangecheck(i); return ref(i); }
	const_reference at(size_type i) const { rangecheck(i); return ref(i); }

	// front() and back()
	reference front() { return ref(); }
	const_reference front() const { return ref(); }
	reference back() { return ref(size()-1); }
	const_reference back() const { return ref(size()-1); }

	// clear support
	void clear() { tidy(); }

	// size
	size_type size() const { return m_size; }
	bool empty() const { return m_size==0; }
	size_type max_size() const { return m_size; }

	// swap (note: linear complexity)
	void swap (my_type& y) 
	{
		std::swap_ranges(begin(),end(),y.begin());
	}

	// assignment
	my_type& operator= (const my_type& right) 
	{
		tidy();
		buy(right.size());
		copy(&right[0]);
		return *this;
	}

	// assign one value to all elements
	void assign(const T& value)
	{
		std::fill_n(begin(),size(),value);
	}

	// assign support
	void assign(size_type size, const T* buffer)
	{
		tidy();
		buy(size);
		copy(buffer);
	}

	template<class Iterator>
	void assign(Iterator first, Iterator last)
	{
		tidy();
		buy(last-first);
		copy(&(*first));
	}

private:
	// check range (may be private because it is static)
	static void rangecheck (size_type i) 
	{
		if(i >= size()) 
		{ 
			throw std::range_error("small_vector"); 
		}
	}

	void buy(size_type capacity)
	{
		if(capacity > max_embedded_size)
		{
			m_data = new T[capacity];
		}

		m_size = capacity;
	}

	void copy(const T* buffer)
	{
		memcpy(ptr(), buffer, m_size);
	}

	void tidy()
	{
		if(m_size > max_embedded_size)
		{
			delete[] m_data;
			m_data = NULL;
		}

		m_size = 0;
	}

	iterator ptr()
	{
		if(m_size > max_embedded_size) return m_data;
		else return m_embeddedData;
	}

	const_iterator ptr() const
	{
		if(m_size > max_embedded_size) return m_data;
		else return m_embeddedData;
	}

	iterator ptr(size_type i)
	{
		if(m_size > max_embedded_size) return m_data+i;
		else return m_embeddedData+i;
	}

	const_iterator ptr(size_type i) const
	{
		if(m_size > max_embedded_size) return m_data+i;
		else return m_embeddedData+i;
	}

	reference ref()
	{
		if(m_size > max_embedded_size) return *m_data;
		else return *m_embeddedData;
	}

	const_reference ref() const
	{
		if(m_size > max_embedded_size) return *m_data;
		else return *m_embeddedData;
	}

	reference ref(size_type i)
	{
		if(m_size > max_embedded_size) return m_data[i];
		else return m_embeddedData[i];
	}

	const_reference ref(size_type i) const
	{
		if(m_size > max_embedded_size) return m_data[i];
		else return m_embeddedData[i];
	}
};

// comparisons
template<class T>
bool operator== (const small_vector<T>& x, const small_vector<T>& y) 
{
	return std::equal(x.begin(), x.end(), y.begin());
}
template<class T>
bool operator< (const small_vector<T>& x, const small_vector<T>& y) 
{
	return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
}
template<class T>
bool operator!= (const small_vector<T>& x, const small_vector<T>& y) 
{
	return !(x==y);
}
template<class T>
bool operator> (const small_vector<T>& x, const small_vector<T>& y) 
{
	return y<x;
}
template<class T>
bool operator<= (const small_vector<T>& x, const small_vector<T>& y) 
{
	return !(y<x);
}
template<class T>
bool operator>= (const small_vector<T>& x, const small_vector<T>& y) 
{
	return !(x<y);
}

// global swap()
template<class T>
inline void swap (small_vector<T>& x, small_vector<T>& y) 
{
	x.swap(y);
}

/////////////////////////////////////////////////////////////////////////////
// ref_vector

template<class T>
class ref_vector 
{
public:
	typedef T               value_type;
	typedef const T*        const_iterator;
	typedef const T&        const_reference;
	typedef const T*        const_pointer;
	typedef size_t          size_type;
	typedef ptrdiff_t       difference_type;
	typedef ref_vector<T>   my_type;

private:
	size_type m_size;
	const T* m_data;

public:
	// constructors
	ref_vector() :
		m_size(0),
		m_data(NULL)
	{
	}

	ref_vector(size_type size, const T* buffer) :
		m_size(size),
		m_data(buffer)
	{
	}

	ref_vector(const my_type& right) :
		m_size(right.m_size),
		m_data(right.m_data)
	{
	}

	template<class Iterator>
	ref_vector(Iterator first, Iterator last) :
		m_size(last-first),
		m_data(&(*first))
	{
	}

	// iterator support
	const_iterator begin() const { return ptr(); }
	const_iterator end() const { return ptr()+size(); }

	// operator[]
	const_reference operator[](size_type i) const { return ref(i); }

	// at() with range check
	const_reference at(size_type i) const { rangecheck(i); return ref(i); }

	// front() and back()
	const_reference front() const { return ref(); }
	const_reference back() const { return ref(size()-1); }

	// size
	size_type size() const { return m_size; }
	bool empty() const { return m_size==0; }
	size_type max_size() const { return m_size; }

	// assignment with type conversion
	template <typename T2>
	my_type& operator= (const ref_vector<T2>& right) 
	{
		m_size = right.m_size;
		m_buffer = right.m_buffer;
		return *this;
	}

private:
	// check range (may be private because it is static)
	static void rangecheck (size_type i) 
	{
		if(i >= size()) 
		{ 
			throw std::range_error("ref_vector"); 
		}
	}

	const_iterator ptr() const
	{
		return m_data;
	}

	const_iterator ptr(size_type i) const
	{
		return m_data+i;
	}

	const_reference ref() const
	{
		return m_data[0];
	}

	const_reference ref(size_type i) const
	{
		return m_data[i];
	}
};

// comparisons
template<class T>
bool operator== (const ref_vector<T>& x, const ref_vector<T>& y) 
{
	return std::equal(x.begin(), x.end(), y.begin());
}
template<class T>
bool operator< (const ref_vector<T>& x, const ref_vector<T>& y) 
{
	return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
}
template<class T>
bool operator!= (const ref_vector<T>& x, const ref_vector<T>& y) 
{
	return !(x==y);
}
template<class T>
bool operator> (const ref_vector<T>& x, const ref_vector<T>& y) 
{
	return y<x;
}
template<class T>
bool operator<= (const ref_vector<T>& x, const ref_vector<T>& y) 
{
	return !(y<x);
}
template<class T>
bool operator>= (const ref_vector<T>& x, const ref_vector<T>& y) 
{
	return !(x<y);
}

#ifndef _ANY_STRING_PROXY_
#define _ANY_STRING_PROXY_
#include "AnyString.h"
#include "AnyStringData.h"
#include "AnyStringRealization.h"
#include "Macros.h"
#include <ostream>

ANY_BEGIN

template<typename CharType, typename CharTraits>
class AnyString<CharType, CharTraits>::CharProxy
{
public:
	friend AnyString<CharType, CharTraits>;
public:

	/*
	* There's no other way to define operator<<
	* as the compiler merely cannot deduce the
	* appropriate type to instantiate the template,
	* provided Proxy is a nested class (nested within template AnyString) 
	*/

	friend std::ostream& operator<<(std::ostream& o, const CharProxy& proxy)
	{
		return o << static_cast<const char_type&>(proxy);
	}

	friend std::wostream& operator<<(std::wostream& o, const CharProxy& proxy)
	{
		return o << static_cast<const char_type&>(proxy);
	}

	friend auto operator<=>(const CharProxy& a, const CharProxy& b)
	{
		return static_cast<const char_type&>(a) <=> static_cast<const char_type&>(b);
	}

	friend bool operator== (const CharProxy& a, const CharProxy& b)
	{
		return static_cast<const char_type&>(a) == static_cast<const char_type&>(b);
	}
	
public:
	using char_type = AnyString::char_type;
	using char_�ref = AnyString::char_�ref;
	using char_traits = AnyString::char_traits;
	using size_type = AnyString::size_type;
public:
	~CharProxy() = default;
private:
	CharProxy(AnyString&, const size_type);
public:
	//char_type* operator&();     - causes unexpected unshareability
	//operator char_type() const; - causes ambiguity when casting to char_type
	//operator char_type& ();     - causes unexpected unshareability
	const char_type* operator&() const;
	char_type* char_ptr();
	char_type char_copy() const;
	char_type& char_ref();
	operator const char_type& () const;
	CharProxy& operator=(char_�ref c);
private:
	void get_infected();
private:
	AnyString<char_type, char_traits>& _proxyship;
	const size_type _index;
};

PROXY_MEMBER CharProxy(AnyString& str, const size_type index) :
	_proxyship(str),
	_index(index)
{
}

PROXY_METHOD operator&() const -> const char_type*
{
	return &_proxyship._data->chars()[_index];
}

PROXY_METHOD char_ptr() -> char_type*
{
	get_infected();
	return &_proxyship._data->chars()[_index];
}

PROXY_METHOD char_copy() const -> char_type
{
	return _proxyship.read_at(_index);
}

PROXY_METHOD char_ref() -> char_type&
{
	get_infected();
	return _proxyship._data->chars()[_index];
}

PROXY_MEMBER operator const char_type& () const
{
	return _proxyship.read_at(_index);
}

PROXY_METHOD operator=(char_�ref c) -> CharProxy&
{
	_proxyship.write_at(c, _index);
	return *this;
}

PROXY_METHOD get_infected() -> void
{
	if (_proxyship._data->is_shareable() && _proxyship._data.use_count() > 1)
		_proxyship._data = _proxyship._data->clone();
	_proxyship._data->set_shareable(false);
}

ANY_END

#endif // !_ANY_STRING_PROXY_
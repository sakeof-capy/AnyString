#ifndef _STRING_DATA_
#define _STRING_DATA_
#include "AnyString.h"

template<typename CharType, typename CharTraits>
class AnyString<CharType, CharTraits>::string_data
{
public:
	using char_type = AnyString::char_type;
	using char_�ref = AnyString::char_�ref;
	using char_traits = AnyString::char_traits;
	using size_type = AnyString::size_type;
	using data_ptr = std::shared_ptr<string_data>;
public:
	string_data(const char_type* const, const size_type);
	string_data(const char_type);
	string_data(const string_data&) = delete;
	~string_data();
public:
	data_ptr getOwnCopy();
	void assign(const char_type* const, const size_type);
	void setShareable(bool sharable) noexcept;
	bool isShareable() const noexcept;
	size_type size() const noexcept;
	char_type* chars() noexcept;
public:
	string_data& operator= (const string_data&) = delete;
private:
	void copy_elems_from(const char_type* const, const size_type);
private:
	char_type* _chrs;
	size_type _size;
	bool _shareable;
};

template<typename CharType, typename CharTraits>
inline AnyString<CharType, CharTraits>::string_data::string_data(const char_type* const str, const size_type size) :
	_chrs(new char_type[size + 1]), //Bad practice - solved by allocators
	_size(size),
	_shareable(true)
{
	copy_elems_from(str, size + 1);
}

template<typename CharType, typename CharTraits>
inline AnyString<CharType, CharTraits>::string_data::string_data(const char_type c) :
	_chrs(new char_type[2] {c, 0}), //Bad practice - solved by allocators
	_size(1u),
	_shareable(true)
{
}

template<typename CharType, typename CharTraits>
inline AnyString<CharType, CharTraits>::string_data::~string_data()
{
	delete _chrs;
	_chrs = nullptr;
}

template<typename CharType, typename CharTraits>
inline void AnyString<CharType, CharTraits>::string_data::setShareable(bool shareable) noexcept
{
	_shareable = shareable;
}

template<typename CharType, typename CharTraits>
inline bool AnyString<CharType, CharTraits>::string_data::isShareable() const noexcept
{
	return _shareable;
}

template<typename CharType, typename CharTraits>
inline auto AnyString<CharType, CharTraits>::string_data::size() const noexcept -> size_type
{
	return _size;
}

template<typename CharType, typename CharTraits>
inline auto AnyString<CharType, CharTraits>::string_data::chars() noexcept -> char_type*
{
	return _chrs;
}

template<typename CharType, typename CharTraits>
inline void AnyString<CharType, CharTraits>::string_data::copy_elems_from(const char_type* const str, const size_type size)
{
	strcpy_s(_chrs, size, str);
}

#endif // !_STRING_DATA_
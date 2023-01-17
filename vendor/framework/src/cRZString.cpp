#include <algorithm>
#include "cRZString.h"

cRZString::cRZString()
{
}

cRZString::cRZString(const char* src) :
	std::string(src)
{
}

cRZString::cRZString(const char* src, uint32_t len) :
	std::string(src, len)
{
}

cRZString::cRZString(cIGZString const& src) :
	std::string(src.Data())
{
}

cRZString::cRZString(cIGZString const* src)
{
	if (src != NULL)
	{
		assign(src->Data());
	}
}

cRZString::~cRZString()
{
}

bool cRZString::QueryInterface(uint32_t iid, void** outPtr)
{
	if (iid == GZIID_cIGZString)
	{
		*outPtr = static_cast<cIGZString*>(this);
		AddRef();
		return true;
	}
	else if (iid == GZIID_cIGZUnknown)
	{
		*outPtr = static_cast<cIGZUnknown*>(this);
		AddRef();
		return true;
	}

	return false;
}

uint32_t cRZString::AddRef(void)
{
	return ++refCount;
}

uint32_t cRZString::Release(void)
{
	return --refCount;
}

void cRZString::FromChar(char const* otherStr)
{
	if (otherStr == NULL)
	{
		erase();
	}
	else
	{
		assign(otherStr);
	}
}

void cRZString::FromChar(char const* otherStr, uint32_t length)
{
	if (otherStr == NULL)
	{
		erase();
	}
	else
	{
		assign(otherStr, length);
	}
}

char const* cRZString::ToChar() const
{
	return c_str();
}

char const* cRZString::Data() const
{
	return c_str();
}

uint32_t cRZString::Strlen() const
{
	return length();
}

bool cRZString::IsEqual(cIGZString const* other, bool caseSensitive) const
{
	if (std::string::length() != other->Strlen())
	{
		return false;
	}

	if (!caseSensitive)
	{
		cRZString lowercaseThis(this);
		cRZString lowercaseOther(other);

		lowercaseThis.MakeLower();
		lowercaseOther.MakeLower();

		return (std::string const&)lowercaseThis == (std::string const&)lowercaseOther;
	}
	else
	{
		char const* otherData = other->Data();
		return strncmp(c_str(), otherData, length()) == 0;
	}
}

bool cRZString::IsEqual(cIGZString const& other, bool caseSensitive) const
{
	return IsEqual(&other, caseSensitive);
}

bool cRZString::IsEqual(char const* other, uint32_t otherLen, bool caseSensitive) const
{
	if (otherLen == -1)
	{
		otherLen = strlen(other);
	}

	if (length() != otherLen)
	{
		return false;
	}

	if (!caseSensitive)
	{
		cRZString lowercaseThis(this);
		cRZString lowercaseOther(other, otherLen);

		lowercaseThis.MakeLower();
		lowercaseOther.MakeLower();

		return (std::string const&)lowercaseThis == (std::string const&)lowercaseOther;
	}
	else
	{
		return strncmp(c_str(), other, otherLen) == 0;
	}
}

int cRZString::CompareTo(cIGZString const& other, bool caseSensitive) const
{
	// TODO
	return 0;
}

int cRZString::CompareTo(char const* other, uint32_t otherLen, bool caseSensitive) const
{
	// TODO
	return 0;
}

int cRZString::CompareTo(cRZString const& other) const
{
	return compare(other);
}

cIGZString& cRZString::operator=(cIGZString const& other)
{
	Copy(other);
	return *this;
}

cRZString& cRZString::operator=(cRZString const& other)
{
	assign(other);
	return *this;
}

void cRZString::Copy(cIGZString const& src)
{
	FromChar(src.Data(), src.Strlen());
}

void cRZString::Resize(uint32_t newLength)
{
	resize(newLength);
}

cIGZString& cRZString::Append(char const* src, uint32_t srcLen)
{
	append(src, srcLen);
	return *this;
}

cIGZString& cRZString::Append(cIGZString const& src)
{
	append(src.ToChar(), src.Strlen());
	return *this;
}

cIGZString& cRZString::Insert(uint32_t position, char const* src, uint32_t srcLen)
{
	insert(position, src, srcLen);
	return *this;
}

cIGZString& cRZString::Insert(uint32_t position, cIGZString const& src)
{
	insert(position, src.ToChar(), src.Strlen());
	return *this;
}

cIGZString& cRZString::Replace(uint32_t position, char const* src, uint32_t srcLen)
{
	replace(position, srcLen, src);
	return *this;
}

cIGZString& cRZString::Replace(uint32_t position, cIGZString const& src)
{
	replace(position, src.Strlen(), src.ToChar());
	return *this;
}

cIGZString& cRZString::Erase(uint32_t position, uint32_t length)
{
	erase(position, length);
	return *this;
}

uint32_t cRZString::Find(char const* needle, uint32_t position, bool caseSensitive) const
{
	if (!caseSensitive)
	{
		cRZString lowercaseThis(this);
		cRZString lowercaseNeedle(needle);

		lowercaseThis.MakeLower();
		lowercaseNeedle.MakeLower();

		return lowercaseThis.find(lowercaseNeedle, position);
	}
	else
	{
		return find(needle, position);
	}
}

uint32_t cRZString::Find(cIGZString const& needle, uint32_t position, bool caseSensitive) const
{
	if (!caseSensitive)
	{
		cRZString lowercaseThis(this);
		cRZString lowercaseNeedle(needle);

		lowercaseThis.MakeLower();
		lowercaseNeedle.MakeLower();

		return lowercaseThis.find(lowercaseNeedle, position);
	}
	else
	{
		return find(needle.ToChar(), position);
	}
}

uint32_t cRZString::Find(cRZString const& needle, uint32_t position)
{
	return find(needle, position);
}

uint32_t cRZString::RFind(char const* needle, uint32_t position, bool caseSensitive) const
{
	if (!caseSensitive)
	{
		cRZString lowercaseThis(this);
		cRZString lowercaseNeedle(needle);

		lowercaseThis.MakeLower();
		lowercaseNeedle.MakeLower();

		return lowercaseThis.rfind(lowercaseNeedle, position);
	}
	else
	{
		return rfind(needle, position);
	}
}

uint32_t cRZString::RFind(cIGZString const& needle, uint32_t position, bool caseSensitive) const
{
	if (!caseSensitive)
	{
		cRZString lowercaseThis(this);
		cRZString lowercaseNeedle(needle);

		lowercaseThis.MakeLower();
		lowercaseNeedle.MakeLower();

		return lowercaseThis.rfind(lowercaseNeedle, position);
	}
	else
	{
		return rfind(needle.ToChar(), position);
	}
}

uint32_t cRZString::RFind(cRZString const& needle, uint32_t position)
{
	return rfind(needle, position);
}

void cRZString::Sprintf(char const* format, ...)
{
	// TODO
}

bool cRZString::BeginsWith(char const* needle, uint32_t needleLen) const
{
	return std::string::find(needle, 0, needleLen) == 0;
}

bool cRZString::EndsWith(char const* needle, uint32_t needleLen) const
{
	if (needleLen > length()) {
		return false;
	}

	return compare(length() - needleLen, needleLen, needle) == 0;
}

void cRZString::Trim()
{
	uint32_t index = find_first_not_of(" \t", 0, 2);
	erase(0, index);

	index = find_last_not_of(" \t", -1, 2);
	erase(index + 1, -1);
}

void cRZString::LTrim()
{
	uint32_t index = find_first_not_of(" \t", 0, 2);
	erase(0, find_first_not_of(" \t", 0, 2));
}

void cRZString::RTrim()
{
	uint32_t index = find_last_not_of(" \t", -1, 2);
	erase(index + 1, -1);
}

void cRZString::MakeLower()
{
	std::transform(begin(), end(), begin(), [](unsigned char c) { return std::tolower(c); });
}

void cRZString::MakeUpper()
{
	std::transform(begin(), end(), begin(), [](unsigned char c) { return std::toupper(c); });
}

void cRZString::Strcat(char const* str)
{
	append(str);
}

void cRZString::Strncpy(char const* str, uint32_t count)
{
	uint32_t len = strlen(str);
	if (len < count)
	{
		count = len;
	}

	assign(str, count);
}

bool cRZString::operator< (const cIGZString& other) const
{
	return CompareTo(other) < 0;
}

bool cRZString::operator> (const cIGZString& other) const
{
	return CompareTo(other) > 0;
}

bool cRZString::operator== (const cIGZString& other) const
{
	return CompareTo(other) == 0;
}

bool cRZString::operator< (char const* other) const
{
	return CompareTo(other) < 0;
}

bool cRZString::operator> (char const* other) const
{
	return CompareTo(other) > 0;
}

bool cRZString::operator== (char const* other) const
{
	return CompareTo(other) == 0;
}
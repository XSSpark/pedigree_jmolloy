/*
 * Copyright (c) 2008 James Molloy, James Pritchett, Jörg Pfähler, Matthew Iselin
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef KERNEL_UTILITIES_STRING_H
#define KERNEL_UTILITIES_STRING_H

/** @addtogroup kernelutilities
 * @{ */

#include <compiler.h>
#include <processor/types.h>

/** String class for ASCII strings
 *\todo provide documentation */
class String
{
  public:
    /** The default constructor does nothing */
    inline String();
    inline explicit String(const char *s);
    inline String(const String &x);
    inline ~String();

    inline String &operator = (const String &x);
    inline String &operator = (const char *s);

    inline size_t length() const PURE;
    inline size_t size() const PURE;

    void assign(const String &x);
    void assign(const char *s);
    void reserve(size_t size);
    void free();

  private:
    /** Pointer to the zero-terminated ASCII string */
    char *m_Data;
    /** The string's length */
    size_t m_Length;
    /** The size of the reserved space for the string */
    size_t m_Size;
};

/** @} */

//
// Part of the implementation
//
String::String()
  : m_Data(0), m_Length(0), m_Size(0)
{
}
String::String(const char *s)
  : m_Data(0), m_Length(0), m_Size(0)
{
  assign(s);
}
String::String(const String &x)
  : m_Data(0), m_Length(0), m_Size(0)
{
  assign(x);
}
String::~String()
{
  free();
}

String &String::operator = (const String &x)
{
  assign(x);
  return *this;
}
String &String::operator = (const char *s)
{
  assign(s);
  return *this;
}

size_t String::length() const
{
  return m_Length;
}
size_t String::size() const
{
  return m_Size;
}

#endif

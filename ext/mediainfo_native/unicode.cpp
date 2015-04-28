#include <ruby.h>
#include <cstdlib>
#include "unicode.h"

MediaInfoDLL::String value_to_mediainfo_string(VALUE s)
{
  size_t  nchars = RSTRING_LEN(s);
  wchar_t buf[nchars + 1];

  nchars = mbstowcs(buf, StringValueCStr(s), nchars);

  if((size_t) (-1) == nchars)
    rb_raise(rb_eArgError, "invalid multi-byte sequence in char array");
  else
    // According to mbstowcs(3), this is not nul-terminated when max characters
    // have been written.
    buf[nchars] = L'\0';

  return MediaInfoDLL::String(buf);
}

VALUE mediainfo_string_to_value(MediaInfoDLL::String s)
{
  size_t nbytes = s.length() * 2;
  char   buf[nbytes + 1];

  nbytes = wcstombs(buf, s.data(), nbytes);

  if((size_t) (-1) == nbytes)
    rb_raise(rb_eArgError, "wide-char sequence not representable in char array");
  else
    // This should actually always be there, just to make sure.
    buf[nbytes] = '\0';

  return rb_locale_str_new_cstr(buf);
}

#include <ruby.h>
#include <ruby/encoding.h>
#include <cstdlib>
#include <iconv.h>
#include "unicode.h"

MediaInfoDLL::String value_to_mediainfo_string(VALUE s)
{
  size_t  nbytes_in  = RSTRING_LEN(s) + 1;
  size_t  nbytes_out = nbytes_in * sizeof(wchar_t);
  wchar_t buf[nbytes_in];
  size_t  rv;

  iconv_t ic = iconv_open("WCHAR_T", rb_enc_get(s)->name);
  if (ic == ((iconv_t) (-1)))
    rb_raise(rb_eStandardError, "iconv_open failure");

  char* src = (char*) StringValueCStr(s);
  char* dst = (char*) buf;

  rv = iconv(&ic, &src, &nbytes_in, &dst, &nbytes_out);

  iconv_close(ic);

  if (rv == (-1))
    rb_raise(rb_eStandardError, "iconv failure");

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

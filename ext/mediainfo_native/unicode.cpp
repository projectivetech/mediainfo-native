#include <ruby.h>
#include <ruby/encoding.h>
#include "unicode.h"

MediaInfoDLL::String value_to_ansi_string(VALUE s)
{
  rb_encoding* enc = rb_enc_find("ANSI");
  VALUE exported = rb_str_export_to_enc(s, enc);
  return MediaInfoDLL::String(StringValueCStr(exported));
}

VALUE ansi_string_to_value(MediaInfoDLL::String s)
{
#ifdef RBX_RUBY_VERSION
  return rb_external_str_new_with_enc(s.c_str(), s.length(), rb_enc_find("ANSI"));
#else
  return rb_external_str_new_with_enc(s.c_str(), s.length(), rb_ascii8bit_encoding());
#endif
}

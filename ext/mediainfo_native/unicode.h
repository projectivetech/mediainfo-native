#ifndef MEDIAINFO_NATIVE_UNICODE_H
#define MEDIAINFO_NATIVE_UNICODE_H

#include <ruby.h>
#include "MediaInfoDLL.h"

MediaInfoDLL::String value_to_ansi_string(VALUE s);
VALUE ansi_string_to_value(MediaInfoDLL::String s);
  
#endif /* MEDIAINFO_NATIVE_UNICODE_H */

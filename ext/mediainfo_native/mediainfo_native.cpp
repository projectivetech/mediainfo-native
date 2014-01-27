#include <string>
#include <ruby.h>
#include "mediainfo_wrapper.h"

using MediaInfoNative::MediaInfoWrapper;

#define GET_WRAPPER(var) \
  MediaInfoWrapper* var; \
  Data_Get_Struct(self, MediaInfoWrapper, var)

extern "C"
{

static
void miw_free(void* ptr) 
{
  delete ((MediaInfoWrapper*) ptr);
}

static 
VALUE miw_new(VALUE klass)
{
  MediaInfoWrapper* miw = new MediaInfoWrapper();
  return Data_Wrap_Struct(klass, 0, miw_free, miw);
}

static
VALUE miw_inform(VALUE self, VALUE path)
{
  Check_Type(path, T_STRING);
  GET_WRAPPER(miw);

  miw->open(path);
  VALUE inform = miw->inform();
  miw->close();

  return inform;
}

void
Init_mediainfo_native()
{
  VALUE mMediaInfoNative = rb_define_module("MediaInfoNative");
  VALUE cMediaInfo       = rb_define_class_under(mMediaInfoNative, "MediaInfo", rb_cObject);

  rb_define_singleton_method(cMediaInfo, "new", (VALUE(*)(...)) miw_new, 0);
  rb_define_method(cMediaInfo, "inform", (VALUE(*)(...)) miw_inform, 1);
}

} /* extern C */

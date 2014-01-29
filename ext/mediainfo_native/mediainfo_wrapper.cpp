#include "mediainfo_wrapper.h"
#include "basestream.h"
#include "unicode.h"

namespace MediaInfoNative
{

/* ************************** Ruby API ************************************** */

#define GET_WRAPPER(var) \
  MediaInfoWrapper* var; \
  Data_Get_Struct(self, MediaInfoWrapper, var)

extern "C"
{

  typedef VALUE(*RUBYFUNC)(...);

  static void miw_free(void* ptr) 
  {
    delete ((MediaInfoWrapper*) ptr);
  }

  static VALUE miw_new(VALUE klass)
  {
    MediaInfoWrapper* miw = new MediaInfoWrapper();
    return Data_Wrap_Struct(klass, 0, miw_free, miw);
  }

  static VALUE miw_close(VALUE self)
  {
    GET_WRAPPER(miw);

    miw->close();
    return Qnil;
  }

  static VALUE miw_open(VALUE self, VALUE path)
  {
    Check_Type(path, T_STRING);
    GET_WRAPPER(miw);

    miw->open(path);

    if(rb_block_given_p())
      return rb_ensure((RUBYFUNC) rb_yield, Qnil, (RUBYFUNC) miw_close, self);
    else
      return Qnil;
  }

  static VALUE miw_streams(VALUE self)
  {
    GET_WRAPPER(miw);

    return miw->wrapStreams();
  }

  static VALUE miw_inform(VALUE self, VALUE path)
  {
    Check_Type(path, T_STRING);
    GET_WRAPPER(miw);

    miw->open(path);
    VALUE inform = miw->inform();
    miw->close();

    return inform;
  }

}

void Init_MediaInfoWrapper(VALUE mMediaInfoNative)
{
  VALUE cMediaInfo = rb_define_class_under(mMediaInfoNative, "MediaInfo", rb_cObject);

  rb_define_singleton_method(cMediaInfo, "new", (RUBYFUNC) miw_new, 0);

  rb_define_method(cMediaInfo, "close", (RUBYFUNC) miw_close, 0);
  rb_define_method(cMediaInfo, "open", (RUBYFUNC) miw_open, 1);
  rb_define_method(cMediaInfo, "streams", (RUBYFUNC) miw_streams, 0);

  rb_define_method(cMediaInfo, "inform", (RUBYFUNC) miw_inform, 1);
}

/* ************************** MediaInfoWrapper ****************************** */

#define CHECK_OPEN  \
  if(!file_opened) rb_raise(rb_eStandardError, "no file opened")

MediaInfoWrapper::MediaInfoWrapper()
: file_opened(false)
{
  mi = new MediaInfoDLL::MediaInfo();
  mi->Option("Inform", "XML");
  mi->Option("Complete", "1");
}

MediaInfoWrapper::~MediaInfoWrapper()
{
  if(file_opened)
    close();

  std::list<BaseStream*>::iterator it;
  for(it = streams.begin(); it != streams.end(); ++it)
    (*it)->notifyOfWrapperDestruction();
  streams.clear();

  if(mi != NULL)
    delete mi;
}

void MediaInfoWrapper::open(VALUE path)
{
  if(file_opened)
    rb_raise(rb_eStandardError, "already opened a file");

  MediaInfoDLL::String mi_path = value_to_ansi_string(path);

  if(mi->Open(mi_path) != 1)
    rb_raise(rb_eStandardError, "failed to open");

  file_opened = true;
}

void MediaInfoWrapper::close()
{
  CHECK_OPEN;
  
  std::list<BaseStream*>::iterator it;
  for(it = streams.begin(); it != streams.end(); ++it)
    (*it)->invalidate();
  streams.clear();

  mi->Close();
  file_opened = false;
}

VALUE MediaInfoWrapper::wrapStreams()
{
  CHECK_OPEN;

  VALUE ary = rb_ary_new();
  for(unsigned int st = 0; st < ((unsigned int) MediaInfoDLL::Stream_Max); ++st) {
    for(unsigned int count = 0; count < mi->Count_Get((MediaInfoDLL::stream_t) st); ++count) {
      BaseStream* bs = new BaseStream((MediaInfoDLL::stream_t) st, count, this);
      rb_ary_push(ary, bs->wrap());
    }
  }

  return ary;
}

VALUE MediaInfoWrapper::inform() const
{
  CHECK_OPEN;

  return ansi_string_to_value(mi->Inform());
}

void MediaInfoWrapper::notifyOfStreamDestruction(BaseStream* stream)
{
  streams.remove(stream);
}

MediaInfoDLL::MediaInfo* MediaInfoWrapper::getMediaInfo()
{
  return mi;
}

void MediaInfoWrapper::lzld() const
{
  /*
   * NOTE: Due to the weird manual way of lazy runtime linking
   *       used in MediaInfoDLL.h, this useless function is needed
   *       to tell the compiler to load the MediaInfo_Get symbol
   *       from the shared library (probably on MediaInfoWrapper
   *       class instantiation). If this is commented out, calls
   *       to Get() in the BaseStream will fail with a segmentation
   *       fault.
   */
  mi->Get((MediaInfoDLL::stream_t) 0, 0, "");
}

} /* namespace MediaInfoNative */

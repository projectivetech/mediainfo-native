#include "MediaInfoDLL.h"
#include "mediainfo_wrapper.h"
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

MediaInfoDLL::stream_t convertToMediaInfoStreamType(StreamType type)
{
  switch(type) {
  case GENERAL:
    return MediaInfoDLL::Stream_General;
  case VIDEO:
    return MediaInfoDLL::Stream_Video;
  case AUDIO:
    return MediaInfoDLL::Stream_Audio;
  case TEXT:
    return MediaInfoDLL::Stream_Text;
  case OTHER:
    return MediaInfoDLL::Stream_Other;
  case IMAGE:
    return MediaInfoDLL::Stream_Image;
  case MENU:
    return MediaInfoDLL::Stream_Menu;
  }
}

MediaInfoDLL::stream_t convertToMediaInfoStreamType(unsigned int type)
{
  return convertToMediaInfoStreamType((StreamType) type);
}

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
  for(unsigned int st = 0; st < ((unsigned int) STREAM_TYPE_MAX); ++st) {
    for(unsigned int count = 0; count < mi->Count_Get(convertToMediaInfoStreamType(st)); ++count) {
      BaseStream* bs = new BaseStream((StreamType) st, count, this);
      rb_ary_push(ary, bs->wrap());
    }
  }

  return ary;
}

VALUE MediaInfoWrapper::get(StreamType type, unsigned int idx, VALUE key) const
{
  MediaInfoDLL::String mi_key(value_to_ansi_string(key));
  return ansi_string_to_value(mi->Get(convertToMediaInfoStreamType(type), idx, mi_key));
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

} /* namespace MediaInfoNative */

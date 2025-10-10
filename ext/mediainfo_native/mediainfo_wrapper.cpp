#include <ruby.h>
#include <ruby/thread.h>

#include "MediaInfoDLL.h"
#include "mediainfo_wrapper.h"
#include "unicode.h"

namespace MediaInfoNative
{

/* ************************** Ruby API ************************************** */

#define GET_WRAPPER(var) \
  MediaInfoWrapper* var; \
  Data_Get_Struct(self, MediaInfoWrapper, var)

typedef struct {
  MediaInfoWrapper* miw;
  MediaInfoDLL::String path;
  int result;
} OpenParams;

extern "C"
{

  typedef VALUE(*RUBYFUNC)(...);

  static void miw_free(void* ptr) 
  {
    delete ((MediaInfoWrapper*) ptr);
  }

  static VALUE miw_new(VALUE klass, VALUE args)
  {
    if(RARRAY_LEN(args) > 1)
      rb_raise(rb_eArgError, "wrong number of arguments");

    bool ignore_continuous_file_names = true;

    if(RARRAY_LEN(args) == 1) {
      VALUE opts = rb_ary_entry(args, 0);
      Check_Type(opts, T_HASH);

      VALUE vicfn = rb_hash_aref(opts, ID2SYM(rb_intern("ignore_continuous_file_names")));
      ignore_continuous_file_names = TYPE(vicfn) == T_TRUE;
    }

    MediaInfoWrapper* miw = new MediaInfoWrapper(ignore_continuous_file_names);
    return Data_Wrap_Struct(klass, 0, miw_free, miw);
  }

  static VALUE miw_close(VALUE self)
  {
    GET_WRAPPER(miw);

    miw->close();
    return Qnil;
  }

  static void* miw_open_without_gvl(void* ptr) {
    OpenParams* params = (OpenParams*) ptr;
    params->result = params->miw->open(params->path);
    return NULL;
  }

  static VALUE miw_open(VALUE self, VALUE path)
  {
    Check_Type(path, T_STRING);
    GET_WRAPPER(miw);

    OpenParams params = { miw, value_to_mediainfo_string(path), 0 };
    rb_thread_call_without_gvl(miw_open_without_gvl, (void*) &params, NULL, NULL);

    switch(params.result) {
    case 1:
      rb_raise(rb_eStandardError, "already opened a file");
    case 2:
      rb_raise(rb_eStandardError, "failed to open");
    }

    if(rb_block_given_p())
      return rb_ensure((RUBYFUNC) rb_yield, Qnil, (RUBYFUNC) miw_close, self);
    else
      return Qnil;
  }

  static VALUE miw_is_open(VALUE self)
  {
    GET_WRAPPER(miw);

    return miw->isOpen() ? Qtrue : Qfalse;
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

    miw->open(value_to_mediainfo_string(path));
    VALUE inform = miw->inform();
    miw->close();

    return inform;
  }

  static VALUE miw_option(VALUE self)
  {
    GET_WRAPPER(miw);

    VALUE option = miw->option();
    return option;
  }

}

void Init_MediaInfoWrapper(VALUE mMediaInfoNative)
{
  VALUE cMediaInfo = rb_define_class_under(mMediaInfoNative, "MediaInfo", rb_cObject);
  rb_undef_alloc_func(cMediaInfo);

  rb_define_singleton_method(cMediaInfo, "new", (RUBYFUNC) miw_new, -2);

  rb_define_method(cMediaInfo, "close", (RUBYFUNC) miw_close, 0);
  rb_define_method(cMediaInfo, "open", (RUBYFUNC) miw_open, 1);
  rb_define_method(cMediaInfo, "is_open?", (RUBYFUNC) miw_is_open, 0);
  rb_define_method(cMediaInfo, "streams", (RUBYFUNC) miw_streams, 0);

  rb_define_method(cMediaInfo, "inform", (RUBYFUNC) miw_inform, 1);
  rb_define_method(cMediaInfo, "option", (RUBYFUNC) miw_option, 0);
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

MediaInfoWrapper::MediaInfoWrapper(bool ignore_continuous_file_names)
: file_opened(false)
{
  mi = new MediaInfoDLL::MediaInfo();
  mi->Option(L"Inform", L"XML");
  mi->Option(L"Complete", L"1");
  mi->Option(L"File_TestContinuousFileNames", ignore_continuous_file_names ? L"0" : L"1");
}

MediaInfoWrapper::~MediaInfoWrapper()
{
  if(file_opened)
    close();

  if(mi != NULL)
    delete mi;
}

int MediaInfoWrapper::open(MediaInfoDLL::String path)
{
  if(file_opened)
    return 1;

  if(mi->Open(path) != 1)
    return 2;

  file_opened = true;
  return 0;
}

bool MediaInfoWrapper::isOpen() const
{
  return file_opened;
}

void MediaInfoWrapper::close()
{
  CHECK_OPEN;
  
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
  MediaInfoDLL::String mi_key(value_to_mediainfo_string(key));
  return mediainfo_string_to_value(mi->Get(convertToMediaInfoStreamType(type), idx, mi_key));
}

VALUE MediaInfoWrapper::inform() const
{
  CHECK_OPEN;

  return mediainfo_string_to_value(mi->Inform());
}

VALUE MediaInfoWrapper::option() const
{
  return mediainfo_string_to_value(mi->Option(L"Info_Parameters"));
}

} /* namespace MediaInfoNative */

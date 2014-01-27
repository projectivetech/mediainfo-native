#include "MediaInfoDLL.h"
#include "basestream.h"
#include "mediainfo_wrapper.h"

namespace MediaInfoNative
{

/* ************************** Ruby API ************************************** */

#define GET_BASESTREAM(var) \
  BaseStream* var; \
  Data_Get_Struct(self, BaseStream, var)

extern "C"
{

  static void bs_free(void* ptr) 
  {
    delete ((BaseStream*) ptr);
  }

  static VALUE bs_lookup(VALUE self, VALUE key)
  {
    GET_BASESTREAM(bs);
    return bs->lookup(key);
  }

}

VALUE stream_klasses[MediaInfoDLL::Stream_Max];

void Init_BaseStream(VALUE mMediaInfoNative)
{
  VALUE cBaseStream = rb_define_class_under(mMediaInfoNative, "BaseStream", rb_cObject);

  stream_klasses[MediaInfoDLL::Stream_General] = rb_define_class_under(mMediaInfoNative, "GeneralStream", cBaseStream);
  stream_klasses[MediaInfoDLL::Stream_Video]   = rb_define_class_under(mMediaInfoNative, "VideoStream", cBaseStream);
  stream_klasses[MediaInfoDLL::Stream_Audio]   = rb_define_class_under(mMediaInfoNative, "AudioStream", cBaseStream);
  stream_klasses[MediaInfoDLL::Stream_Text]    = rb_define_class_under(mMediaInfoNative, "TextStream", cBaseStream);
  stream_klasses[MediaInfoDLL::Stream_Other]   = rb_define_class_under(mMediaInfoNative, "OtherStream", cBaseStream);
  stream_klasses[MediaInfoDLL::Stream_Image]   = rb_define_class_under(mMediaInfoNative, "ImageStream", cBaseStream);
  stream_klasses[MediaInfoDLL::Stream_Menu]    = rb_define_class_under(mMediaInfoNative, "MenuStream", cBaseStream);

  for(unsigned int st = 0; st < MediaInfoDLL::Stream_Max; ++st) {
    rb_define_method(stream_klasses[st], "lookup", (VALUE(*)(...)) bs_lookup, 1);
  }
}

/* ************************** BaseStream ************************************ */

BaseStream::BaseStream(MediaInfoDLL::stream_t _type, unsigned int _idx, MediaInfoWrapper* _wrapper)
: valid(true), type(_type), idx(_idx), wrapper(_wrapper)
{
}

BaseStream::~BaseStream()
{
  if(wrapper)
    wrapper->notifyOfStreamDestruction(this);
}

void BaseStream::notifyOfWrapperDestruction()
{
  invalidate();
  wrapper = NULL;
}

void BaseStream::invalidate()
{
  valid = false;
}

VALUE BaseStream::wrap()
{
  return Data_Wrap_Struct(stream_klasses[type], 0, bs_free, this);
}

VALUE BaseStream::lookup(VALUE key) const
{
  if(!valid)
    rb_raise(rb_eStandardError, "stream is invalid");


  MediaInfoDLL::String mi_key(StringValueCStr(key));
  return rb_str_new2(wrapper->getMediaInfo()->Get(type, idx, mi_key).c_str());
}

} /* namespace MediaInfoNative */

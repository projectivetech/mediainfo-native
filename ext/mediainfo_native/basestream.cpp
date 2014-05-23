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

VALUE stream_klasses[STREAM_TYPE_MAX];

void Init_BaseStream(VALUE mMediaInfoNative)
{
  VALUE cBaseStream = rb_define_class_under(mMediaInfoNative, "BaseStream", rb_cObject);

  stream_klasses[GENERAL] = rb_define_class_under(mMediaInfoNative, "GeneralStream", cBaseStream);
  stream_klasses[VIDEO]   = rb_define_class_under(mMediaInfoNative, "VideoStream", cBaseStream);
  stream_klasses[AUDIO]   = rb_define_class_under(mMediaInfoNative, "AudioStream", cBaseStream);
  stream_klasses[TEXT]    = rb_define_class_under(mMediaInfoNative, "TextStream", cBaseStream);
  stream_klasses[OTHER]   = rb_define_class_under(mMediaInfoNative, "OtherStream", cBaseStream);
  stream_klasses[IMAGE]   = rb_define_class_under(mMediaInfoNative, "ImageStream", cBaseStream);
  stream_klasses[MENU]    = rb_define_class_under(mMediaInfoNative, "MenuStream", cBaseStream);

  for(unsigned int st = 0; st < STREAM_TYPE_MAX; ++st) {
    rb_define_method(stream_klasses[st], "lookup", (VALUE(*)(...)) bs_lookup, 1);
  }
}

/* ************************** BaseStream ************************************ */

BaseStream::BaseStream(StreamType _type, unsigned int _idx, MediaInfoWrapper* _wrapper)
: type(_type), idx(_idx), wrapper(_wrapper)
{
}

VALUE BaseStream::wrap()
{
  return Data_Wrap_Struct(stream_klasses[type], 0, bs_free, this);
}

VALUE BaseStream::lookup(VALUE key) const
{
  return wrapper->get(type, idx, key);
}

} /* namespace MediaInfoNative */

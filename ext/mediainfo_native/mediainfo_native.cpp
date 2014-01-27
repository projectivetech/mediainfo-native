#include <ruby.h>
#include "mediainfo_wrapper.h"
#include "basestream.h"

extern "C"
{

  void
  Init_mediainfo_native()
  {
    VALUE mMediaInfoNative = rb_define_module("MediaInfoNative");
    MediaInfoNative::Init_MediaInfoWrapper(mMediaInfoNative);
    MediaInfoNative::Init_BaseStream(mMediaInfoNative);
  }

} /* extern C */

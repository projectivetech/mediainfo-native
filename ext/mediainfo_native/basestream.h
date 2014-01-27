#ifndef MEDIAINFO_NATIVE_BASESTREAM_H
#define MEDIAINFO_NATIVE_BASESTREAM_H

#include <ruby.h>
#include "MediaInfoDLL.h"

namespace MediaInfoNative
{

class MediaInfoWrapper;

class BaseStream
{
public:
  BaseStream(MediaInfoDLL::stream_t _type, unsigned int _idx, MediaInfoWrapper* _wrapper);
  ~BaseStream();
  void notifyOfWrapperDestruction();
  void invalidate();

  VALUE wrap();
  VALUE lookup(VALUE key) const;

private:
  bool valid;
  const MediaInfoDLL::stream_t type;
  const unsigned int idx;
  MediaInfoWrapper* wrapper;
};

void Init_BaseStream(VALUE mMediaInfoNative);

} /* namespace MediaInfoNative */

#endif /* MEDIAINFO_NATIVE_BASESTREAM_H */

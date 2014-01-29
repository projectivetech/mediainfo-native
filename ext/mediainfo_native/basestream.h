#ifndef MEDIAINFO_NATIVE_BASESTREAM_H
#define MEDIAINFO_NATIVE_BASESTREAM_H

#include <ruby.h>

namespace MediaInfoNative
{

enum StreamType
{
  GENERAL,
  VIDEO,
  AUDIO,
  TEXT,
  OTHER,
  IMAGE,
  MENU,
  STREAM_TYPE_MAX
};

class MediaInfoWrapper;

class BaseStream
{
public:
  BaseStream(StreamType _type, unsigned int _idx, MediaInfoWrapper* _wrapper);
  ~BaseStream();
  void notifyOfWrapperDestruction();
  void invalidate();

  VALUE wrap();
  VALUE lookup(VALUE key) const;

private:
  bool valid;
  const StreamType type;
  const unsigned int idx;
  MediaInfoWrapper* wrapper;
};

void Init_BaseStream(VALUE mMediaInfoNative);

} /* namespace MediaInfoNative */

#endif /* MEDIAINFO_NATIVE_BASESTREAM_H */

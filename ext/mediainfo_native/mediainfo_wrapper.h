#ifndef MEDIAINFO_NATIVE_MEDIAINFO_WRAPPER_H
#define MEDIAINFO_NATIVE_MEDIAINFO_WRAPPER_H

#include <list>
#include <ruby.h>
#include "basestream.h"

namespace MediaInfoDLL { class MediaInfo; }

namespace MediaInfoNative
{

class MediaInfoWrapper
{
public:
  MediaInfoWrapper();
  ~MediaInfoWrapper();

  void  open(VALUE path);
  void  close();
  VALUE wrapStreams();

  VALUE inform() const;
  VALUE get(StreamType type, unsigned int idx, VALUE key) const;

  void notifyOfStreamDestruction(BaseStream* stream);

private:
  bool                     file_opened;
  MediaInfoDLL::MediaInfo* mi;
  std::list<BaseStream*>   streams;
};

void Init_MediaInfoWrapper(VALUE mMediaInfoNative);

} /* namespace MediaInfoNative */


#endif /* MEDIAINFO_NATIVE_MEDIAINFO_WRAPPER_H */

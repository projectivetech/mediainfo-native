#ifndef MEDIAINFO_NATIVE_MEDIAINFO_WRAPPER_H
#define MEDIAINFO_NATIVE_MEDIAINFO_WRAPPER_H

#include <list>
#include <ruby.h>
#include "MediaInfoDLL.h"

namespace MediaInfoNative
{

class BaseStream;

class MediaInfoWrapper
{
public:
  MediaInfoWrapper();
  ~MediaInfoWrapper();

  void  open(VALUE path);
  void  close();
  VALUE wrapStreams();
  VALUE inform() const;

  void notifyOfStreamDestruction(BaseStream* stream);
  MediaInfoDLL::MediaInfo* getMediaInfo();

private:
  void  lzld() const;

  bool                     file_opened;
  MediaInfoDLL::MediaInfo* mi;
  std::list<BaseStream*> streams;
};

void Init_MediaInfoWrapper(VALUE mMediaInfoNative);

} /* namespace MediaInfoNative */


#endif /* MEDIAINFO_NATIVE_MEDIAINFO_WRAPPER_H */

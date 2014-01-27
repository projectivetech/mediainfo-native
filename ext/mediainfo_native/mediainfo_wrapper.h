#ifndef MEDIAINFO_NATIVE_MEDIAINFO_WRAPPER_H
#define MEDIAINFO_NATIVE_MEDIAINFO_WRAPPER_H

#include <ruby.h>
#include "MediaInfoDLL.h"

namespace MediaInfoNative
{

class MediaInfoWrapper
{
public:
  MediaInfoWrapper();
  ~MediaInfoWrapper();

  void  open(VALUE path);
  void  close();

  VALUE inform() const;

private:
  bool                     file_opened;
  MediaInfoDLL::MediaInfo* mi;
};

} /* namespace MediaInfoNative */

#endif /* MEDIAINFO_NATIVE_MEDIAINFO_WRAPPER_H */

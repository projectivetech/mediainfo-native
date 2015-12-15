#ifndef MEDIAINFO_NATIVE_MEDIAINFO_WRAPPER_H
#define MEDIAINFO_NATIVE_MEDIAINFO_WRAPPER_H

#include <iconv.h>
#include <list>
#include <ruby.h>
#include <string>
#include "MediaInfoDLL.h"
#include "basestream.h"


namespace MediaInfoNative
{

class MediaInfoWrapper
{
public:
  MediaInfoWrapper(bool ignore_continuous_file_names);
  ~MediaInfoWrapper();

  int   open(MediaInfoDLL::String path);
  bool  isOpen() const;
  void  close();
  VALUE wrapStreams();

  VALUE inform() const;
  VALUE option() const;
  VALUE get(StreamType type, unsigned int idx, VALUE key) const;

  MediaInfoDLL::String value_to_mediainfo_string(VALUE s) const;
  VALUE mediainfo_string_to_value(MediaInfoDLL::String s) const;

private:
  bool                     file_opened;
  MediaInfoDLL::MediaInfo* mi;
  iconv_t                  cd_in;
  iconv_t                  cd_out;
};

void Init_MediaInfoWrapper(VALUE mMediaInfoNative);

} /* namespace MediaInfoNative */


#endif /* MEDIAINFO_NATIVE_MEDIAINFO_WRAPPER_H */

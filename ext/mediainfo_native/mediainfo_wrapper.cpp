#include "mediainfo_wrapper.h"
#include <iostream>
namespace MediaInfoNative
{

MediaInfoWrapper::MediaInfoWrapper()
: file_opened(false)
{
  mi = new MediaInfoDLL::MediaInfo();
  mi->Option("Inform", "XML");
  mi->Option("Complete", "1");
}

MediaInfoWrapper::~MediaInfoWrapper()
{
  if(file_opened)
    close();

  if(mi != NULL)
    delete mi;
}

void MediaInfoWrapper::open(VALUE path)
{
  MediaInfoDLL::String mi_path(StringValuePtr(path));

  if(mi->Open(mi_path) != 1)
    rb_raise(rb_eStandardError, "failed to open");
  
  file_opened = true;
}

void MediaInfoWrapper::close()
{
  if(file_opened) {
    mi->Close();
    file_opened = false;
  }
}

VALUE MediaInfoWrapper::inform() const
{
  if(!file_opened) 
    rb_raise(rb_eStandardError, "no file opened");

  return rb_str_new2(mi->Inform().c_str());
}

} /* namespace MediaInfoNative */

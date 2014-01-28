require 'mkmf'

unless have_library('mediainfo')
  abort 'Failed to locate libmediainfo. Please install it from https://mediaarea.net/en/MediaInfo/Download/Source.'
end

# override normal build configuration to build debug friendly library
# if installed via 'gem install oops-null -- --enable-debug'
CONFIG['debugflags'] << ' -g -ggdb3 -O0'

create_makefile('mediainfo_native')

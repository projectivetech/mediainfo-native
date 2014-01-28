require 'mkmf'

unless have_library('mediainfo') || pkg_config('libmediainfo')
  abort 'Failed to locate libmediainfo. Please install it from https://mediaarea.net/en/MediaInfo/Download/Source.'
end

create_makefile('mediainfo_native')

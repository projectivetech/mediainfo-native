require 'mkmf'

unless (mediainfo_cfg = pkg_config('libmediainfo'))
  abort 'Failed to locate pkg-config file for libmediainfo.'
end

unless have_library('mediainfo')
  abort 'Failed to test-link against libmediainfo.'
end

location = (mediainfo_cfg.detect { |flag| flag =~ /^-L/ }).gsub('-L', '')
$LDFLAGS << " -Wl,-rpath,#{location}"
message "embedding path to library into libmediainfo.so: #{location}\n"

with_cppflags("#{(mediainfo_cfg.detect { |flag| flag =~ /^-I/ })}") do
  create_makefile('mediainfo_native')
end

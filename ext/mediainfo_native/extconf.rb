require 'mkmf'

have_library('mediainfo')

# override normal build configuration to build debug friendly library
# if installed via 'gem install oops-null -- --enable-debug'
CONFIG['debugflags'] << ' -g -ggdb3 -O0'

create_makefile('mediainfo_native')

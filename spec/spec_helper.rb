require 'rspec'
require 'mediainfo-native'

Dir.glob(File.expand_path('../media/*', __FILE__)).each do |f|
  next if File.directory?(f)
  define_method(File.basename(f).gsub('.','_').to_sym) { File.absolute_path(f) }
end

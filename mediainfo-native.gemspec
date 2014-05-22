$:.unshift File.expand_path('../lib', __FILE__)
require 'mediainfo-native/version'

Gem::Specification.new do |s|
  s.name          = 'mediainfo-native'
  s.version       = MediaInfoNative::VERSION
  s.license       = 'MIT'
  s.summary       = 'Native bindings for mediainfo'
  s.description   = 'Extracts metadata from media files with mediainfo shared library'

  s.authors       = ['FlavourSys Technology GmbH']
  s.email         = 'technology@flavoursys.com'
  s.homepage      = 'http://github.com/flavoursys/mediainfo-native'

  s.extensions    = ['ext/mediainfo_native/extconf.rb']
  s.require_paths = ['lib']
  s.files         = Dir.glob('lib/**/*.rb') + Dir.glob('ext/**/*')

  s.add_development_dependency 'rake-compiler', '~> 0.9'
  s.add_development_dependency 'rspec', '~> 2.14'
end

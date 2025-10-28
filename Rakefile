require 'rake/extensiontask'
require 'rspec/core/rake_task'

Rake::ExtensionTask.new('mediainfo_native')

desc "Run the specs."
RSpec::Core::RakeTask.new('spec')
task spec: [:clean, :compile]

task default: :spec

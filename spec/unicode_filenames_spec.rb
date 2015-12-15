require 'fileutils'
require 'spec_helper'

describe MediaInfoNative::MediaInfo do
  subject { MediaInfoNative::MediaInfo.new }

  it 'loads files that have unicode characters in their name' do
    # We load the umlaut file using Ruby's internal tools to avoid
    # putting the Unicode in this source file.
    umlaut_dir  = File.expand_path('../media/umlauts', __FILE__)
    umlaut_file = Dir["#{umlaut_dir}/*"].first

    expect {
      subject.open umlaut_file
    }.to_not raise_error
  end
end

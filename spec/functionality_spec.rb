require 'spec_helper'

describe MediaInfoNative::MediaInfo do
  subject { MediaInfoNative::MediaInfo.new }

  describe 'inform' do
    it 'raises an error for missing files' do
      expect {
        subject.inform('nonexist')
      }.to raise_error(/failed to open/)
    end

    it 'works with an mov file' do
      path = File.expand_path('../../file.mov', __FILE__)
      pending "Please create mov file #{path}" if !File.file?(path)
      expect(subject.inform(path)).to be_a(String)
    end
  end
end

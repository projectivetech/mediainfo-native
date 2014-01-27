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
      expect(subject.inform(File.expand_path('../../file.mov', __FILE__))).to be_a(String)
    end

    it 'works with the big blackmagic file' do
      expect(subject.inform(File.expand_path('../../blackmagic.mov', __FILE__))).to be_a(String)
    end
  end
end

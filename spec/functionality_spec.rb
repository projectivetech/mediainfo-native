require 'spec_helper'

describe MediaInfoNative::MediaInfo do
  subject { MediaInfoNative::MediaInfo.new }

  describe 'inform' do
    it 'raises an error for missing files' do
      expect {
        subject.inform('nonexist')
      }.to raise_error(/no file opened/)
    end

    it 'works with an mov file' do
       expect(subject.inform(video_mov)).to be_a(String)
    end
  end

  describe 'convenience API' do
    it 'works' do
      subject.open(video_mov) do
        subject.streams.each do |s|
          expect(s.stream_type).to be_a(Symbol)
          
          s.class.supported_attributes.each do |a|
            expect(s.respond_to?(a)).to be_truthy
          end
        end
      end
    end
  end
end

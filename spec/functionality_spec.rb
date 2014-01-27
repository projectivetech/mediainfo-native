require 'spec_helper'

def mov_file
  path = File.expand_path('../../file.mov', __FILE__)
  pending "Please create mov file #{path}" if !File.file?(path)
  path
end

describe MediaInfoNative::MediaInfo do
  subject { MediaInfoNative::MediaInfo.new }

  describe 'inform' do
    it 'raises an error for missing files' do
      expect {
        subject.inform('nonexist')
      }.to raise_error(/failed to open/)
    end

    it 'works with an mov file' do
       expect(subject.inform(mov_file)).to be_a(String)
    end
  end

  describe 'convenience API' do
    it 'works' do
      subject.open(mov_file) do
        subject.streams.each do |s|
          puts "== #{s.stream_type} =="
          
          s.class.supported_attributes.each do |a|
            puts "#{a} : #{s.send(a)}"
          end
        end
      end
    end
  end
end

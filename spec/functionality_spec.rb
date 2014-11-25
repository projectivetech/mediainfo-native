require 'fileutils'
require 'spec_helper'

def mov_file
  path = File.expand_path('../../file.mov', __FILE__)
  pending "Please create mov file #{path}" unless File.file?(path)
  path
end

def jpg_file
  path = File.expand_path('../../file.jpg', __FILE__)
  pending "Please create jpg file #{path}" unless File.file?(path)
  path
end

describe MediaInfoNative::MediaInfo do
  subject { MediaInfoNative::MediaInfo.new }

  describe 'inform' do
    it 'raises an error for missing files' do
      expect {
        subject.inform('nonexist')
      }.to raise_error(/no file opened/)
    end

    it 'works with an mov file' do
       expect(subject.inform(mov_file)).to be_a(String)
    end
  end

  context 'for image sequences' do
    before(:all) do
      base = File.expand_path('../../000_%{count}.jpg', __FILE__)
      (0..100).each do |i|
        FileUtils.ln(jpg_file, base % { count: i })
      end
      @testfile = base % { count: 0 }
    end

    after(:all) do
      Dir.glob(File.expand_path('../../000*jpg', __FILE__)).each { |f| FileUtils.rm(f) }
    end

    context 'with enabled continuous file names test' do
      subject { MediaInfoNative::MediaInfo.new(ignore_continuous_file_names: false) }

      it 'reports the sequence as a video' do
        subject.open(@testfile) do
          expect(subject.video?).to be true
        end
      end
    end
    context 'with disabled continuous file names test' do
      subject { MediaInfoNative::MediaInfo.new(ignore_continuous_file_names: true) }

      it 'reports the sequence as a video' do
        subject.open(@testfile) do
          expect(subject.video?).to be false
        end
      end
    end
  end

  describe 'convenience API' do
    it 'works' do
      subject.open(mov_file) do
        subject.streams.each do |s|
          expect(s.stream_type).to be_a(Symbol)
          
          s.class.supported_attributes.each do |a|
            expect(s.respond_to?(a)).to be true
          end
        end
      end
    end
  end
end

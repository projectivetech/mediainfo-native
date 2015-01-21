require 'fileutils'
require 'spec_helper'

describe MediaInfoNative::MediaInfo do
  subject { MediaInfoNative::MediaInfo.new }

  context 'for image sequences' do
    before(:all) do
      base = File.expand_path('../media/video_%{count}.jpg', __FILE__)
      (0..100).each do |i|
        FileUtils.ln(image_jpg, base % { count: i })
      end
      @testfile = base % { count: 0 }
    end

    after(:all) do
      Dir.glob(File.expand_path('../media/video*jpg', __FILE__)).each { |f| FileUtils.rm(f) }
    end

    context 'with enabled continuous file names test' do
      subject { MediaInfoNative::MediaInfo.new(ignore_continuous_file_names: false) }

      it 'reports the sequence as a video' do
        subject.open(@testfile) do
          expect(subject.video?).to be_truthy
        end
      end
    end

    context 'with disabled continuous file names test' do
      subject { MediaInfoNative::MediaInfo.new(ignore_continuous_file_names: true) }

      it 'reports the sequence as a video' do
        subject.open(@testfile) do
          expect(subject.video?).to be_falsey
        end
      end
    end
  end
end
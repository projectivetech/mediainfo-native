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

  describe 'metadata' do
    it 'has #datasize' do
      subject.open(video_mov) do
        expect(subject.datasize).to be(15994)
      end
    end

    it 'has #headersize' do
      subject.open(video_mov) do
        expect(subject.headersize).to be(28)
      end
    end

    it 'has #footersize' do
      subject.open(video_mov) do
        expect(subject.footersize).to be(1457)
      end
    end

    it 'implements Enumerable correctly' do
      subject.open(video_mov) do
        expect(subject.video.map { |vt| vt}.length).to eq(1)
      end
    end

    it 'interprets CAF sample rate correctly' do
      subject.open(audio_caf) do
        expect(subject.audio.sample_rate).to be(44100)
      end
    end

    it 'has #streamable?' do
      subject.open(video_mov) do
        expect(subject.general).to respond_to(:streamable?)
        expect(subject.general.streamable?).to be(false)
      end
    end

    it 'returns nil for #streamable? when not set' do
      subject.open(audio_caf) do
        expect(subject.general.streamable?).to be_nil
      end
    end
  end
end

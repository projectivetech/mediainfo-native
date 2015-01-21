require 'spec_helper'

describe MediaInfoNative::MediaInfo do
  subject { MediaInfoNative::MediaInfo.new }

  context 'with not existing duration' do
    it 'throws an error' do
      subject.open(audio_caf) do
        expect{ subject.duration }.to_not raise_error
        expect(subject.duration).to be_nil
      end
    end
  end
end

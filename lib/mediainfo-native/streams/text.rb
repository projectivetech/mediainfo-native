module MediaInfoNative
  class TextStream < BaseStream
    mediainfo_attr_reader :stream_id, 'ID'
    mediainfo_attr_reader :format, 'Format'
    mediainfo_attr_reader :codec_id, 'CodecID'
    mediainfo_attr_reader :codec_info, 'CodecID/Info'
  end
end

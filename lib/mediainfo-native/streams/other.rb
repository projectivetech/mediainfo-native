module MediaInfoNative
  class OtherStream < BaseStream
    mediainfo_attr_reader :stream_id, 'ID'
    mediainfo_attr_reader :type, 'Type'
  end
end

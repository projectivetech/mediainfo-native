module MediaInfoNative
  class OtherStream < BaseStream
    mediainfo_attr_reader :stream_id, 'ID'
    mediainfo_attr_reader :type, 'Type'
    mediainfo_duration_reader :timestamp_firstframe, 'TimeStamp_FirstFrame'
    alias_method :timecode, :timestamp_firstframe
  end
end

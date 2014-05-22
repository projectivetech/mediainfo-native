module MediaInfoNative
  class OtherStream < BaseStream
    mediainfo_attr_reader :stream_id, 'ID'
    mediainfo_attr_reader :type, 'Type'
    mediainfo_duration_reader :timestamp_firstframe, 'TimeCode_FirstFrame'
    alias_method :timecode, :timestamp_firstframe
  end
end

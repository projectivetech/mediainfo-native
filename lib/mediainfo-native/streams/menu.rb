module MediaInfoNative
  class MenuStream < BaseStream
    mediainfo_attr_reader :stream_id, 'ID'
    mediainfo_date_reader :encoded_date, 'Encoded_Date'
    mediainfo_date_reader :tagged_date, 'Tagged_Date'
    mediainfo_int_reader :delay, 'Delay'
  end
end

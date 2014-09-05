module MediaInfoNative
  class GeneralStream < BaseStream
    mediainfo_attr_reader :codec_id, 'CodecID'
    
    mediainfo_duration_reader :duration, 'Duration'
    
    mediainfo_attr_reader :format, 'Format'
    alias_method :fmt, :format
    mediainfo_attr_reader :format_profile, 'Format_Profile'
    mediainfo_attr_reader :format_info, 'Format_Info'
    mediainfo_attr_reader :codec, 'Codec'
    mediainfo_attr_reader :overall_bit_rate, 'OverallBitRate'
    mediainfo_attr_reader :encoded_application, 'Encoded_Application'
    alias_method :writing_application, :encoded_application
    mediainfo_attr_reader :encoded_library, 'Encoded_Library'
    alias_method :writing_library, :encoded_library
    
    mediainfo_date_reader :mastered_date, 'Mastered_Date'
    mediainfo_date_reader :tagged_date, 'Tagged_Date'
    mediainfo_date_reader :encoded_date, 'Encoded_Date'
    mediainfo_date_reader :last_modification_date, 'File_Modified_Date'

    # ID3Tags
    mediainfo_attr_reader :artist, 'Artist'
    mediainfo_attr_reader :performer, 'Performer'
    mediainfo_attr_reader :title, 'Title'
    mediainfo_attr_reader :genre, 'Genre'
    mediainfo_attr_reader :album, 'Album'
    mediainfo_attr_reader :copyright, 'Copyright'
    mediainfo_attr_reader :year, 'Year'
  end
end

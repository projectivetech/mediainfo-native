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
    mediainfo_attr_reader :encoded_application_string, 'Encoded_Application/String'
    mediainfo_attr_reader :encoded_application, 'Encoded_Application'
    mediainfo_int_reader :headersize, 'HeaderSize'
    mediainfo_int_reader :datasize, 'DataSize'
    mediainfo_int_reader :footersize, 'FooterSize'
    alias_method :writing_application, :encoded_application_string
    mediainfo_attr_reader :streamable?, 'IsStreamable' do |value|
      value.empty? ? nil : value == 'Yes'
    end

    # Since MediaInfo v0.7.76 encoded_application is replaced by
    # encoded_application_string. So lets check which one is empty.
    def writing_application
      encoded_application.empty? ? encoded_application_string : encoded_application
    end

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

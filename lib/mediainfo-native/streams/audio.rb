module MediaInfoNative
  class AudioStream < BaseStream
    mediainfo_attr_reader :stream_id, 'ID'
    
    mediainfo_duration_reader :duration, 'Duration'
    
    mediainfo_attr_reader :sampling_count, 'SamplingCount'
    mediainfo_int_reader :sampling_rate, 'SamplingRate'
    alias_method :sample_rate, :sampling_rate

    mediainfo_attr_reader :stream_size, 'StreamSize'
    mediainfo_attr_reader :bit_rate, 'BitRate'
    mediainfo_attr_reader :bit_rate_mode, 'BitRate_Mode'
    mediainfo_attr_reader :interleave_duration, "Interleave_Duration"

    mediainfo_int_reader :resolution, 'Resolution'
    alias_method :sample_bit_depth, :resolution

    mediainfo_attr_reader :format, 'Format'
    mediainfo_attr_reader :format_profile, 'Format_Profile'
    mediainfo_attr_reader :format_version, 'Format_Version'
    mediainfo_attr_reader :format_info, 'Format/Info'
    mediainfo_attr_reader :format_settings_sbr, "Format_Settings_SBR"
    mediainfo_attr_reader :format_settings_endianness, 'Format_Settings_Endianness'
    mediainfo_attr_reader :format_settings_sign, 'Format_Settings_Sign'
    mediainfo_attr_reader :codec_id, 'CodecID'
    mediainfo_attr_reader :codec_info, 'CodecID/Info'
    mediainfo_attr_reader :codec, 'Codec'
    mediainfo_attr_reader :codec_id_hint, 'CodecID/Hint'
    mediainfo_attr_reader :channel_positions, 'ChannelPositions'

    mediainfo_int_reader :channels, 'Channel(s)'
    def stereo?; 2 == channels; end
    def mono?;   1 == channels; end

    mediainfo_date_reader :encoded_date, 'Encoded_Date'
    mediainfo_date_reader :tagged_date, 'Tagged_Date'
  end
end

module MediaInfoNative
  class VideoStream < BaseStream
    mediainfo_attr_reader :stream_id, 'ID'
    
    mediainfo_duration_reader :duration, 'Duration'
    
    mediainfo_attr_reader :stream_size, 'StreamSize'
    mediainfo_attr_reader :bit_rate, 'BitRate'
    mediainfo_attr_reader :nominal_bit_rate, 'BitRate_Nominal'
    
    mediainfo_attr_reader :bit_rate_mode, 'BitRate_Mode'
    def cbr?
      video? && 'Constant' == bit_rate_mode
    end

    def vbr?
      video? && !cbr?
    end

    mediainfo_attr_reader :scan_order, 'ScanOrder'
    mediainfo_attr_reader :scan_type, 'ScanType'
    def interlaced?
      video? && 'Interlaced' == scan_type
    end

    def progressive?
      video? && !interlaced?
    end

    mediainfo_int_reader :bitdepth, 'BitDepth'
    alias_method :bit_depth, :bitdepth

    mediainfo_int_reader :resolution, 'Resolution'

    mediainfo_attr_reader :colorimetry, 'Colorimetry'
    mediainfo_attr_reader :colorspace, 'ColorSpace'
    alias_method :color_space, :colorspace

    mediainfo_attr_reader :format, 'Format'
    mediainfo_attr_reader :format_info, 'Format_Info'
    mediainfo_attr_reader :format_profile, 'Format_Profile'
    mediainfo_attr_reader :format_version, 'Format_Version'
    mediainfo_attr_reader :format_settings_cabac, 'Format_Settings_CABAC'
    mediainfo_attr_reader :format_settings_reframes, 'Format_Settings_ReFrames'
    mediainfo_attr_reader :format_settings_matrix, 'Format_Settings_Matrix'
    mediainfo_attr_reader :format_settings_gop, 'Format_Settings_GOP'
    mediainfo_attr_reader :format_commercial, 'Format_Commercial'
    
    mediainfo_attr_reader :colour_primaries, 'colour_primaries'
    alias_method :color_primaries, :colour_primaries

    mediainfo_attr_reader :transfer_characteristics, 'transfer_characteristics'
    mediainfo_attr_reader :matrix_coefficients, 'matrix_coefficients'

    mediainfo_attr_reader :codec_id, 'CodecID'
    mediainfo_attr_reader :codec_info, 'CodecID/Info'
    mediainfo_attr_reader :codec, 'Codec'
    alias_method :codec_id_info, :codec_info

    mediainfo_attr_reader :frame_rate, 'FrameRate'
    def fps
      frame_rate[/[\d.]+/].to_f if frame_rate
    end
    alias_method :framerate, :fps

    mediainfo_attr_reader :minimum_frame_rate, 'FrameRate_Minimum'
    def min_fps
      minimum_frame_rate[/[\d.]+/].to_f if video?
    end
    alias_method :min_framerate, :min_fps

    mediainfo_attr_reader :maximum_frame_rate, 'FrameRate_Maximum'
    def max_fps
      maximum_frame_rate[/[\d.]+/].to_f if video?
    end
    alias_method :max_framerate, :max_fps

    mediainfo_attr_reader :frame_rate_mode, 'FrameRate_Mode'

    mediainfo_attr_reader :display_aspect_ratio, 'DisplayAspectRatio'

    mediainfo_attr_reader :bits_pixel_frame, "Bits-(Pixel*Frame)"

    mediainfo_int_reader :width, 'Width'
    mediainfo_int_reader :height, 'Height'

    def frame_size
      "#{width}x#{height}" if width && height
    end

    mediainfo_date_reader :encoded_date, 'Encoded_Date'
    mediainfo_date_reader :tagged_date, 'Tagged_Date'
    
    mediainfo_attr_reader :standard, 'Standard'
  end
end

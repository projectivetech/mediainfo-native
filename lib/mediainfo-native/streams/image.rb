module MediaInfoNative
  class ImageStream < BaseStream
    mediainfo_attr_reader :resolution, 'Resolution'
    mediainfo_attr_reader :format, 'Format'
    
    mediainfo_int_reader :width, 'Width'
    mediainfo_int_reader :height, 'Height'
   
    def frame_size
      "#{width}x#{height}" if width && height
    end
  end
end

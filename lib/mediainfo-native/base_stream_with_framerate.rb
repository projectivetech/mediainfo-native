module MediaInfoNative
  class BaseStreamWithFramerate < BaseStream
    mediainfo_attr_reader :frame_rate, 'FrameRate'
    def fps
      frame_rate[/[\d.]+/].to_f if frame_rate
    end
    alias_method :framerate, :fps
  end
end

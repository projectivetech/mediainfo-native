module MediaInfoNative 
  class MediaInfo

    # These are parts of the convenience wrapper of the old Mediainfo gem.
    # I recklessly throw lots of the noise away. Hope it resembles the old
    # API.

    BaseStream::TYPES.each do |type|
      define_method "#{type}" do
        StreamProxy.new(streams.select { |s| s.stream_type == type  })
      end
    end

    def method_missing(meth, *args, &block)
      self.general.send(meth, *args, &block)
    end

    [:video, :audio, :image].each do |t|
      define_method "#{t}?" do
        self.send(t).count > 0
      end
    end
  end

  class StreamProxy
    def initialize(streams)    
      @streams = streams
    end
    
    SingleStreamAPIError   = Class.new(RuntimeError)
    NoStreamsForProxyError = Class.new(NoMethodError)
    UnknownAttributeError  = Class.new(NoMethodError)
    
    def [](idx); @streams[idx]; end
    def count; @streams.size; end

    def method_missing(m, *a, &b)
      case @streams.size
      when 0
        raise NoStreamsForProxyError
      when 1
        if @streams.first.respond_to?(m)
          @streams.first.send(m, *a, &b)
        else
          raise UnknownAttributeError
        end
      else
        raise SingleStreamAPIError
      end
    end
  end
end

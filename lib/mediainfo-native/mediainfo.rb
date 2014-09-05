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

    def respond_to?(meth, include_all = false)
      (is_open? && self.general.respond_to?(meth, include_all)) || super
    end

    def method_missing(meth, *args, &block)
      if self.general.respond_to?(meth)
        self.general.send(meth, *args, &block)
      else
        super
      end
    end

    [:video, :audio, :image, :other].each do |t|
      define_method "#{t}?" do
        self.send(t).count > 0
      end
    end

    # Alias the open method as it is actually a Kernel method and
    # brings trouble with method_missing approaches.
    alias_method :open_file, :open
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

    def respond_to?(meth, include_all = false)
      begin
        case @streams.size
        when 0
          false
        when 1
          @streams.first.respond_to?(meth, include_all)
        else
          raise SingleStreamAPIError
        end
      end || super
    end

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

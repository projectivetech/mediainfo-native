module MediaInfoNative
  class BaseStream
    extend AttrReaders

    TYPES = [:general, :video, :audio, :image, :menu, :text, :other]

    def stream_type
      /(\w+)Stream/.match(self.class.name)[1].downcase.to_sym
    end

    TYPES.each do |type| 
      define_method("#{type}?") { type == stream_type }
    end
  end
end

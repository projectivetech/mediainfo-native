module MediaInfoNative
  module AttrReaders
    def supported_attribute(attribute)
      @supported_attributes ||= []
      @supported_attributes << attribute
    end

    def supported_attributes
      @supported_attributes || []
    end

    def mediainfo_attr_reader(attribute, mediainfo_key)     
      attribute_before_type_cast = "#{attribute}_before_type_cast"
      
      define_method attribute_before_type_cast do
        instance_variable_get("@#{attribute_before_type_cast}") || instance_variable_set("@#{attribute_before_type_cast}", lookup(mediainfo_key))
      end
      
      define_method attribute do
        if v = instance_variable_get("@#{attribute}")
          v
        else
          v = send(attribute_before_type_cast)
          v = yield v if v and block_given?
          
          instance_variable_set("@#{attribute}", v)
        end
      end

      supported_attribute(attribute)
    end
    
    def mediainfo_duration_reader(*a)
      mediainfo_attr_reader *a do |v|
        case
        when v.include?(":")
          # If it is like 00:20:30.600
          splitted = v.split(/:|\./)
          (splitted[0].to_i * 60 * 60 * 1000) + 
            (splitted[1].to_i * 60 * 1000) + 
            (splitted[2].to_i * 1000) +
            (splitted[3].to_i)
        when v.include?('ms')
          # If it is like '20mn 30s 600ms'
          t = 0
          v.split(/\s+/).each do |tf|
            case tf
            # TODO: Haven't actually seen how they represent hours yet 
            # but hopefully this is ok.. :\
            when /\d+h/  then t += tf.to_i * 60 * 60 * 1000
            when /\d+mn/ then t += tf.to_i * 60 * 1000
            when /\d+ms/ then t += tf.to_i
            when /\d+s/  then t += tf.to_i * 1000
            end
          end
          t
        when /\A\d+\z/ =~ v
          v.to_i
        when /\A\d+\.\d+\z/ =~ v
          v.to_f.round
        else
          puts "TimeFragment: #{v}"
          raise "unexpected time fragment! please report bug!"
        end
      end
    end
    
    def mediainfo_date_reader(*a)
      # Mediainfo can return wrong timestamps, so we have to correct them before
      # we let ruby try to parse.
      # Also we now catch exceptions.
      mediainfo_attr_reader(*a) do |v|
        begin
          Time.parse(v.gsub('-00', '-01'))
        rescue
          Time.now
        end
      end
    end
    
    def mediainfo_int_reader(*a)
      mediainfo_attr_reader(*a) { |v| v.gsub(/\D+/, "").to_i }
    end
  end
end

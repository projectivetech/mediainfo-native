module MediaInfoNative
  module Watchdog
    class MediaInfo < MediaInfoNative::Watchdog::WatchdogProxy
      # We kill mediainfo after 5 minutes.
      DEFAULT_TIMEOUT = 300

      def initialize
        super(MediaInfoNative::MediaInfo, DEFAULT_TIMEOUT)
      end
    end
  end
end

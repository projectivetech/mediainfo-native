require 'drb'
require 'securerandom'
require 'timeout'

module MediaInfoNative
  module Watchdog

    # The RemoteObjectProcess encapsulates the child process that
    # provides the DRb service object.
    class RemoteObjectProcess
      def initialize(uri, klass)
        @pid = fork do
          RemoteObject.new(uri, klass).loop!
        end

        # Wait for DRb connection to become ready.
        sleep 0.2
      end

      def term
        # Exceptions in these methods almost always mean the process is already dead.
        Process.kill('TERM', @pid) rescue nil
      end

      def kill
        Process.kill('KILL', @pid) rescue nil
      end

      def wait
        Process.wait(@pid) rescue nil
      end

      private

      # RemoteObject is the base 'application' class for the child process.
      # It starts the DRb service and enters a simple event loop.
      class RemoteObject
        def initialize(uri, klass)
          @m = Mutex.new
          @c = ConditionVariable.new

          DRb.start_service(uri, RemoteObjectProxy.new(self, klass))
        end

        def loop!
          # Sleep to keep the DRb service running, until woken up.
          @m.synchronize do
            @c.wait(@m)
          end
        end

        def stop!
          Thread.new do
            # Wait for DRb answer package to be sent.
            sleep 0.2

            # End main thread -> exit.
            @m.synchronize do
              @c.signal
            end
          end
        end
      end

      # RemoteObjectProxy is a simple adapter class for the 'real' class
      # to be served via DRb. In addition to the real class' methods it
      # provides a 'stop!' method that brings down the child process.
      class RemoteObjectProxy
        def initialize(mother, klass)
          @mother = mother
          @obj    = klass.new
        end

        def stop!
          @mother.stop!
        end

        def respond_to?(meth, include_all = false)
          public_methods(include_all).include?(meth) || @obj.respond_to?(meth, include_all)
        end

        def method_missing(meth, *args, &block)
          if @obj.respond_to?(meth)
            @obj.send(meth, *args, &block)
          else
            super
          end
        end
      end
    end

    # This exception is raised when the watchdog bites.
    class WatchdogTimeoutOccurred < StandardError; end

    # WatchdogProxy creates an instance of a class on a child process
    # and watches the execution of remote procedure calls.
    class WatchdogProxy
      def self.register_remote_object(uri, object)
        @objects ||= {}
        @objects[uri] = object
      end

      def self.stop_remote_object(uri)
        @objects[uri] && @objects[uri].stop! rescue nil
      end

      def self.stop_object_proc(uri)
        proc { WatchdogProxy.stop_remote_object(uri) }
      end

      def initialize(klass, timeout)
        @timeout = timeout
        uri      = "drbunix:///tmp/#{SecureRandom.hex(8)}"

        # Fork the child process that serves the class.
        @child   = RemoteObjectProcess.new(uri, klass)

        # Start a local DRbServer (unnamed?) for callbacks (blocks!).
        # Each new Watchdog will overwrite the main master DRbServer.
        # This looks weird, but doesn't affect concurrent uses of multiple
        # Watchdogs, I tested it. Trust me.
        DRb.start_service

        # Get a DRb reference to the remote class.
        @object  = DRbObject.new_with_uri(uri)

        # Stop the child process on GC.
        # See also: http://www.mikeperham.com/2010/02/24/the-trouble-with-ruby-finalizers/
        WatchdogProxy.register_remote_object(uri, @object)
        ObjectSpace.define_finalizer(self, self.class.stop_object_proc(uri))
      end

      def respond_to?(meth, include_all = false)
        public_methods(include_all).include?(meth) || @object.respond_to?(meth, include_all)
      end

      def method_missing(meth, *args, &block)
        if @object.respond_to?(meth)
          begin
            Timeout.timeout(@timeout) do
              @object.send(meth, *args, &block)
            end
          rescue Timeout::Error
            @child.term
            sleep 5
            @child.kill
            @child.wait
            fail WatchdogTimeoutOccurred
          end
        else
          super
        end
      end
    end

  end
end

require 'serialport'

class Arduino
  def initialize port_str, baud_rate
    @port_str  = port_str
    @baud_rate = baud_rate
    @data_bits = 8
    @stop_bits = 1

    @arduino_serialport = SerialPort.new @port_str,
                                         @baud_rate,
                                         @data_bits,
                                         @stop_bits
  end

  def send_string s
    slice_size = 8
    (s.length.div slice_size).times do
      |i|
      @arduino_serialport.print s[(slice_size * i)...(slice_size * (i + 1))]
    end

    if s.length % slice_size > 0 
      @arduino_serialport.print s[(s.length - s.length % slice_size)..-1]
    end

    @arduino_serialport.print "\n"
  end

  def << s
    send_string s
  end

  def read_string
    s = @arduino_serialport.gets
    s.is_a?(String) ? s : nil
  end

  def flush_input
    until @arduino_serialport.flush_input
    end
  end

  def >> s
    s << read_string
  end
  
end

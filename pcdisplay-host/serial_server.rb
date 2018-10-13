require_relative 'arduino_serial'
require 'byebug'

$rules = {}
$interval = 0.1
$verbose = false
$stdio = false
$ports = []
$arduino = nil
$bauld = nil

# begin
#   $arduino = Arduino.new nil, nil
# rescue Errno::ENOENT => e
#   puts e
# end

def verbose v
  $verbose = v
end

def port_path port_str
  $ports += Dir[port_str]
end

def bauld b
  $bauld = b
end

def request request_name, &block
  $rules[request_name] = lambda(&block)
end

def parse_and_execute raw_string
  begin
    splitted = raw_string.split
  rescue ArgumentError
    return nil
  end
  command = splitted[0]
  args = splitted[1..-1]

  unless $rules[command].nil?
    return $rules[command].(*args)
  end
  
  nil
end

def receive_string
  s = $stdio ? $stdin.gets : $arduino.read_string
  if $verbose then puts "RECEIVED #{s}" end
  return s
end

def send_string s
  $stdio ? $nstdout.puts(s) : $arduino.send_string(s)
  if $verbose then puts "SENT #{s}" end
end

def get_valid_arduino
  if $bauld && $ports && $ports.length > 0
    Arduino.new($ports.reject {|x| x.nil?}[0], $bauld)
  else
    nil
  end
end

def serve
  $arduino = get_valid_arduino
  if !$stdio then $arduino.flush_input end
  loop do
    raw_string = receive_string

    unless raw_string.nil?
      response = parse_and_execute raw_string

      if response.nil?
        send_string 'NULL'
      else
        send_string response
      end
    end
    
    sleep $interval
  end
end

at_exit do
  serve
end

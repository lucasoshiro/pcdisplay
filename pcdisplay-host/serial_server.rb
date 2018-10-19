require_relative 'arduino_serial'
require 'byebug'
require 'timeout'

$rules = {}
$interval = 0.1
$verbose = false
$port_paths = []
$timeout = 4
$arduino = nil
$bauld = nil

def verbose v
  $verbose = v
end

def port_path port_str
  $port_paths << port_str
end

def bauld b
  $bauld = b
end

def timeout t
  $timeout = t
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
  Timeout.timeout $timeout do
    s = $arduino.read_string
    if $verbose then puts "RECEIVED #{s}" end
    return s
  end
end

def send_string s
  Timeout.timeout $timeout do
    $arduino.send_string s
    if $verbose then puts "SENT #{s}" end
  end
end

def get_valid_arduino
  ports = $port_paths.map{|path| Dir[path]}.reduce :+
  if $bauld && ports && ports.length > 0
    Arduino.new(ports.reject {|x| x.nil?}[0], $bauld)
  else
    nil
  end
end

def serve
  loop do
    $arduino = get_valid_arduino

    if $arduino
      loop do
        begin
          raw_string = receive_string
          unless raw_string.nil?
            response = parse_and_execute raw_string

            if response.nil?
              send_string 'NULL'
            else
              send_string response
            end
            sleep $interval
          end
        rescue Timeout::Error
          break
        end
      end
    end
      
    sleep $interval
  end
end

at_exit do
  serve
end

load 'arduino_serial.rb'
require 'byebug'

$rules = {}
$mutex = Mutex.new
$interval = 0.1
$arduino = Arduino.new nil, nil
$verbose = false

def verbose v
  $verbose = v
end

def request request_name, &block
  $mutex.synchronize do
    $rules[request_name] = lambda(&block)
  end
end

def parse_and_execute raw_string
  splitted = raw_string.split
  command = splitted[0]
  args = splitted[1..-1]

  unless $rules[command].nil?
    return $rules[command].(*args)
  end
  
  nil
end

def receive_string
  s = $arduino.read_string
  if $verbose then puts "RECEIVED #{s}" end
  return s
end

def send_string s
  $arduino.send_string s
  if $verbose then puts "SENT #{s}" end
end

$server_thread = Thread.new do
  loop do
    raw_string = receive_string

    unless raw_string.nil?
      $mutex.synchronize do
        response = parse_and_execute raw_string

        if response.nil?
          send_string 'NULL'
        else
          send_string response
        end
      end
    end
    
    sleep $interval
  end
end

at_exit do
  $server_thread.join
end
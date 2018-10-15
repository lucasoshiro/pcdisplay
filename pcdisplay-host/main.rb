#!/usr/bin/env ruby

require_relative 'pc'
require_relative 'serial_server'

require 'shellwords'
require 'i18n'

$verbose = ARGV.member? '-v'
$pc = PC.instance

I18n.config.available_locales = :en

bauld 9600
port_path '/dev/ttyACM*'
port_path '/dev/ttyUSB*'
timeout 5

request 'HELLO' do
  'HELLO'
end

request 'CPU' do
  "CPU #{($pc.cpu_usage * 100).to_i}"
end

request 'RAM' do
  ram = $pc.ram_info
  
  used_mem  = ram[:usedMem]
  total_mem = ram[:totalMem]
  
  "RAM #{used_mem} #{total_mem}"
end

request 'NAME' do
  name = Shellwords.escape $pc.pc_name
  "NAME #{name}"
end

request 'NET' do
  net_info = $pc.net_info
  
  down = net_info[:downSpeed]
  up   = net_info[:upSpeed]
  "NET #{down} #{up}"
end

request 'TIME' do
  time = $pc.time
  
  day   = time.day
  month = time.month
  year  = time.year
  hour  = time.hour
  min   = time.min
  sec   = time.sec
  
  "TIME #{day} #{month} #{year} #{hour} #{min} #{sec}"
end

request 'VOLUME' do
  |volume|
  if volume then $pc.sound_volume = volume.to_i end
  "VOLUME #{$pc.sound_volume}"
end

request 'TEMP' do
  "TEMP #{$pc.cpu_temperature}"
end

request 'SYSINFO' do
  name = Shellwords.escape $pc.sys_info[:NAME]
  "SYSINFO #{name}"
end

request 'MEDIA' do
  nil

  # DEPRECATED !!!
  # if $pc.active_players_name.length == 0
  #   nil
  # else
  #   # puts '$pc.active_players_name'
  #   # print $pc.active_players_name
  #   # puts ''
  #   player = $pc.active_players_name[0]
  #   metadata = $pc.player_metadata player

  #   title  = Shellwords.escape(I18n.transliterate(metadata[:title] || ''))[0..63]
  #   album  = Shellwords.escape(I18n.transliterate(metadata[:album] || ''))[0..63]
  #   artist = Shellwords.escape(I18n.transliterate(metadata[:artist] || ''))[0..63]
  #   track  = metadata[:track] || '0'
    
  #   "MEDIA #{title} #{album} #{artist} #{track}"
  # end
end

request 'MEDIA_PLAY_PAUSE' do
  player = $pc.active_players_name[0]
  player and $pc.player_play_pause player
  nil
end

request 'MEDIA_NEXT' do
  player = $pc.active_players_name[0]
  player and $pc.player_next player
  nil
end

request 'MEDIA_PREV' do
  player = $pc.active_players_name[0]
  player and $pc.player_prev player
  nil
end

puts 'ready'

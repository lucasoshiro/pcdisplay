#!/usr/bin/env ruby

load 'pc.rb'
load 'serial_server.rb'
require 'shellwords'

$verbose = ARGV.member? '-v'

PC.instance

request 'HELLO' do
  'HELLO'
end

request 'CPU' do
  "CPU #{(PC.instance.cpu_usage * 100).to_i}"
end

request 'RAM' do
  ram = PC.instance.ram_info
  
  used_mem  = ram[:usedMem]
  total_mem = ram[:totalMem]
  
  "RAM #{used_mem} #{total_mem}"
end

request 'NAME' do
  "NAME #{PC.instance.pc_name}"
end

request 'NET' do
  net_info = PC.instance.net_info
  
  down = net_info[:downSpeed]
  up   = net_info[:upSpeed]
  "NET #{down} #{up}"
end

request 'TIME' do
  time = PC.instance.time
  
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
  if volume then PC.instance.sound_volume = volume.to_i end
  "VOLUME #{PC.instance.sound_volume}"
end

request 'TEMP' do
  "TEMP #{PC.instance.cpu_temperature}"
end

request 'SYSINFO' do
  name = PC.instance.sys_info[:NAME]
  "SYSINFO #{name}"
end

request 'MEDIA' do
  if PC.instance.active_players_name.length == 0
    nil
  else
    player = PC.instance.active_players_name[0]
    metadata = PC.instance.player_metadata player
    
    title  = Shellwords.escape(metadata[:title])  || ''
    album  = Shellwords.escape(metadata[:album])  || ''
    artist = Shellwords.escape(metadata[:artist]) || ''
    track  = metadata[:track]                     || '-1'
    
    "MEDIA #{title} #{album} #{artist} #{track}"
  end
end

request 'MEDIA_PLAY_PAUSE' do
  player = PC.instance.active_players_name[0]
  player and PC.instance.player_play_pause player
  nil
end

request 'MEDIA_NEXT' do
  player = PC.instance.active_players_name[0]
  player and PC.instance.player_next player
  nil
end

request 'MEDIA_PREV' do
  player = PC.instance.active_players_name[0]
  player and PC.instance.player_prev player
  nil
end

puts 'ready'

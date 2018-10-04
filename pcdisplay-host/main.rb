#!/usr/bin/env ruby

load 'pc.rb'
load 'serial_server.rb'

require 'shellwords'
require 'i18n'

$verbose = ARGV.member? '-v'
$stdio = ARGV.member? '-i'

PC.instance

I18n.config.available_locales = :en

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
  name = Shellwords.escape PC.instance.pc_name
  "NAME #{name}"
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
  name = Shellwords.escape PC.instance.sys_info[:NAME]
  "SYSINFO #{name}"
end

request 'MEDIA' do
  if PC.instance.active_players_name.length == 0
    nil
  else
    puts PC.instance.active_players_name
    player = PC.instance.active_players_name[0]
    metadata = PC.instance.player_metadata player

    title  = Shellwords.escape(I18n.transliterate(metadata[:title] || ''))[0..63]
    album  = Shellwords.escape(I18n.transliterate(metadata[:album] || ''))[0..63]
    artist = Shellwords.escape(I18n.transliterate(metadata[:artist] || ''))[0..63]
    track  = metadata[:track] || '0'
    
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

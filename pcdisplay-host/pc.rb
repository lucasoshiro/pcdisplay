require 'singleton'
require_relative 'player'

class PC
  include Singleton
  attr_reader :sys_info,
              :pc_name,
              :cpu_usage,
              :ram_info,
              :net_info,
              :cpu_temperature

  attr_writer :intervals

  def initialize
    @sys_info = `cat /etc/os-release`.lines.map do |line|
      x = line.gsub("\n", '').split('=')
      {x[0].to_sym => x[1].gsub('"', '')}
    end.reduce(:merge)

    @pc_name = `hostname`.gsub("\n", '')

    @intervals = {
      refresh_cpu: 1,
      refresh_ram: 1,
      refresh_net: 1,
      refresh_cpu_temperature: 1,
      refresh_players: 1,
      refresh_volume: 10
    }

    @players = {}
    @volume_mutex = Mutex.new

    auto_refresh
  end

  def time
    Time.now
  end

  def sound_volume
    @volume_mutex.synchronize {@sound_volume}
  end

  def sound_volume= volume
    if volume.is_a?(Integer) &&
       volume >= 0 && volume <= 100 &&
       volume != @sound_volume
      `amixer -D pulse set Master #{volume}%`
      @volume_mutex.synchronize {@sound_volume = volume}
    end
    return sound_volume
  end

  def active_players_name
    @players.map {|name, _| name}
  end

  def player_play_pause name
    @players[name].send_control :PlayPause
    nil
  end

  def player_next name
    @players[name].send_control :Next
    nil
  end

  def player_prev name
    @players[name].send_control :Previous
    nil
  end

  def player_metadata name
    @players[name].metadata
  end

  private

  def auto_refresh
    [:refresh_cpu,
     :refresh_ram,
     :refresh_net,
     :refresh_cpu_temperature,
    # :refresh_players
    ].each do |refresh_method|
      self.send refresh_method
      Thread.new do
        loop do

          begin
            self.send refresh_method
          rescue Exception => e
            puts e.backtrace
            puts ''
            puts ''
          end
          sleep @intervals[refresh_method]
        end
      end
    end

    nil
  end

  def refresh_cpu
    cpu_stat = `cat /proc/stat`.lines[0].split[1..-1].map {|v| v.to_f}
    old_total_cpu_time = cpu_stat.reduce(:+)
    old_total_cpu_usage = old_total_cpu_time - cpu_stat[3]

    sleep 1

    cpu_stat = `cat /proc/stat`.lines[0].split[1..-1].map {|v| v.to_f}
    new_total_cpu_time = cpu_stat.reduce(:+)
    new_total_cpu_usage = new_total_cpu_time - cpu_stat[3]
    
    @cpu_usage =  (new_total_cpu_usage - old_total_cpu_usage) / (new_total_cpu_time - old_total_cpu_time)
    nil
  end

  def refresh_ram
    free = `free --mebi`.lines.map{|line| line.split}
    @ram_info = {
      totalMem: free[1][1],
      usedMem:  free[1][2],
      totalSwp: free[2][1],
      usedSwp:  free[2][2]
    }
    nil
  end

  def refresh_net
    dt = 1
    table = nil
    
    file = File.new('/proc/net/dev', 'r')
    table = file.readlines[2..-1].map{|line| line.split[1..-1].map{|val| val.to_i}}
    file.close

    old_sum_down = table.inject(0){|sum, line| sum + line[0]}
    old_sum_up   = table.inject(0){|sum, line| sum + line[8]}

    sleep dt

    file = File.new('/proc/net/dev', 'r')
    table = file.readlines[2..-1].map{|line| line.split[1..-1].map{|val| val.to_i}}
    file.close

    new_sum_down = table.inject(0){|sum, line| sum + line[0]}
    new_sum_up   = table.inject(0){|sum, line| sum + line[8]}

    @net_info = {
      upSpeed: (new_sum_up - old_sum_up) / dt,
      downSpeed: (new_sum_down - old_sum_down) / dt
    }
    nil
  end

  def refresh_players
    puts 'M'
    active_players_names = Player.active_players_names

    puts 'active_players_names'
    print active_players_names
    puts ''

    dead_players_names = @players.keys - active_players_names
    new_players_names = active_players_names - @players.keys

    dead_players_names.each do
      |name|
      @players[name].close
      @players.delete[name]
    end

    new_players_names.each do
      |name|
      @players[name] = Player.new name
    end

    nil
  end
  
  def refresh_cpu_temperature
    sensors_output = `sensors -u`
    temps = sensors_output.lines
              .select{|line| line.match?(/^  temp[0-9]+_input/)}
              .map{|line| line.split[1].to_i}

    @cpu_temperature = temps[0]
  end

  def refresh_volume
    volume = `amixer -D pulse get Master`.lines[-2..-1].map do
      |line|
      line.split[-2].gsub(/[\]\[%]/, '').to_i
    end.sum / 2

    @volume_mutex.synchronize {@sound_volume = volume}
  end
end

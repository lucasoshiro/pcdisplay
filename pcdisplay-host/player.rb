require 'dbus'
require 'byebug'

$VERBOSE = nil

class Player
  @@bus = DBus::SessionBus.instance

  def self.active_players_names
    @@bus.proxy.ListNames[0].select do
      |name|
      name.match(/^org\.mpris\.MediaPlayer2/)
    end
  end

  def initialize service_name
    @service_name = service_name
    @player_interface = @@bus.service(@service_name).object('/org/mpris/MediaPlayer2')['org.mpris.MediaPlayer2.Player']
  end

  def initialize name
    dbus_player = @@bus.service(name).object('/org/mpris/MediaPlayer2')
    @player_interface = dbus_player['org.mpris.MediaPlayer2.Player']
    @properties_interface = dbus_player['org.freedesktop.DBus.Properties']
    @name = name
    @metadata = {}
    @mutex = Mutex.new

    refresh_metadata

    begin
      @properties_interface.on_signal 'PropertiesChanged' do
        refresh_metadata
    end
    rescue Exception => m
      print m
    end

    @loop = DBus::Main.new
    @loop << @@bus

    @metadata_thread = Thread.new {@loop.run}
  end

  def refresh_metadata
    begin
      m = _metadata

      @mutex.synchronize do
        @metadata = m
      end
    rescue
    end
  end

  def send_control control
    Thread.new do
      begin
        @player_interface.send control
      rescue
      end
    end
    nil
  end

  def _metadata
    if @properties_interface.nil? || !@properties_interface.respond_to?(:Get) then
      {}
    else
      @properties_interface.Get('org.mpris.MediaPlayer2.Player', 'Metadata')[0]
    end
  end

  def metadata
    m = {}
    @mutex.synchronize {m = @metadata}

    if m.empty? then m
    else
      {
        title:  m['xesam:title'],
        album:  m['xesam:album'],
        artist: m['xesam:artist'] ? m['xesam:artist'][0] : nil,
        track:  m['xesam:trackNumber']
      }
    end
  
  end

  def close
    @metadata_thread.kill
  end
end

require 'dbus'

class Player
  def self.active_players
    bus = DBus.instance
  end
end

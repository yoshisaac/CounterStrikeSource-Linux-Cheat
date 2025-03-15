#include "bhop.hpp"

#include "player_info.hpp"

#include "../xutil.hpp"
#include "../memory.hpp"

#include "../client.hpp"

#include "../gui/config.hpp"

void bhop(pid_t game_pid, Display* bhop_display) {
  for (;;) {
    if (Xutil::window_to_pid(bhop_display, Xutil::focused_window(bhop_display)) != game_pid || !config.misc.bhop) {
      usleep(1000*1000/250); //at most, servers will run at a 128 ticks per second.
      continue; 
    }

    PlayerInfo::Player plocal = PlayerInfo::get_local_player();

    if (plocal.on_ground == true && Xutil::key_down(bhop_display, ' ') == true) {
      int DO = 5;
      Memory::write(game_pid, Client::force_jump, &DO, sizeof(int));
    } else {
      int DONT = 4;
      Memory::write(game_pid, Client::force_jump, &DONT, sizeof(int));
    }
    
    usleep(1000*1000/250);
  }
}


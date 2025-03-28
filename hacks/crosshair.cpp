#include "crosshair.hpp"

#include "player_info.hpp"

#include "../gui/config.hpp"

#include "../xutil.hpp"

void crosshair(pid_t game_pid, XdbeBackBuffer back_buffer, Display* draw_display, Window window, GC gc) {
  if (Xutil::window_to_pid(draw_display, Xutil::focused_window(draw_display)) != game_pid) return;

  int screen_center[2] = {1920/2, 1080/2};

  PlayerInfo::Player plocal = PlayerInfo::get_local_player();

  XSetLineAttributes(draw_display, gc, 2, LineSolid, CapButt, JoinBevel);

  
  if (config.visuals.crosshair) {
    XSetForeground(draw_display, gc, Xutil::xcolor_from_rgb(config.visuals.crosshair_color[0],
							    config.visuals.crosshair_color[1],
							    config.visuals.crosshair_color[2],
							    draw_display));
  
    XDrawLine(draw_display, back_buffer, gc, screen_center[0]-13, screen_center[1], screen_center[0]+14, screen_center[1]);
    XDrawLine(draw_display, back_buffer, gc, screen_center[0], screen_center[1]-13, screen_center[0], screen_center[1]+14);
  }

  if (config.visuals.crosshair_recoil) {
    XSetForeground(draw_display, gc, Xutil::xcolor_from_rgb(config.visuals.crosshair_recoil_color[0],
							    config.visuals.crosshair_recoil_color[1],
							    config.visuals.crosshair_recoil_color[2],
							    draw_display));
  
    XDrawLine(draw_display, back_buffer, gc,
	      (screen_center[0] - (int(screen_center[0] / 90) * plocal.aim_punch[1])) - 13,
	      (screen_center[1] + (int(screen_center[1] / 45) * plocal.aim_punch[0])),
	      (screen_center[0] - (int(screen_center[0] / 90) * plocal.aim_punch[1])) + 14,
	      (screen_center[1] + (int(screen_center[1] / 45) * plocal.aim_punch[0])));

    XDrawLine(draw_display, back_buffer, gc,
	      (screen_center[0] - (int(screen_center[0] / 90) * plocal.aim_punch[1])),
	      (screen_center[1] + (int(screen_center[1] / 45) * plocal.aim_punch[0])) - 13,
	      (screen_center[0] - (int(screen_center[0] / 90) * plocal.aim_punch[1])),
	      (screen_center[1] + (int(screen_center[1] / 45) * plocal.aim_punch[0])) + 14);
  }
}

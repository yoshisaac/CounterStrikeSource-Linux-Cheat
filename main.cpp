#include <unistd.h>
#include <stdio.h>
#include <thread>

#include "hacks/player_info.hpp"
#include "hacks/aimbot.hpp"
#include "hacks/draw.hpp"
#include "hacks/bhop.hpp"

#include "gui/gui.hpp"

#include "client.hpp"
#include "engine.hpp"

#include "process.hpp"
#include "memory.hpp"
#include "xutil.hpp"

int main(int argc, char *argv[]) {
  if (getuid()) { //check if root
    printf("Please run as root\n");
    return 1;
  }

  pid_t game_pid = Process::name_to_pid("cstrike_linux64");
  if (!game_pid) {
    printf("Game not open\n");
    return 1;
  }
  
  printf("Game pid: %d\n", game_pid);  
  
  /* X11 initiation */
  Display* display = XOpenDisplay(NULL);
  if (!display) return 1;

  Display* draw_display = XOpenDisplay(NULL);
  if (!draw_display) return 1;

  Display* bhop_display = XOpenDisplay(NULL);
  if (!bhop_display) return 1;

  Display* aim_display = XOpenDisplay(NULL);
  if (!aim_display) return 1;  
  
  int screen = DefaultScreen(display);

  int shape_event_base;
  int shape_error_base;

  if (!XShapeQueryExtension(display, &shape_event_base, &shape_error_base)) {
    printf("cs-source-hack: NO shape extension in your system !\n");
    return 1;
  }

  Window root = DefaultRootWindow(display);

  Visual* visual = DefaultVisual(display, screen);

  XVisualInfo vinfo;
  XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);
  Colormap colormap = XCreateColormap(display, DefaultRootWindow(display), vinfo.visual, AllocNone);

  Xutil::Color bgcolor = Xutil::xcolor_from_rgba(0, 0, 0, 0, display);
  
#define BASIC_EVENT_MASK (StructureNotifyMask|ExposureMask|PropertyChangeMask|EnterWindowMask|LeaveWindowMask|KeyPressMask|KeyReleaseMask|KeymapStateMask)
#define NOT_PROPAGATE_MASK (KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|PointerMotionMask|ButtonMotionMask)

  XSetWindowAttributes attr;
  attr.background_pixmap = None;
  attr.background_pixel = bgcolor;
  attr.border_pixel=0;
  attr.win_gravity=NorthWestGravity;
  attr.bit_gravity=ForgetGravity;
  attr.save_under=1;
  attr.event_mask=BASIC_EVENT_MASK;
  attr.do_not_propagate_mask=NOT_PROPAGATE_MASK;
  attr.override_redirect=1; // OpenGL > 0
  attr.colormap = colormap;

  unsigned long mask = CWColormap | CWBorderPixel | CWBackPixel | CWEventMask | CWWinGravity|CWBitGravity | CWSaveUnder | CWDontPropagate | CWOverrideRedirect;

  Window window = XCreateWindow(display, root, 0, 0, 1920, 1080, 0, vinfo.depth, InputOutput, vinfo.visual, mask, &attr);
  
  XShapeCombineMask(display, window, ShapeInput, 0, 0, None, ShapeSet);
    
  XShapeSelectInput(display, window, ShapeNotifyMask);
  
  XserverRegion region = XFixesCreateRegion(display, NULL, 0);
  XFixesSetWindowShapeRegion(display, window, ShapeInput, 0, 0, region);
  XFixesDestroyRegion(display, region);

  XdbeBackBuffer back_buffer = XdbeAllocateBackBufferName(display, window, 0);
  
  XMapWindow(display, window);

  Draw::black = Xutil::xcolor_from_rgb(0, 0, 0, display);
  Draw::gray = Xutil::xcolor_from_rgb(210, 210, 210, display);
  Draw::green = Xutil::xcolor_from_rgb(0, 255, 50, draw_display);
  Draw::red = Xutil::xcolor_from_rgb(255, 0, 50, draw_display);
  Draw::white = Xutil::xcolor_from_rgb(255, 255, 255, draw_display);
  Draw::cyan = Xutil::xcolor_from_rgb(0, 255, 240, draw_display);
  Draw::yellow = Xutil::xcolor_from_rgb(255, 255, 0, draw_display);
  Draw::orange = Xutil::xcolor_from_rgb(255, 170, 0, draw_display); 
  
  Draw::shadowfont = XLoadQueryFont(display, "6x13bold");
  Draw::font = XLoadQueryFont(display, "6x13");
  /* X11 initiation end */

  const uintptr_t client_address = Memory::module_base_address(game_pid, "linux64/client.so");
  printf("client: %p\n", client_address);
  
  const uintptr_t engine_address = Memory::module_base_address(game_pid, "linux64/engine.so");
  printf("engine: %p\n", engine_address);

  PlayerInfo::entity_list = client_address + 0xFA8528;

  PlayerInfo::ptr_local_player = client_address + 0xF946B0;

  printf("player list: %p\n", PlayerInfo::entity_list);
  printf("local player: %p\n", PlayerInfo::ptr_local_player);

  Client::force_jump = client_address + 0x1017578;
  
  Engine::view_matrix = engine_address + 0xB996A0;
  Engine::view_angles = engine_address + 0xA13494;

  //printf("view_angles: %p\n", Client::view_angles);
  
  std::thread gui_thread(gui, argc, argv);
  pthread_setname_np(gui_thread.native_handle(), "gui_thread");  
  
  std::thread draw_thread(draw, game_pid, back_buffer, draw_display, window);
  pthread_setname_np(draw_thread.native_handle(), "draw_thread");

  std::thread aim_thread(aimbot, game_pid, aim_display);
  pthread_setname_np(aim_thread.native_handle(), "aim_thread");

  std::thread bhop_thread(bhop, game_pid, bhop_display);
  pthread_setname_np(bhop_thread.native_handle(), "bhop_thread");
  
  for (;;) {
    players(game_pid);
    usleep(1000*1000/250);
  }
}

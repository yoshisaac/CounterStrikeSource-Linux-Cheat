#include "esp.hpp"
#include "draw.hpp"

#include "../gui/config.hpp"

#include "player_info.hpp"

#include "../math.hpp"

void esp(pid_t game_pid, XdbeBackBuffer back_buffer, Display* draw_display, Window window, GC gc) {
  if (Xutil::window_to_pid(draw_display, Xutil::focused_window(draw_display)) != game_pid) return;
  if (!config.esp.master) return;
  
  for (unsigned int i = 0; i < 64; ++i) {
    PlayerInfo::Player player = PlayerInfo::get_player(i);
    PlayerInfo::Player plocal = PlayerInfo::get_local_player();

    if (player.index == plocal.index) continue;
    if (player.team_num == plocal.team_num) continue;
    if (player.index == -1) continue;
    if (player.life_state == 1 || player.health <= 0) continue;
    if (int(player.location[0]) == 0 || int(player.location[1]) == 0 || int(player.location[2]) == 0) continue; //dumb check null pointer player objects

    float z_offset = 72;
    if (player.crouched == true)
      z_offset = 54;
    else
      z_offset = 72;
    
    float y_offset[2];
    float location_z_offset[3] = {player.location[0], player.location[1], player.location[2] + z_offset};
    world_to_screen(game_pid, location_z_offset, y_offset);
    
    float screen[2];
    if (!world_to_screen(game_pid, player.location, screen)) continue;

    
    Xutil::Color dormant_color;
    if (player.dormant == true) {
      PlayerInfo::l_players[i].dormant_frames += 1;

      if (player.dormant_alpha-5 > 0 && player.dormant_frames%15 == 0)
	PlayerInfo::l_players[i].dormant_alpha -= 5;

      if (player.dormant_alpha-5 <= 55)
	continue;
      
      dormant_color = Xutil::xcolor_from_rgb(player.dormant_alpha,
					     player.dormant_alpha,
					     player.dormant_alpha,
					     draw_display);
    }	   
    else {
      PlayerInfo::l_players[i].dormant_frames = 0;
      PlayerInfo::l_players[i].dormant_alpha = 220;
    }

     
    float distance = distance_3d(plocal.location, player.location);

    float y_offset_text[2];
    location_z_offset[2] = location_z_offset[2] + 5;
    world_to_screen(game_pid, location_z_offset, y_offset_text);

    float y_offset_text2[2];
    location_z_offset[2] = location_z_offset[2] - 15;
    world_to_screen(game_pid, location_z_offset, y_offset_text2);

    
    if (config.esp.health) {
      //health bar shadow
      XSetForeground(draw_display, gc, Draw::black);
      XSetFont(draw_display, gc, Draw::shadowfont->fid);
      XSetLineAttributes(draw_display, gc, 4, LineSolid, CapButt, JoinMiter);

      XDrawLine(draw_display, back_buffer, gc, screen[0] - (13000/distance) - 5, y_offset[1]-2, screen[0] - (13000/distance) - 5, screen[1]+2);
      XDrawString(draw_display, back_buffer, gc, screen[0] - (13000/distance) + 1, y_offset_text[1] + 1, std::to_string(player.health).c_str(), strlen(std::to_string(player.health).c_str()));
    
      int ydelta = (y_offset[1] - screen[1]) * (1.f - (player.health / 100.f));

      //health bar color
      if (player.health > 100) { //show that they have more health than what is conventional
	XSetForeground(draw_display, gc, Draw::cyan);
	ydelta = 0;
      }
      else if (player.health <= 100 && player.health >= 90)
	XSetForeground(draw_display, gc, Draw::green);
      else if (player.health < 90 && player.health > 60)
	XSetForeground(draw_display, gc, Draw::yellow);
      else if (player.health <= 60 && player.health > 35)
	XSetForeground(draw_display, gc, Draw::orange);
      else if (player.health <= 35)
	XSetForeground(draw_display, gc, Draw::red);

      if (player.dormant == true) XSetForeground(draw_display, gc, dormant_color);
      
      XSetFont(draw_display, gc, Draw::font->fid);


      XSetLineAttributes(draw_display, gc, 2, LineSolid, CapButt, JoinMiter);
      XDrawLine(draw_display, back_buffer, gc, screen[0] - (13000/distance) - 5, screen[1], screen[0] - (13000/distance) - 5, y_offset[1] - ydelta);          
      XDrawString(draw_display, back_buffer, gc, screen[0] - (13000/distance), y_offset_text[1], std::to_string(player.health).c_str(), strlen(std::to_string(player.health).c_str()));
    }      

    //boxes
    if (config.esp.box) {
      XSetForeground(draw_display, gc, Draw::black);
      
      XSetLineAttributes(draw_display, gc, 4, LineSolid, CapButt, JoinMiter);

      XDrawLine(draw_display, back_buffer, gc, (screen[0] - (12000/distance)), y_offset[1] - 1, (screen[0] - (12000/distance)), screen[1] + 1); //left
      XDrawLine(draw_display, back_buffer, gc, (screen[0] + (12000/distance)), y_offset[1] - 1, (screen[0] + (12000/distance)), screen[1] + 1); //right
      XDrawLine(draw_display, back_buffer, gc, screen[0] + (12000/distance) + 1, y_offset[1], screen[0] - (12000/distance) - 1, y_offset[1]); //top
      XDrawLine(draw_display, back_buffer, gc, screen[0] - (12000/distance) - 1, screen[1], screen[0] + (12000/distance) + 1, screen[1]); //bottom
      
      XSetForeground(draw_display, gc, Xutil::xcolor_from_rgb(config.esp.box_color[0], config.esp.box_color[1], config.esp.box_color[2], draw_display));
      if (player.dormant == true) XSetForeground(draw_display, gc, dormant_color);
      
      XSetLineAttributes(draw_display, gc, 2, LineSolid, CapRound, JoinRound);

      XDrawLine(draw_display, back_buffer, gc, (screen[0] - (12000/distance)), y_offset[1], (screen[0] - (12000/distance)), screen[1]);
      XDrawLine(draw_display, back_buffer, gc, (screen[0] + (12000/distance)), y_offset[1], (screen[0] + (12000/distance)), screen[1]);
      XDrawLine(draw_display, back_buffer, gc, screen[0] + (12000/distance), y_offset[1], screen[0] - (12000/distance) - 1, y_offset[1]);
      XDrawLine(draw_display, back_buffer, gc, screen[0] - (12000/distance), screen[1], screen[0] + (12000/distance), screen[1]);
    }

    //dormant flag
    if (player.dormant == true) {
      XSetForeground(draw_display, gc, Draw::gray);
      XDrawString(draw_display, back_buffer, gc, screen[0] + (12000/distance)+4, y_offset_text2[1], "dormant", strlen("dormant"));
    }    

    //snap lines
    if (config.esp.snap_lines) {
      XSetForeground(draw_display, gc, Xutil::xcolor_from_rgb(config.esp.snap_lines_color[0], config.esp.snap_lines_color[1], config.esp.snap_lines_color[2], draw_display));
      XSetLineAttributes(draw_display, gc, 0, LineSolid, CapButt, JoinMiter);
      
      XDrawLine(draw_display, back_buffer, gc, 1920/2, 1080, screen[0], screen[1]);
    }

    //index
    /*
    XSetFont(draw_display, gc, Draw::shadowfont->fid);
    XSetForeground(draw_display, gc, Draw::black);

      
    XDrawString(draw_display, back_buffer, gc, screen[0] + 1, screen[1] - 10 + 1, std::to_string(player.index).c_str(), strlen(std::to_string(player.index).c_str()));

    XSetFont(draw_display, gc, Draw::font->fid);
    XSetForeground(draw_display, gc, Draw::white);

      
    XDrawString(draw_display, back_buffer, gc, screen[0], screen[1] - 10, std::to_string(player.index).c_str(), strlen(std::to_string(player.index).c_str()));
    */

    /*
    XSetForeground(draw_display, gc, Draw::white);
    for (int i = 0; i < 32; ++i) {
      float bone[2]; 
      world_to_screen(game_pid, player.bone_matrix[i], bone);
      XDrawString(draw_display, back_buffer, gc, bone[0], bone[1], std::to_string(i).c_str(), strlen(std::to_string(i).c_str()));
    }
    */

    if (config.esp.skeleton) {
      XSetLineAttributes(draw_display, gc, 0, LineSolid, CapButt, JoinBevel);
      XSetForeground(draw_display, gc, Xutil::xcolor_from_rgb(config.esp.skeleton_color[0], config.esp.skeleton_color[1], config.esp.skeleton_color[2], draw_display));
      if (player.dormant == true) XSetForeground(draw_display, gc, dormant_color);

      float bone1[2];
      float bone2[2];
      //left leg
      world_to_screen(game_pid, player.bone_matrix[0], bone1);
      world_to_screen(game_pid, player.bone_matrix[1], bone2);
      XDrawLine(draw_display, back_buffer, gc, bone1[0], bone1[1], bone2[0], bone2[1]);

      world_to_screen(game_pid, player.bone_matrix[2], bone1);
      XDrawLine(draw_display, back_buffer, gc, bone1[0], bone1[1], bone2[0], bone2[1]);

      //right leg
      world_to_screen(game_pid, player.bone_matrix[0], bone1);
      world_to_screen(game_pid, player.bone_matrix[5], bone2);
      XDrawLine(draw_display, back_buffer, gc, bone1[0], bone1[1], bone2[0], bone2[1]);

      world_to_screen(game_pid, player.bone_matrix[6], bone1);
      XDrawLine(draw_display, back_buffer, gc, bone1[0], bone1[1], bone2[0], bone2[1]);

      //back + head
      world_to_screen(game_pid, player.bone_matrix[0], bone1);
      world_to_screen(game_pid, player.bone_matrix[10], bone2);
      XDrawLine(draw_display, back_buffer, gc, bone1[0], bone1[1], bone2[0], bone2[1]);

      world_to_screen(game_pid, player.bone_matrix[12], bone1);
      XDrawLine(draw_display, back_buffer, gc, bone1[0], bone1[1], bone2[0], bone2[1]);

      world_to_screen(game_pid, player.bone_matrix[14], bone2);
      XDrawLine(draw_display, back_buffer, gc, bone1[0], bone1[1], bone2[0], bone2[1]);

    
      //left arm
      world_to_screen(game_pid, player.bone_matrix[12], bone1);
      world_to_screen(game_pid, player.bone_matrix[16], bone2);
      XDrawLine(draw_display, back_buffer, gc, bone1[0], bone1[1], bone2[0], bone2[1]);

      world_to_screen(game_pid, player.bone_matrix[17], bone1);
      XDrawLine(draw_display, back_buffer, gc, bone1[0], bone1[1], bone2[0], bone2[1]);

      world_to_screen(game_pid, player.bone_matrix[18], bone2);
      XDrawLine(draw_display, back_buffer, gc, bone1[0], bone1[1], bone2[0], bone2[1]);

      //right arm
      world_to_screen(game_pid, player.bone_matrix[12], bone1);
      world_to_screen(game_pid, player.bone_matrix[29], bone2);
      XDrawLine(draw_display, back_buffer, gc, bone1[0], bone1[1], bone2[0], bone2[1]);

      world_to_screen(game_pid, player.bone_matrix[30], bone1);
      XDrawLine(draw_display, back_buffer, gc, bone1[0], bone1[1], bone2[0], bone2[1]);    

      world_to_screen(game_pid, player.bone_matrix[31], bone2);
      XDrawLine(draw_display, back_buffer, gc, bone1[0], bone1[1], bone2[0], bone2[1]);         

    }
    
  }
}

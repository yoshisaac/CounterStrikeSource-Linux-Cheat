#include <unistd.h>
#include <cstdint>
#include <stdio.h>

void players(pid_t game_pid);

namespace PlayerInfo {
  inline uintptr_t entity_list;

  class Player {
  public:
    int index;
    int health;
    int team_num;
    bool dormant;
    bool crouched;
    bool on_ground;
    float height;
    unsigned char life_state;
    float aim_punch[2];
    float location[3];
    float bone_matrix[200][3];

    int dormant_alpha;
    int dormant_frames;

    float fov_aim_distance;
    
    Player() {
      index = -1;
      health = -1;
      team_num = -1;
      dormant = true;
      life_state = -1;
      fov_aim_distance = 999999;
      location[0] = 0;
      location[1] = 0;
      location[2] = 0;
    }

    Player(int index, int health, int team_num, unsigned char life_state,
	   bool dormant, bool crouched, bool on_ground,
	   float height, float location[3],
	   float aim_punch[2],
	   float bone_matrix[200][3],
	   int dormant_alpha, int dormant_frames) {
      this->index = index;
      this->health = health;
      this->team_num = team_num;
      this->life_state = life_state;
      this->dormant = dormant;
      this->crouched = crouched;
      this->on_ground = on_ground;
      
      this->height = height;

      this->aim_punch[0] = aim_punch[0];
      this->aim_punch[1] = aim_punch[1];
      
      this->location[0] = location[0]; 
      this->location[1] = location[1];
      this->location[2] = location[2];

      for (int i = 0; i < 200; ++i) {
	this->bone_matrix[i][0] = bone_matrix[i][0];	
      	this->bone_matrix[i][1] = bone_matrix[i][1];	
      	this->bone_matrix[i][2] = bone_matrix[i][2];	
      }

      this->dormant_alpha = dormant_alpha;
      
      this->dormant_frames = dormant_frames;
    }
  };

  inline Player l_players[64];

  static Player get_player(unsigned int index) {
    if (index > 64) return Player();
    return l_players[index];
  }

  inline int i_local_player = -1;
  inline uintptr_t ptr_local_player;
  
  static Player get_local_player(void) {
    return l_players[i_local_player];
  }
}

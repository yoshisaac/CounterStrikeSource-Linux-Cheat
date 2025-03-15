#include "player_info.hpp"

#include <unistd.h>
#include <cstdint>

#include "../memory.hpp"

void players(pid_t game_pid) {
  for (unsigned int i = 0; i < 64; ++i) {
    uintptr_t player;
    Memory::read(game_pid, PlayerInfo::entity_list + (i * 0x10), &player, sizeof(uintptr_t));
    if (!player) {
      PlayerInfo::l_players[i] = PlayerInfo::Player();
      continue;
    }
    uintptr_t local_player;
    Memory::read(game_pid, PlayerInfo::ptr_local_player, &local_player, sizeof(uintptr_t));

    if (player == local_player) { PlayerInfo::i_local_player = i; }
    

    int health = -1;
    Memory::read(game_pid, player + 0xC0, &health, sizeof(int));
    
    unsigned char life_state = 0;
    Memory::read(game_pid, player + 0xBF, &life_state, sizeof(unsigned char));
    
    bool dormant;
    Memory::read(game_pid, player + 0x1Ea, &dormant, sizeof(bool));
    
    float height;
    Memory::read(game_pid, player + 0x134, &height, sizeof(float));

    unsigned int flags;
    Memory::read(game_pid, player + 0x430, &flags, sizeof(flags));
    bool crouched = flags & (1<<2);
    bool on_ground = flags & (1<<0);
    
    unsigned char team_num;
    Memory::read(game_pid, player + 0xC8, &team_num, sizeof(unsigned char));

    float aim_punch[2];
    Memory::read(game_pid, player + 0x1274, &aim_punch, sizeof(float[2]));
    aim_punch[0] *= 2;
    aim_punch[1] *= 2;

    int shots_fired;
    Memory::read(game_pid, player + 0x1A2C, &shots_fired, sizeof(int));
    
    float location[3];
    Memory::read(game_pid, player + 0x2F8, &location, sizeof(float[3]));

    uintptr_t bone_matrix_ptr;
    Memory::read(game_pid, player + 0x808, &bone_matrix_ptr, sizeof(uintptr_t));
    uint32_t tmp;
    Memory::read(game_pid, player + 0x808, &tmp, sizeof(uint32_t)); //stupid hack to check for a null pointer
    float bone_matrix[200][3];
    for (int i = 0; i < 100 && bone_matrix_ptr != NULL && bone_matrix_ptr != tmp; ++i) {
      uintptr_t bone = bone_matrix_ptr + 0xC + (0x30 * i);
      Memory::read(game_pid, bone, &bone_matrix[i][0], sizeof(float));
      Memory::read(game_pid, bone + 0x10, &bone_matrix[i][1], sizeof(float));
      Memory::read(game_pid, bone + 0x20, &bone_matrix[i][2], sizeof(float));
    }
    
     
    PlayerInfo::l_players[i] = PlayerInfo::Player(i, health, team_num, life_state,
						  dormant, crouched, on_ground,
						  height, location, aim_punch,
						  bone_matrix,
						  PlayerInfo::l_players[i].dormant_alpha,
						  PlayerInfo::l_players[i].dormant_frames);
  }
}

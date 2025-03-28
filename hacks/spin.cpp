#include "spin.hpp"

#include <cmath>
#include <thread>

#include "../gui/config.hpp"

#include "../engine.hpp"

#include "../memory.hpp"

void spin(pid_t game_pid) {
  float f = 0.0;
  float ff = 0.0;      

  for (;;) {
    float plocal_angles[3];
    Memory::read(game_pid, Engine::view_angles, &plocal_angles, sizeof(float[3]));

    if (config.misc.spin_yaw) {
      plocal_angles[1] = sin(f) * 180;    
      f+=0.05;
    }
    
    if (config.misc.spin_pitch) {
      plocal_angles[0] = cos(ff) * 89;
      ff+=0.05;
    }
	
    Memory::write(game_pid, Engine::view_angles, &plocal_angles, sizeof(float[3]));

    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }
}

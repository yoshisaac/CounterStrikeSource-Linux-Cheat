struct Esp {
  bool master = true;

  bool box = true;
  int box_color[3] = {255, 0, 50};
  
  bool health = true;

  bool skeleton = false;
  int skeleton_color[3] = {255, 255, 255};
  
  bool snap_lines = false;
  int snap_lines_color[3] = {0, 255, 0};
};

struct Visuals {
  bool master = true;

  bool crosshair = true;
  int crosshair_color[3] = {255, 255, 255};  
  
  bool crosshair_recoil = false;
  int crosshair_recoil_color[3] = {255, 0, 0};
};

struct Aim {
  bool master = false;
  char key = 'c';
  bool recoil = true;
};

struct Misc {
  bool bhop = true;
  bool spin_yaw = false;
  bool spin_pitch = false;
};

struct Config {
  Esp esp;
  Visuals visuals;
  Aim aim;
  Misc misc;
};

inline Config config;

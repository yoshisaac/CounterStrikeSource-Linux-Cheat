struct Esp {
  bool master = true;
  bool box = true;
  bool health = true;
  bool skeleton = false;
  bool snap_lines = false;
};

struct Aim {
  bool master = false;
  char key = 'x';
  bool recoil = true;
};

struct Misc {
  bool bhop = true;
};

struct Config {
  Esp esp;
  Aim aim;
  Misc misc;
};

inline Config config;

# What is this?
This program is designed to give Extra Sensory Perception, and automate player movement, in the game "Counter-Strike: Source". In a nutshell, this is a hack/cheat.

# How does this?
It externally (as a seperate program from the video game) reads and writes memory using system calls from the Linux kernel. Those system calls are primarily functions like `processes_vm_readv()` and `process_vm_writev()`.

# How to compile
### Cloning Repository
Clone the repo using `git`.
```bash
$ git clone https://github.com/yoshisaac/CounterStrikeSource-Linux-Trainer.git
$ cd CounterStrikeSource-Linux-Trainer/
```

### Installing dependencies
Install the necessary packages.  
  
Debian Advanced Package Tool (APT):
```bash
$ sudo apt install libx11-dev libxext-dev libxfixes-dev libxcomposite-dev qt6-base-dev
```
  
Arch Package Manager (pacman):  
```bash
$ sudo pacman -S base-devel xorg-fonts-misc qt6-base
```
Once you install the package `xorg-fonts-misc`, you may need to reboot your computer.  
  
### Compiling
Build the program from source.  
```bash
$ make
...
```

### Running
Launching the program as the root user.  
```bash
$ sudo ./cs-source-hack
[sudo] password for user:
...
```
  
# Features
- Player ESP
  * Box
  * Skeleton
  * Health
  * Snap Lines
- Aimbot
  * Key bind
  * Automatic Recoil Control
- Misc
  * Bunny hop
  
![Screenshot of ESP](https://r2.e-z.host/bb3dfc85-7f7f-4dcb-8b0b-3a4af0aa57e4/63i42hzza5hmminxl1.png)
![Screenshot of menu](https://r2.e-z.host/bb3dfc85-7f7f-4dcb-8b0b-3a4af0aa57e4/ep27oycoce79iee2wi.png)

# TODO
- More visual features

<h1 align="center">fx</h1>
<p align="center">
<img src="https://user-images.githubusercontent.com/49120638/196036238-1cf59b2f-1172-4a03-ae02-ac2dcf02273f.png" width="400">
</p>

<p align="center">Minimalist multi tab file explorer for UNIX like systems</p>

<br>

### About fx
fx is a multi tab file explorer written in C. It doesn't have any third party dependencies and the codebase is under 1000 LOC. 
The goal is simplicity without sacrificing any key features of a modern file explorer. 

<br>

### How to install
```
git clone https://github.com/hnhx/fx
cd fx
sudo make install
```
<br>

### How to use
If you want to perform file operations (move, remove etc.) you have to select the entry / entries first. So if you want to remove `foo`, you have to move to it, and when its highlighted you have to press `space` to select it and then `r` to delete it.
The keybinds can be configured in `config.h`, that will help you understand how to navigate the program.

<br>

### Config
Edit `config.h` then just recompile the program via `make`.

<br>

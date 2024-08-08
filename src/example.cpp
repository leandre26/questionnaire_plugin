// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "tui_screen.hpp"



/*
  __  __       _       
 |  \/  | __ _(_)_ __  
 | |\/| |/ _` | | '_ \ 
 | |  | | (_| | | | | |
 |_|  |_|\__,_|_|_| |_|
                       
*/

int main() {

  TuiScreen tui_screen(18);
  tui_screen.load_settings();

  auto screen = ScreenInteractive::Fullscreen();
  tui_screen.prepare_tui(screen);
  screen.Loop(tui_screen.component);

  return 0;
}


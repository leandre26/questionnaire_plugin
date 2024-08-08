// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "tui_screen.hpp"

#if 1
// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <functional>  // for function
#include <memory>      // for shared_ptr, allocator, __shared_ptr_access
#include <string>      // for string, basic_string
#include <vector>      // for vector

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Slider, Checkbox, Vertical, Renderer, Button, Input, Menu, Radiobox, Toggle
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, operator|, Element, size, xflex, text, WIDTH, hbox, vbox, EQUAL, border, GREATER_THAN

using namespace ftxui;

// Display a component nicely with a title on the left.
Component Wrap(std::string name, Component component) {
  return Renderer(component, [name, component] {
    return hbox({
               text(name) | size(WIDTH, EQUAL, 8),
               separator(),
               component->Render() | xflex,
           }) |
           xflex;
  });
}

bool reset_field(Event e, std::string &field) {
  if (e == Event::Delete) {
    field = "";
    return true;
  }
  return false;
}

int main() {

  auto screen = ScreenInteractive::Fullscreen();
  TuiScreen tui_screen(18);
  tui_screen.load_settings();
  tui_screen.prepare_tui(screen);


  screen.Loop(tui_screen.component);

  return 0;
}

#else

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

#endif
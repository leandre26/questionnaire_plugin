
#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <functional> // for function
#include <memory>     // for shared_ptr, allocator, __shared_ptr_access
#include <string>     // for string, basic_string
#include <vector>     // for vector

#include "ftxui/component/captured_mouse.hpp" // for ftxui
#include "ftxui/component/component.hpp" // for Slider, Checkbox, Vertical, Renderer, Button, Input, Menu, Radiobox, Toggle
#include "ftxui/component/component_base.hpp" // for ComponentBase
#include "ftxui/component/screen_interactive.hpp" // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp" // for separator, operator|, Element, size, xflex, text, WIDTH, hbox, vbox, EQUAL, border, GREATER_THAN

#include <fstream> // for ofstream
#include <nlohmann/json.hpp> // for json

#define SETTINGS_FILENAME "tui_settings.json"

using namespace ftxui;
using json = nlohmann::json;

class TuiScreen {
  public:

  TuiScreen(int width = 18);
  ~TuiScreen();
  void prepare_tui(ScreenInteractive &screen);
  void prepare_tui_simple(ScreenInteractive &screen);
  
  void load_settings(std::string filename = SETTINGS_FILENAME);
  void save_settings();

  bool has_data() { 
    if (_has_new_output) {
      _has_new_output = false;
      return true;
    } else {
      return false;
    }
  }

  void update_settings();
  json get_data();

  private:
  bool reset_field(Event e, std::string &field);
  Component Wrap(std::string name, Component component);

  public:
  Component component;
  bool exit_requested = false;

  private:
  bool _has_new_output = false;
  std::vector<std::string> _toggle_labels = {"Enabled", "Disabled"};
  json _settings = json::parse("{}");
  json _data;
  std::string _settings_filename = SETTINGS_FILENAME;
  int _header_width = 18;
  std::string _machine_operator = "";
  std::string _mads_user = "";
  std::string _operation = "";
  std::string _trial_number = "1";
  std::string _comment = "";
  bool _test_selected;
  bool _cn1_selected;
  bool _cn2_selected;
  bool _cn3_selected;
  int _toggle_selected;
  int _logging_status = 0;
  
};


#endif // SCREEN_HPP
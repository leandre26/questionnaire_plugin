#ifndef QUESTIONNAIRE_SCREEN_HPP
#define QUESTIONNAIRE_SCREEN_HPP

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

#define QUESTIONNAIRE_SETTINGS_FILENAME "questionnaire_settings.json"

using namespace ftxui;
using json = nlohmann::json;

class QuestionnaireScreen {
public:
  QuestionnaireScreen(int width = 18);
  ~QuestionnaireScreen();
  
  void prepare_questionnaire(ScreenInteractive &screen);
  void load_settings(std::string filename = QUESTIONNAIRE_SETTINGS_FILENAME);
  void save_settings();

  bool has_data() { 
    if (_has_new_output) {
      _has_new_output = false;
      return true;
    }
    return false;
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
  int _header_width = 18;

  std::string _nom = "";
  std::string _date = "";
  int _pression = 0;
  std::string _pression_str = "0"; 
  int _machine_index = 0;
  std::vector<std::string> _machines = {"VX1", "VX2", "VX3", "VX4"};

  json _settings = json::parse("{}");
  json _data;
  std::string _settings_filename = QUESTIONNAIRE_SETTINGS_FILENAME;
};

#endif // QUESTIONNAIRE_SCREEN_HPP

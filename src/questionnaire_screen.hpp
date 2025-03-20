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

  // Questionnaire fields
  std::string _nom = "";
  std::string _groupe = "";
  std::string _date = "";
  
  int _zone_index = 0;
  std::vector<std::string> _zones = {"Initiation S1/S2", "Perfectionnement S3/S4/S5"};
  
  int _machine_index = 0;
  std::vector<std::string> _machines = {"VX1", "VX2", "VX3", "VX4"};

  std::string _pression_hydrolique_str = "0";
  std::string _pression_broche_air_str = "0";
  
  int _huile_broche_index = 0;
  int _huile_glissieres_index = 0;
  int _lubrifiant_index = 0;
  int _bac_copeaux_index = 0;
  int _proprete_usinage_index = 0;
  int _proprete_machine_index = 0;
  
  std::string _temperature_bloc_froid_str = "0";
  std::string _observation = "";

  std::vector<std::string> _niveau_options = {"Haut", "Moyen", "Bas"};
  std::vector<std::string> _proprete_options = {"Propre", "Moyen", "Non nettoyé"};

  json _settings = json::parse("{}");
  json _data;
  std::string _settings_filename = QUESTIONNAIRE_SETTINGS_FILENAME;
};

#endif // QUESTIONNAIRE_SCREEN_HPP

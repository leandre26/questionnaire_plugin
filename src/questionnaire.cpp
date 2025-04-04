/*
  ____                                   _             _       
 / ___|  ___  _   _ _ __ ___ ___   _ __ | |_   _  __ _(_)_ __  
 \___ \ / _ \| | | | '__/ __/ _ \ | '_ \| | | | |/ _` | | '_ \ 
  ___) | (_) | |_| | | | (_|  __/ | |_) | | |_| | (_| | | | | |
 |____/ \___/ \__,_|_|  \___\___| | .__/|_|\__,_|\__, |_|_| |_|
                                  |_|            |___/         
# A Template for TuiPlugin, a Source Plugin
# Generated by the command: plugin -t source -d tui_plugin TUI
# Hostname: Fram-IV.local
# Current working directory: /Users/p4010/Develop/MADS_plugins
# Creation date: 2024-08-07T13:56:04.950+0200
# NOTICE: MADS Version 1.0.2
*/
// Mandatory included headers
#include <source.hpp>
#include <nlohmann/json.hpp>
#include <pugg/Kernel.h>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include "ftxui/component/loop.hpp"
#include <thread>
#include "questionnaire_screen.hpp"

#ifndef PLUGIN_NAME
#define PLUGIN_NAME "questionnaire"
#endif

using namespace std;
using json = nlohmann::json;
using namespace chrono_literals;
using namespace ftxui;

class QuestionnairePlugin : public Source<json> {
public:
  string kind() override { return PLUGIN_NAME; }

  return_type get_output(json &out, std::vector<unsigned char> *blob = nullptr) override {
    out.clear();
    if (!_agent_id.empty()) out["agent_id"] = _agent_id;
    _loop->RunOnce();
    if (_loop->HasQuitted()) {
      out["exit_requested"] = true;
      return return_type::critical;
    }
    if (_questionnaire_screen.has_data()) {
      out = _questionnaire_screen.get_data();
      return return_type::success;
    }
    return return_type::retry;
  }

  void set_params(void const *params) override {
    Source::set_params(params);
    _params.merge_patch(*(json *)params);
    _questionnaire_screen.load_settings();
    _questionnaire_screen.prepare_questionnaire(_screen);
    _loop = unique_ptr<Loop>(new Loop(&_screen, _questionnaire_screen.component));
  }

  map<string, string> info() override {
    return {};
  };

private:
  QuestionnaireScreen _questionnaire_screen{18};
  ScreenInteractive _screen = ScreenInteractive::Fullscreen();
  unique_ptr<Loop> _loop;
};

/*
  ____  _             _             _      _
 |  _ \| |_   _  __ _(_)_ __     __| |_ __(_)_   _____ _ __
 | |_) | | | | |/ _` | | '_ \   / _` | '__| \ \ / / _ \ '__|
 |  __/| | |_| | (_| | | | | | | (_| | |  | |\ V /  __/ |
 |_|   |_|\__,_|\__, |_|_| |_|  \__,_|_|  |_| \_/ \___|_|
                |___/
Enable the class as plugin
*/
INSTALL_SOURCE_DRIVER(QuestionnairePlugin, json)


/*
                  _
  _ __ ___   __ _(_)_ __
 | '_ ` _ \ / _` | | '_ \
 | | | | | | (_| | | | | |
 |_| |_| |_|\__,_|_|_| |_|

For testing purposes, when directly executing the plugin
*/

int main(int argc, char const *argv[]) {
  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  QuestionnaireScreen questionnaire_screen(18);
  questionnaire_screen.prepare_questionnaire(screen);

  screen.Loop(questionnaire_screen.component);

  json output = questionnaire_screen.get_data();
  cout << "Output: " << output.dump(2) << endl;

  return 0;
}


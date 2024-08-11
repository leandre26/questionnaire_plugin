#include "tui_screen.hpp"
#include <iostream>

TuiScreen::TuiScreen(int width) : _header_width(width) {}

TuiScreen::~TuiScreen() { save_settings(); }

bool TuiScreen::reset_field(Event e, std::string &field) {
  if (e == Event::F10 || e == Event::Delete) {
    field = "";
    return true;
  }
  return false;
}

Component TuiScreen::Wrap(std::string name, Component component) {
  return Renderer(component, [=] {
    return hbox({
               text(name) | size(WIDTH, EQUAL, _header_width),
               separator(),
               component->Render() | xflex,
           }) |
           xflex;
  });
}

void TuiScreen::prepare_tui(ScreenInteractive &screen) {
  // clang-format off
  // -- Input ------------------------------------------------------------------
  auto input_operator = Input(&_machine_operator, "Operator name");
  input_operator |=
      CatchEvent([&](Event e) { return reset_field(e, _machine_operator); });

  auto input_user = Input(&_mads_user, "MADS user");
  input_user |= CatchEvent([&](Event e) { return reset_field(e, _mads_user); });

  auto input_operation = Input(&_operation, "Operation description");
  input_operation |=
      CatchEvent([&](Event e) { return reset_field(e, _operation); });

  auto input_trial_num = Input(&_trial_number, "1");
  input_trial_num |= CatchEvent([&](Event e) { 
    return e.is_character() && !isdigit(e.character()[0]); 
  });
  input_trial_num |= CatchEvent([&](Event e) {
    if (e == Event::Character('+')) {
      uint32_t n = atoi(_trial_number.c_str());
      _trial_number = std::to_string(n + 1);
    } else if (e == Event::Character('-')) {
      uint32_t n = atoi(_trial_number.c_str());
      _trial_number = std::to_string(n - 1);
    } else
      return false;
    return true;
  });
  input_operator = Wrap("Machine operator", input_operator);
  input_user = Wrap("MADS user", input_user);
  input_operation = Wrap("Operation", input_operation);
  input_trial_num = Wrap("Trial number", input_trial_num);

  // -- Comment ---------------------------------------------------------------
  auto input_comment = Input(&_comment, "No comment");
  input_comment = Wrap("Comment", input_comment) | yflex_grow;
  input_comment |= CatchEvent([&](Event e) { 
    return reset_field(e, _comment);
  });

  // -- Toggle------------------------------------------------------------------
  _toggle_selected = 0;

  auto logging = Toggle(&_toggle_labels, &_toggle_selected);
  auto b_logging = Button("Logging", [&] {
    if (_toggle_selected == 0){
      _data["event"] = "logging ON";
      _data["pause"] = false;
    } else {
      _data["event"] = "logging Off";
      _data["pause"] = true;
    } 
    _has_new_output = true;
  });

  // -- Checkbox ---------------------------------------------------------------
  _test_selected = _settings.value("test_selected", false);
  _cn1_selected  = _settings.value("cn1_selected", false);
  _cn2_selected  = _settings.value("cn2_selected", false);
  _cn3_selected  = _settings.value("cn3_selected", false);

  auto checkboxes = Container::Vertical({
    Checkbox("This is a test", &_test_selected) | color(Color::Green),
    Checkbox("Custom tag 1", &_cn1_selected),
    Checkbox("Custom tag 2", &_cn2_selected),
    Checkbox("Custom tag 3", &_cn3_selected),
  });
  checkboxes = Wrap("Tags", checkboxes);

  // -- Button -----------------------------------------------------------------
  auto b_quit =
      Button("Quit", screen.ExitLoopClosure()) | color(Color::Orange1) | center;

  auto b_mkin = Button("Mark In", [&] { 
    _running = true;
    _data["event"] = "marker in";
    _has_new_output = true;
  }) | color(Color::Blue1) | flex;
  b_mkin = Maybe(b_mkin, [&] { return !_running; });

  auto b_mk = Button("Mark", [&] {
    _has_new_output = true;
    _data["event"] = "marker";
  }) | color(Color::Green) | flex;

  auto b_mkout = Button("Mark out", [&] {
    _running = false;
    _data["event"] = "marker out";
    uint32_t n = atoi(_trial_number.c_str());
    _trial_number = std::to_string(n + 1);
    _has_new_output = true;
  }) | color(Color::Yellow) | flex;
  b_mkout = Maybe(b_mkout, [&] { return _running; });

  auto b_stop = Button("Stop", [&] {
    _data["event"] = "stop";
    _has_new_output = true;
  }) | color(Color::Red) | flex;

  auto b_restart = Button("Restart", [&] {
    _data["event"] = "restart";
    _has_new_output = true;
  }) | color(Color::Purple) | flex;

  // -- Layout -----------------------------------------------------------------
  auto layout = Container::Vertical({
    input_operator,
    input_user,
    input_operation,
    input_trial_num,
    input_comment,
    logging,
    b_logging,
    checkboxes,
    b_mkin,
    b_mk, b_mkout,
    b_stop,
    b_restart,
    b_quit
  });

  component = Renderer(layout, [=] {
    return vbox({
      input_operator->Render(),
      input_user->Render(),
      input_operation->Render(),
      input_trial_num->Render(),
      separator(),
      input_comment->Render(),
      separator(),
      hbox({
        text("Logging") | size(WIDTH, EQUAL, _header_width),
        separator(), 
        logging->Render() | vcenter, separator(),
        b_logging->Render()
      }),
      separator(),
      checkboxes->Render(),
      separator(),
      hbox({
        b_mkin->Render(), 
        b_mk->Render(), 
        b_mkout->Render(),
        b_stop->Render(), 
        b_restart->Render()
      }),
      separator(),
      b_quit->Render(),
    }) |xflex | size(WIDTH, GREATER_THAN, 100) | border;
  });
  // clang-format on
}


void TuiScreen::load_settings(std::string filename) {
  _settings_filename = filename;
  std::ifstream file(filename);
  if (file.is_open()) {
    file >> _settings;
    file.close();
  }
  _machine_operator = _settings.value("operator", "");
  _mads_user = _settings.value("user", "");
  _operation = _settings.value("operation", "");
  _trial_number = _settings.value("trial_number", "1");
  _comment = _settings.value("comment", "");
}

void TuiScreen::save_settings() {
  get_data();
  std::ofstream file(_settings_filename);
  if (file.is_open()) {
    file << _settings.dump(2);
    file.close();
  }
}

void TuiScreen::update_settings() {
  _settings = {
      {"operator", _machine_operator}, {"user", _mads_user},
      {"operation", _operation},       {"trial_number", _trial_number},
      {"comment", _comment},           {"test_selected", _test_selected},
      {"cn1_selected", _cn1_selected}, {"cn2_selected", _cn2_selected},
      {"cn3_selected", _cn3_selected}};
}

json TuiScreen::get_data() {
  update_settings();
  _data["info"] = _settings;
  return _data;
}
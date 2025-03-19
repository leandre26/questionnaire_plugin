#include "questionnaire_screen.hpp"
#include <iostream>

QuestionnaireScreen::QuestionnaireScreen(int width) : _header_width(width) {}

QuestionnaireScreen::~QuestionnaireScreen() { save_settings(); }

bool QuestionnaireScreen::reset_field(Event e, std::string &field) {
  if (e == Event::F10 || e == Event::Delete) {
    field = "";
    return true;
  }
  return false;
}

Component QuestionnaireScreen::Wrap(std::string name, Component component) {
  return Renderer(component, [=] {
    return hbox({
               text(name) | size(WIDTH, EQUAL, _header_width),
               separator(),
               component->Render() | xflex,
           }) |
           xflex;
  });
}

void QuestionnaireScreen::prepare_questionnaire(ScreenInteractive &screen) {
  auto input_name = Input(&_name, "Nom");
  input_name |= CatchEvent([&](Event e) { return reset_field(e, _name); });
  input_name = Wrap("Nom", input_name);

  auto input_date = Input(&_date, "JJ/MM/AAAA");
  input_date |= CatchEvent([&](Event e) { return reset_field(e, _date); });
  input_date = Wrap("Date", input_date);

  auto input_pressure = Input(&_pressure, "Pression (bar)");
  input_pressure |= CatchEvent([&](Event e) {
    return e.is_character() && !isdigit(e.character()[0]);
  });
  input_pressure = Wrap("Pression", input_pressure);

  _machine_index = 0;
  auto machine_select = Radiobox(&_machines, &_machine_index);
  machine_select = Wrap("Machine", machine_select);

  auto b_validate = Button("Valider", screen.ExitLoopClosure());

  auto layout = Container::Vertical({
    input_name,
    input_date,
    machine_select,
    input_pressure,
    separator(),
    b_validate
  });

  component = Renderer(layout, [=] {
    return vbox({
      input_name->Render(),
      input_date->Render(),
      machine_select->Render(),
      input_pressure->Render(),
      separator(),
      b_validate->Render()
    }) | xflex | border;
  });
}

void QuestionnaireScreen::load_settings(std::string filename) {
  _settings_filename = filename;
  std::ifstream file(filename);
  if (file.is_open()) {
    file >> _settings;
    file.close();
  }
  _name = _settings.value("name", "");
  _date = _settings.value("date", "");
  _pressure = _settings.value("pressure", "0");
}

void QuestionnaireScreen::save_settings() {
  update_settings();
  std::ofstream file(_settings_filename);
  if (file.is_open()) {
    file << _settings.dump(2);
    file.close();
  }
}

void QuestionnaireScreen::update_settings() {
  _settings = {
      {"name", _name},
      {"date", _date},
      {"pressure", _pressure},
      {"machine_index", _machine_index}
  };
}

json QuestionnaireScreen::get_data() {
  update_settings();
  _data["info"] = _settings;
  return _data;
}
  _data["info"] = _settings;
  return _data;
}

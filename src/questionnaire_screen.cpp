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
  auto input_nom = Input(&_nom, "Nom"); // Changed _name -> _nom
  input_nom = CatchEvent(input_nom, [&](Event e) { return reset_field(e, _nom); });
  input_nom = Wrap("Nom", input_nom);

  auto input_date = Input(&_date, "JJ/MM/AAAA");
  input_date |= CatchEvent([&](Event e) { return reset_field(e, _date); });
  input_date = Wrap("Date", input_date);

  auto input_pression = Input(&_pression, "Pression (bar)"); // Changed _pressure -> _pression
  input_pression |= CatchEvent([&](Event e) {
    return e.is_character() && !isdigit(e.character()[0]);
  });
  input_pression = Wrap("Pression", input_pression);

  _machine_index = 0;
  auto machine_select = Radiobox(&_machines, &_machine_index);
  machine_select = Wrap("Machine", machine_select);

  auto b_validate = Button("Valider", screen.ExitLoopClosure());

  auto layout = Container::Vertical({
    input_nom,
    input_date,
    machine_select,
    input_pression,
    separator(),
    b_validate
  });

  component = Renderer(layout, [=] {
    return vbox({
      input_nom->Render(),
      input_date->Render(),
      machine_select->Render(),
      input_pression->Render(),
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
  _nom = _settings.value("nom", "");
  _date = _settings.value("date", "");
  _pression = _settings.value("pression", 0);
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
      {"nom", _nom},
      {"date", _date},
      {"pression", _pression},
      {"machine_index", _machine_index}
  };
}

json QuestionnaireScreen::get_data() {
  update_settings();
  _data["info"] = _settings;
  return _data;
}

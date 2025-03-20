#include "questionnaire_screen.hpp"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

QuestionnaireScreen::QuestionnaireScreen(int width) : _header_width(width), _pression_hydrolique(0.0), _pression_broche_air(0.0), _temperature_bloc_froid(0.0) {}

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
  auto input_nom = Input(&_nom, "Nom");
  input_nom = CatchEvent(input_nom, [&](Event e) { return reset_field(e, _nom); });
  input_nom = Wrap("Nom", input_nom);

  auto input_groupe = Input(&_groupe, "Groupe");
  input_groupe = CatchEvent(input_groupe, [&](Event e) { return reset_field(e, _groupe); });
  input_groupe = Wrap("Groupe", input_groupe);

  auto input_date = Input(&_date, "JJ/MM/AAAA");
  input_date = CatchEvent(input_date, [&](Event e) { return reset_field(e, _date); });
  input_date = Wrap("Date", input_date);

  auto zone_select = Radiobox(&_zones, &_zone_index);
  zone_select = Wrap("Zones CN", zone_select);

  _machine_index = 0;
  auto machine_select = Radiobox(&_machines, &_machine_index);
  machine_select = Wrap("Machine", machine_select);

  _pression_hydrolique_str = std::to_string(_pression_hydrolique);
  auto input_pression_hydrolique = Input(&_pression_hydrolique_str, "Pression Hydrolique (bar)");
  input_pression_hydrolique = CatchEvent(input_pression_hydrolique, [&](Event e) {
    return e.is_character() && !isdigit(e.character()[0]) && e.character()[0] != '.';
  });
  input_pression_hydrolique = Wrap("Pression Hydrolique", input_pression_hydrolique);

  _pression_broche_air_str = std::to_string(_pression_broche_air);
  auto input_pression_broche_air = Input(&_pression_broche_air_str, "Pression Broche Air (bar)");
  input_pression_broche_air = CatchEvent(input_pression_broche_air, [&](Event e) {
    return e.is_character() && !isdigit(e.character()[0]) && e.character()[0] != '.';
  });
  input_pression_broche_air = Wrap("Pression Broche Air", input_pression_broche_air);

  _temperature_bloc_froid_str = std::to_string(_temperature_bloc_froid);
  auto input_temperature_bloc_froid = Input(&_temperature_bloc_froid_str, "Température Bloc Froid (°C)");
  input_temperature_bloc_froid = CatchEvent(input_temperature_bloc_froid, [&](Event e) {
    return e.is_character() && !isdigit(e.character()[0]) && e.character()[0] != '.';
  });
  input_temperature_bloc_froid = Wrap("Température Bloc Froid", input_temperature_bloc_froid);

  auto huile_broche_select = Radiobox(&_niveau_options, &_huile_broche_index);
  huile_broche_select = Wrap("Niveau Huile Broche", huile_broche_select);

  auto huile_glissieres_select = Radiobox(&_niveau_options, &_huile_glissieres_index);
  huile_glissieres_select = Wrap("Niveau Huile Glissieres", huile_glissieres_select);

  auto lubrifiant_select = Radiobox(&_niveau_options, &_lubrifiant_index);
  lubrifiant_select = Wrap("Niveau Lubrifiant", lubrifiant_select);

  auto bac_copeaux_select = Radiobox(&_niveau_options, &_bac_copeaux_index);
  bac_copeaux_select = Wrap("Niveau Bac Copeaux", bac_copeaux_select);

  auto proprete_usinage_select = Radiobox(&_proprete_options, &_proprete_usinage_index);
  proprete_usinage_select = Wrap("Propreté Zone Usinage", proprete_usinage_select);

  auto proprete_machine_select = Radiobox(&_proprete_options, &_proprete_machine_index);
  proprete_machine_select = Wrap("Propreté Machine", proprete_machine_select);

  auto input_observation = Input(&_observation, "Observation (optionnel)");
  input_observation = Wrap("Observation", input_observation);

  auto b_validate = Button("Valider", screen.ExitLoopClosure());

  auto layout = Container::Vertical({
    input_nom,
    input_groupe,
    input_date,
    zone_select,
    machine_select,
    input_pression_hydrolique,
    input_pression_broche_air,
    input_temperature_bloc_froid,
    huile_broche_select,
    huile_glissieres_select,
    lubrifiant_select,
    bac_copeaux_select,
    proprete_usinage_select,
    proprete_machine_select,
    input_observation,
    b_validate
  });

  component = Renderer(layout, [=] {
    return vbox({
      input_nom->Render(),
      input_groupe->Render(),
      input_date->Render(),
      zone_select->Render(),
      machine_select->Render(),
      input_pression_hydrolique->Render(),
      input_pression_broche_air->Render(),
      input_temperature_bloc_froid->Render(),
      huile_broche_select->Render(),
      huile_glissieres_select->Render(),
      lubrifiant_select->Render(),
      bac_copeaux_select->Render(),
      proprete_usinage_select->Render(),
      proprete_machine_select->Render(),
      input_observation->Render(),
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
  _groupe = _settings.value("groupe","");
  _date = _settings.value("date", "");
  _pression_hydrolique_str = _settings.value("pression hydrolique", "0");
  _pression_broche_air_str = _settings.value("pression broche air", "0");
  _temperature_bloc_froid_str = _settings.value("temperature bloc froid", "0");
  _observation = _settings.value("observation", "");
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
      {"groupe", _groupe},
      {"date", _date},
      {"zone_index", _zone_index},
      {"machine_index", _machine_index},
      {"pression_hydrolique", _pression_hydrolique_str},
      {"pression_broche_air", _pression_broche_air_str},
      {"temperature_bloc_froid", _temperature_bloc_froid_str},
      {"huile_broche_index", _huile_broche_index},
      {"huile_glissieres_index", _huile_glissieres_index},
      {"lubrifiant_index", _lubrifiant_index},
      {"bac_copeaux_index", _bac_copeaux_index},
      {"proprete_usinage_index", _proprete_usinage_index},
      {"proprete_machine_index", _proprete_machine_index},
      {"observation", _observation}
  };
}

json QuestionnaireScreen::get_data() {
  update_settings();
  _data["info"] = _settings;
  return _data;
}

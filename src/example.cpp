#include "questionnaire_screen.hpp"

#include <functional>  
#include <memory>      
#include <string>      
#include <vector>      

#include "ftxui/component/captured_mouse.hpp"  
#include "ftxui/component/component.hpp"  
#include "ftxui/component/component_base.hpp"  
#include "ftxui/component/screen_interactive.hpp"  
#include "ftxui/dom/elements.hpp"  

using namespace ftxui;

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
  QuestionnaireScreen questionnaire_screen(18);
  questionnaire_screen.load_settings();
  questionnaire_screen.prepare_questionnaire(screen);

  screen.Loop(questionnaire_screen.component);

  return 0;
}

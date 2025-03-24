#include <crow.h>
#include "src/questionnaire_screen.hpp"

int main() {
    crow::SimpleApp app;

    // API route to fetch questionnaire data
    CROW_ROUTE(app, "/questionnaire")([]() {
        QuestionnaireScreen questionnaire(30);
        return questionnaire.get_data().dump();  // Return JSON
    });

    app.port(8080).multithreaded().run();
}

#include <crow.h>
#include "questionnaire_screen.hpp"

int main() {
    crow::SimpleApp app;

     // Route to serve the questionnaire form
    CROW_ROUTE(app, "/questionnaire")
    ([]() {
        return R"(
            <!DOCTYPE html>
            <html>
            <head>
                <title>Questionnaire</title>
                <meta name="viewport" content="width=device-width, initial-scale=1">
                <style>
                    body { font-family: Arial, sans-serif; text-align: center; }
                    form { display: inline-block; text-align: left; margin-top: 20px; }
                    input, textarea { display: block; width: 100%; margin-bottom: 10px; padding: 8px; }
                    button { padding: 10px 20px; background-color: blue; color: white; border: none; cursor: pointer; }
                </style>
            </head>
            <body>
                <h2>Fill out this questionnaire</h2>
                <form action="/submit" method="POST">
                    <label for="name">Name:</label>
                    <input type="text" id="name" name="name" required>
                    
                    <label for="feedback">Feedback:</label>
                    <textarea id="feedback" name="feedback" required></textarea>

                    <button type="submit">Submit</button>
                </form>
            </body>
            </html>
        )";
    });

    // Route to handle form submission
    CROW_ROUTE(app, "/submit").methods("POST"_method)
    ([](const crow::request& req) {
        auto params = crow::query_string(req.body);
        std::string name = params.get("name") ? params.get("name") : "Anonymous";
        std::string feedback = params.get("feedback") ? params.get("feedback") : "No feedback given";

        CROW_LOG_INFO << "New response: " << name << " - " << feedback;
        return "Thank you, " + name + "! Your response has been recorded.";
    });

    app.bindaddr("0.0.0.0").port(5000).multithreaded().run();
}

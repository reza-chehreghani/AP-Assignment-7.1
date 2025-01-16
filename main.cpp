#include "config.hpp"
#include "futballFantasy.hpp"

int main() {
    FutballFantasy futball_fantasy;

    string input;
    while (getline(cin, input))
        try {
            istringstream iss(input);
            cout << futball_fantasy.do_order(iss).str();
        } catch(BadRequest& ex) {
            cout << "Bad Request\n";
        } catch(NotFound& ex) {
            cout << "Not Found\n";
        } catch (PermissionDenied& ex) {
            cout << "Permission Denied\n";
        }

    return 0;
}

BadRequest::BadRequest(const char *ex) : invalid_argument(ex) {}

NotFound::NotFound(const char *ex) : invalid_argument(ex) {}

PermissionDenied::PermissionDenied(const char *ex) : invalid_argument(ex) {}
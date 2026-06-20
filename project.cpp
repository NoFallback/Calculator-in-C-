#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio> //for input output files//
#include <fstream>
#include <iostream>
#include <map>
#include <numeric> // REQUIRED for accumulate
#include <queue>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef pair<int, int> pi;
typedef vector<ll> vl;

ll const MOD = 1e9 + 7;

bool is_long_long(string &str, long long &value) {
  try {
    size_t processed_chars = 0;
    value = stoll(str, &processed_chars);
    return str.length() == processed_chars;
  } catch (invalid_argument &e) {
    return false;
  } catch (out_of_range &e) {
    return false;
  }
}

bool is_int(string &str, int &value) {
  try {
    size_t processed_chars = 0;
    value = stoll(str, &processed_chars);
    return str.length() == processed_chars;
  } catch (invalid_argument &e) {
    return false;
  } catch (out_of_range &e) {
    return false;
  }
}

ll oper(ll &num1, ll &num2, string &op) { // operation func
  if (op == "+") {
    return num1 + num2;
  } else if (op == "-") {
    return num1 - num2;
  } else if (op == "*") {
    return num1 * num2;
  } else { // division
    return num1 / num2;
  }
}

bool parsing_for_digits(
    string str,
    double &num) { // str is a number in string data type   // tells if string
                   // is correctly formated for a double type and gives the
                   // converted string output
  try {
    num = stod(str);
    return true;
  } catch (exception &) {
    return false;
  }
}

int main() {
  // freopen("input.txt", "r", stdin);
  // freopen("output.txt", "w", stdout);
  ios::sync_with_stdio(0);
  cin.tie(0); // to speed up input/output

  // write your code below
  string dev_pass = "qwerty";
  printf("%50s\n", "WELCOME USER");
  cout << string(90, '=') << endl << endl;
  cout << "Hello User I am your calculator assistant 'Coc' \nAnd I will help "
          "you "
          "understand how this calculator works"
       << endl;
  cout << "First things first :-" << endl;
  cout << endl << "To activate calculator enter 'a'" << endl;
  cout << "To deactivate calculator and switch to suggestioin mode enter 's'"
       << endl;
  cout << "To deactivate calculator and quit enter 'q'" << endl;
  cout << "To activate developer mode enter 'd'\n" << endl;

  string user_input;
  while (true) {
    cout << "\nTO REPEAT THE INSTRUCTIONS enter 'r'\n\n";
    cout << ">> ";
    cin >> user_input;
    cout << "##################################################################"
            "\n";

    if (user_input == "q") { // quit function
      return 0;
    } else if (user_input == "r") {
      cout << endl << "REPEATING THE INSTRUCTIONS :" << endl << endl;
      ;
      cout << endl << "To activate calculator enter 'a'" << endl;
      cout
          << "To deactivate calculator and switch to suggestioin mode enter 's'"
          << endl;
      cout << "To deactivate calculator and quit enter 'q'" << endl;
      cout << "To activate developer mode enter 'd'\n" << endl;
    } else if (user_input == "d") { // developer mode
      cout << "\nDeveloper password = ";
      string pass_check;
      cin >> pass_check;
      if (pass_check == dev_pass) {
        cout << "Congratulations You have Entered development mode !!!\n";
      } else {
        cout << "OOPS !!!\nWrong password, Try again later\n\n";
      }
    } else if (user_input == "s") { // suggestion box
      cout << endl << "Give your suggestions below : " << endl;
      ofstream write_sugg("suggestion.txt");

      string user_sugg;
      cin.ignore(); /// Important to clear the input buffer
      while (getline(cin, user_sugg) && !user_sugg.empty()) {
        write_sugg << user_sugg << endl;
      }
      write_sugg.close();
      cout << endl << "Yeah, We have saved your responses\n\n";
    } else if (user_input == "a") { // activate calculator
      cout
          << endl
          << "-----------------------------CALCULATOR "
             "-----------------------------"
          << endl
          << "You have successfully entered calculation mode\nRemember for "
             "any help u can use GUIDE MENU by entering 'gm' on prompt space\n";
      cout << "Enter 'quit' to exit from calculator workspace" << endl;
      string calc_in;
      do {
        cout << endl << ">> ";

        getline(cin, calc_in);
        if (calc_in == "gm") {
          cout << "\nGUIDE MENU\n";
          ifstream gm("Guide.txt");
          string line;
          while (getline(gm, line) && !line.empty()) {
            cout << line << endl;
          }
          continue; ////test///////test///////test///////test///////test///////test///////test///
        }
        if (calc_in == "quit") {
          break;
        }
        ////////////////////////////////// WORK on CALC /////////////////////
        stringstream lin(calc_in);
        // cin.ignore();
        string str_num1, str_num2;
        ll num1, num2;
        string op;
        string extra;
        if (lin >> str_num1 >> op >> str_num2) {

          if (lin >> extra) {
            cout << "\nInvalid Syntax : Check Guide Menu for more info\n";

          } else if (is_long_long(str_num1, num1) &&
                     is_long_long(str_num2, num2)) {
            if (op == "+" || op == "-" || op == "/" ||
                op == "*") { // op is correct       //// do calc
              cout << "\nRESULT : " << oper(num1, num2, op) << endl;
            } else {
              cout << "\nInvalid Syntax : Check Guide Menu for more info\n";
            }
          } else {
            cout << "\nInvalid Syntax : Check Guide Menu for more info\n";
          }
        }

        /////////////////////////////////////////////////////////////////////
      } while (calc_in != "quit");
      cout << "\n--------------------------------------------------------------"
              "------";
      cout << "\nSuccessfully quit Calculator workspace\n";

    } else { // Invalid key
      cout << "\nINVALID RESPONSE \nTRY AGAIN\n\n";
    }
    continue;

    return 0;
  }
}
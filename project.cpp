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

double oper(double &num1, double &num2, char &op) { // operation func
  if (op == '+') {
    return num1 + num2;
  } else if (op == '-') {
    return num1 - num2;
  } else if (op == '*') {
    return num1 * num2;
  } else { // division
    return num1 / num2;
  }
}

map<char, int> priority = {
    {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}}; // priority orders for operators

vector<char> operator_list = {'+', '-', '*', '/'};

vector<char> valid_char = {
    '.', '0', '1', '2',
    '3', '4', '5', '6', // valid characters in user calc mode input line
    '7', '8', '9', ' ',
    '+', '-', '*', '/'};

bool is_valid_pair(char ch1,
                   char ch2) { // check and tell if pair is valid or not
  // num num
  if (isdigit(ch1) && isdigit(ch2)) {
    return true;
  }
  // num . OR . num
  if ((isdigit(ch1) && ch2 == '.') || (isdigit(ch2) && ch1 == '.')) {
    return true;
  }
  // _ num OR num _
  if ((isdigit(ch1) && ch2 == ' ') || (isdigit(ch2) && ch1 == ' ')) {
    return true;
  }
  // _ _
  if (ch1 == ' ' || ch2 == ' ') {
    return true;
  }
  // Checking if ch is op ???
  auto it1 = find(operator_list.begin(), operator_list.end(), ch1);
  auto it2 = find(operator_list.begin(), operator_list.end(), ch2);
  // op _ OR _ op
  if ((it1 != operator_list.end() && ch2 == ' ') ||
      (it2 != operator_list.end() && ch1 == ' ')) {
    return true;
  }
  // num op OR op num
  if ((it1 != operator_list.end() && isdigit(ch2)) ||
      (it2 != operator_list.end() && isdigit(ch1))) {
    return true;
  }
  return false;
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

bool comrad(
    stack<double> &num_stk, stack<char> &op_stk,
    double &out) { // COMrad (used to take 2 num and 1 op to operate
                   // them from stacks) [finds the value but not adds to stack]
  if (num_stk.size() == 0 && op_stk.size() == 0) {
    return true;
  }
  if ((num_stk.size() == 0 && op_stk.size() != 0) ||
      (num_stk.size() != 0 && op_stk.size() == 0)) {
    return false;
  }
  char work_op = op_stk.top();
  op_stk.pop();
  double num2 = num_stk.top();
  num_stk.pop();
  if (num_stk.size() == 0) {
    return false;
  }
  double num1 = num_stk.top();
  num_stk.pop();
  out = oper(num1, num2, work_op);
  return true;
}

bool mid_work(stack<double> &num_stk, stack<char> &op_stk,
              char new_op) { // calculates and resolves when priority order in
                             // stack diffs on new_op adding
  while (op_stk.size() != 0 && priority[op_stk.top()] != 1) {
    double out;
    bool right = comrad(num_stk, op_stk, out);
    if (right == false) {
      return false;
    }
    num_stk.push(out);
  }
  op_stk.push(new_op);
  return true;
}

bool total_work(
    stack<double> &num_stk,
    stack<char> &op_stk) { // calculates the final ouput of the whole calc_input
  if (num_stk.size() == 0) {
    return false;
  }
  while (op_stk.size() != 0) {
    double out;
    bool right = comrad(num_stk, op_stk, out);
    if (right == false) {
      return false;
    }
    num_stk.push(out);
  }
  return true;
}

bool enum_input(string &line, double &ans) {
  if (line.empty()) {
    return false;
  }
  stack<double> num_stk;
  stack<char> op_stk;
  string str_num_maker;
  char prev_ch = line[0];
  for (char ch : line) {
    // CHECK IF CH IS VALID OR NOT
    auto it = find(valid_char.begin(), valid_char.end(),
                   ch); // maybe <algorithm> needed
    if (it == valid_char.end()) {
      return false;
    }
    // Checking if a valid pair
    // <<< moved if block from inside of whitespace
    // block 1st lines>>>
    bool right = is_valid_pair(prev_ch, ch);
    if (right == false) {
      return false;
    }
    // IF IT IS WHITESPACE
    if (ch == ' ') {

      if (isdigit(prev_ch)) {
        bool right;
        double created_num_from_string;
        right = parsing_for_digits(str_num_maker, created_num_from_string);
        if (right == false) {
          return false;
        }
        num_stk.push(created_num_from_string);
        str_num_maker = "";
        prev_ch = ch;
      }
      continue;
    }
    // IF IT IS NUMBER (THEN ADD TO STRING AND SEND TO PARSING_DIGIT FUNC AND
    // THEN TO STACK)
    // num num // . num // _ num // op num
    if (isdigit(ch)) {
      str_num_maker += ch;
      prev_ch = ch;
      continue;
    }

    // IF . CASE
    // num . //
    if (ch == '.') {
      str_num_maker += ch;
      prev_ch = ch;
      continue;
    }
    //   ITS PRIORITY
    //  ORDER W.R.T. LAST OP IN STACK (ACCORDING APPLY ADDING STACK OR USE OPER
    //  FUNC)
    auto it_op = find(operator_list.begin(), operator_list.end(), ch);
    if (it_op != operator_list.end()) {
      if (!op_stk.empty() && priority[op_stk.top()] == 2 && priority[ch] == 1) {
        double created_num_from_string;
        bool rightt1;
        rightt1 = parsing_for_digits(str_num_maker, created_num_from_string);
        if (rightt1 == false) {
          return false;
        }
        num_stk.push(created_num_from_string);
        str_num_maker = "";
        ////
        bool right = mid_work(num_stk, op_stk, ch);
        if (right == false) {
          return false;
        }
      } else {
        double created_num_from_string;
        bool rightt1;
        rightt1 = parsing_for_digits(str_num_maker, created_num_from_string);
        if (rightt1 == false) {
          return false;
        }
        num_stk.push(created_num_from_string);
        str_num_maker = "";
        ////
        op_stk.push(ch);
      }
      prev_ch = ch;
      continue;
    }
  } // CHECK POINT
  if (str_num_maker != "") {
    bool rightt;
    double created_num_from_string;
    rightt = parsing_for_digits(str_num_maker, created_num_from_string);
    if (rightt == false) {
      return false;
    }
    num_stk.push(created_num_from_string);
  }
  bool rright;
  rright = total_work(num_stk, op_stk);
  if (rright == false) {
    return false;
  }
  ans = num_stk.top();
  // IF OPERATOR CHECK IF IT IS LAST CHARACTER
  return true;
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
      cin.ignore();
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
        // stringstream lin(calc_in);
        // // cin.ignore();
        // string str_num1, str_num2;
        // ll num1, num2;
        // string op;
        // string extra;
        // if (lin >> str_num1 >> op >> str_num2) {

        //   if (lin >> extra) {
        //     cout << "\nInvalid Syntax : Check Guide Menu for more info\n";

        //   } else if (is_long_long(str_num1, num1) &&
        //              is_long_long(str_num2, num2)) {
        //     if (op == "+" || op == "-" || op == "/" ||
        //         op == "*") { // op is correct       //// do calc
        //       cout << "\nRESULT : " << oper(num1, num2, op) << endl;
        //     } else {
        //       cout << "\nInvalid Syntax : Check Guide Menu for more info\n";
        //     }
        //   } else {
        //     cout << "\nInvalid Syntax : Check Guide Menu for more info\n";
        //   }
        // }

        /////////////////////////////////////////////////////////////////////
        /////////////// NEW TRIAL ///////// NEW TRIAL ///////////////////////
        double ans;
        bool status = enum_input(calc_in, ans);
        if (status == false) {
          cout << "\n\n|= ERROR !!!\nIncorrect Please Try Again !!!\n";
        } else {
          cout << "\nRESULT = " << ans << endl;
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
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

bool num_string_pusher(
    string &str,
    stack<double> &num_stk) { // makes string of num empty and adds to stack_num
  if (!str.empty()) {         // if num_in_form_of_string_is _empty
    bool right;
    double created_num_from_string;
    right = parsing_for_digits(str, created_num_from_string);
    if (right == false) {
      return false;
    }
    num_stk.push(created_num_from_string);
    str = "";
    return true;
  } else {
    return true;
  }
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
    bool right1 = is_valid_pair(prev_ch, ch);
    if (right1 == false) {
      return false;
    }
    // IF IT IS WHITESPACE
    if (ch == ' ') {

      if (isdigit(prev_ch)) { // num _
        bool right2 = num_string_pusher(str_num_maker, num_stk);
        if (right2 == false) {
          return false;
        }
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
    if (it_op != operator_list.end()) { // ch == operator // num op // _ op //
      if (!op_stk.empty()) {            // op_stk not empty
        if (priority[op_stk.top()] == 2 &&
            priority[ch] == 1) { // priority problem
          bool right3 = num_string_pusher(str_num_maker, num_stk);
          if (right3 == false) {
            return false;
          }
          //
          bool right5 = mid_work(num_stk, op_stk, ch);
          if (right5 == false) {
            return false;
          }
        } else { // priority order is safe
          bool right6 = num_string_pusher(str_num_maker, num_stk);
          if (right6 == false) {
            return false;
          }
          op_stk.push(ch);
        }
      } else { // op_stk is empty
        bool right4 = num_string_pusher(str_num_maker, num_stk);
        if (right4 == false) {
          return false;
        }
        op_stk.push(ch);
      }
      /////// ABOVE IS REBUILD //////
      prev_ch = ch;
      continue;
    }
    // If ALL CASES FAILED//
    cout << "FAILURE OF CASES\n\n";
  } // all ch loop done
  // checked if all numbers r added to num_stk
  bool right10 = num_string_pusher(str_num_maker, num_stk);
  if (right10 == false) {
    return false;
  }
  //
  bool rright;
  rright = total_work(num_stk, op_stk);
  if (rright == false) {
    return false;
  }
  // Answer format
  ans = num_stk.top();
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
  cout << "To switch to suggestioin mode enter 's'" << endl;
  cout << "To quit the program enter 'q'" << endl;
  cout << "To activate developer mode enter 'd'\n" << endl;

  string user_input;
  while (true) {
    cout << "\nTO REPEAT THE INSTRUCTIONS enter 'r'\n\n";
    cout << ">> ";
    cin >> user_input;
    cout << "\n################################################################"
            "##"
            "\n";

    if (user_input == "q") { // quit function
      return 0;
    } else if (user_input == "r") {
      cout << endl << "REPEATING THE INSTRUCTIONS :" << endl << endl;
      ;
      cout << endl << "To activate calculator enter 'a'" << endl;
      cout << "To switch to suggestioin mode enter 's'" << endl;
      cout << "To quit the program enter 'q'" << endl;
      cout << "To activate developer mode enter 'd'\n" << endl;
    } else if (user_input == "d") { // developer mode
      cout << "\nDeveloper password = ";
      string pass_check;
      cin >> pass_check;
      if (pass_check == dev_pass) {
        cout << "\n&&& Congratulations You have Entered development mode !!! "
                "&&&\n";
        string dev_in;
        do {
          cout << "\n&*& Instructions :-\n";
          cout << "\nTo check history enter 'h'\nTo clear history enter "
                  "'hc'\nTo check suggestion box enter's'\nTo clear suggestion "
                  "box'sc'\nTo exit developer mode enter 'q'\n";
          cout << "\n>> ";
          cin >> dev_in;
          if (dev_in == "h") {
            ifstream check_his("History.txt");
            cout << "\n-+-+-+- HISTORY "
                    "-+-+-+-\n-+-+-+-+-+-+-+-+-+-+-+-\n\n";
            string line;
            while (getline(check_his, line)) {
              cout << line << endl;
            }
            cout << "-+-+-+-+-+-+-+-+-+-+-+-\n";
            check_his.close();
            continue;
          }
          if (dev_in == "hc") {
            ofstream clear_his("History.txt");
            clear_his.close();
          }
          //
          if (dev_in == "s") {
            ifstream check_sugg("suggestion.txt");
            cout << "\n^^^^^^^ SUGGESTIONS "
                    "^^^^^^^\n^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n";
            string line;
            while (getline(check_sugg, line)) {
              cout << line << endl;
            }
            cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
            check_sugg.close();
            continue;
          }
          if (dev_in == "sc") {
            ofstream clear_sugg("suggestion.txt");
            clear_sugg.close();
          }
          if (dev_in == "hc") {
            ofstream clear_his("History.txt");
            clear_his.close();
          }
        } while (dev_in != "q");
        cout << "\n&&& Successfully quited developer mode &&&\n";
      } else {
        cout << "OOPS !!!\nWrong password, Try again later\n\n";
      }
    } else if (user_input == "s") { // suggestion box
      cout << endl << "Give your suggestions below : " << endl;
      ofstream write_sugg("suggestion.txt", ios::app);

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
      cout << "Enter 'h' to view history" << endl;
      string calc_in;
      cin.ignore();
      ofstream write_his("History.txt", ios::app); // opened history.txt
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
        if (calc_in == "h") {
          ifstream check_his("History.txt");
          cout << "\n-+-+-+- HISTORY "
                  "-+-+-+-\n-+-+-+-+-+-+-+-+-+-+-+-\n\n";
          string line;
          while (getline(check_his, line)) {
            cout << line << endl;
          }
          cout << "-+-+-+-+-+-+-+-+-+-+-+-\n";
          check_his.close();
          continue;
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
          cout << "\n\n<<ERROR>> !!!\nIncorrect | Please Try Again !!!\n";
        } else {
          cout << "\nRESULT = " << ans << endl;
        }
        // history update

        if (status == false) {
          write_his << "- - - - Session - - - -" << endl;
          write_his << calc_in << endl;
          write_his << "\nERROR (No output)\n" << endl;
        } else {
          write_his << "- - - - Session - - - -" << endl;
          write_his << calc_in << endl;
          write_his << "\nOutput = " << ans << endl << endl;
        }

        /////////////////////////////////////////////////////////////////////
      } while (calc_in != "quit");
      write_his.close(); // closed history.txt
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
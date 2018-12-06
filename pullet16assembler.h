/****************************************************************
 * Header file for the Pullet16 assembler.
 *
 * Author/copyright:  Duncan A. Buell.  All rights reserved.
 * Used with permission and modified by: Jane Random Hacker
 * Date: 17 August 2018
**/

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::max;
using std::ofstream;
using std::set;
using std::string;
using std::vector;

#include "../../Utilities/scanner.h"
#include "../../Utilities/scanline.h"
#include "../../Utilities/utils.h"

#include "dabnamespace.h"
#include "codeline.h"
#include "hex.h"
#include "symbol.h"

class Assembler {
 public:
  Assembler();
  virtual ~Assembler();

  void Assemble(Scanner& in_scanner, string binary_filename,
                ofstream& out_stream);

 private:
  bool found_end_statement_;
  bool has_an_error_;

  const string kDummyCodeA = "1100110011001100";
  const string kDummyCodeB = "111100001111";
  const string kDummyCodeC = "1111000000000000";
  const string kDummyCodeD = "0000000011110000";

  int pc_in_assembler_;
  int maxpc_;
  vector<CodeLine> codelines_;
  vector<string> machine_code_lines_;
  map<int, string> machinecode_;
  map<string, Symbol> symboltable_;
  map<string, string> opcodes_ = { {"BAN", "000"},
                                   {"SUB", "001"},
                                   {"STC", "010"},
                                   {"AND", "011"},
                                   {"ADD", "100"},
                                   {"LD ", "101"},
                                   {"BR ", "110"},
                                   {"STP", "111"},
                                   {"RD ", "111"},
                                   {"WRT", "111"},
                                   {"HEX", "HEX"},
                                   {"ORG", "ORG"},
                                   {"END", "END"},
                                   {"DS ", "DS "}
                                 };
  set<string> mnemonics_;

  string GetInvalidMessage(string leadingtext, string invalidstring);
  string GetInvalidMessage(string leadingtext, Hex hex);
  string GetUndefinedMessage(string badtext);
  void PassOne(Scanner& in_scanner);
  void PassTwo();
  void PrintCodeLines();
  void PrintMachineCode(string binary_filename, ofstream& out_stream);
  void PrintSymbolTable();
  void SetNewPC(CodeLine codeline);
  void UpdateSymbolTable(int programcounter, string label);
};
#endif

#include "pullet16assembler.h"

/***************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456
 * Class 'Assembler' for assembling code.
 *
 * Author/copyright:  Duncan A. Buell.  All rights reserved.
 * Used with permission and modified by: Jane Random Hacker
 * Date: 17 August 2018
**/

/***************************************************************************
 * Constructor
**/
Assembler::Assembler() {

}

/***************************************************************************
 * Destructor
**/
Assembler::~Assembler() {
}

/***************************************************************************
 * Accessors and Mutators
**/

/***************************************************************************
 * General functions.
**/

/***************************************************************************
 * Function 'Assemble'.
 * This top level function assembles the code.
 *
 * Parameters:
 *   in_scanner - the scanner to read for source code
 *   out_stream - the output stream to write to
**/
void Assembler::Assemble(Scanner& in_scanner, string binary_filename,
                         ofstream& out_stream) {
#ifdef EBUG
  Utils::log_stream << "enter Assemble" << endl;
#endif

  //////////////////////////////////////////////////////////////////////////
  // Pass one
  // Produce the symbol table and detect errors in symbols.
  cout << "ENTERING PASS ONE" << endl;
  PassOne(in_scanner);
  cout << "EXITING PASS ONE" << endl;
  //////////////////////////////////////////////////////////////////////////
  // Pass two
  // Generate the machine code.
  PassTwo();
  //////////////////////////////////////////////////////////////////////////
  // Dump the results.

#ifdef EBUG
  Utils::log_stream << "leave Assemble" << endl;
#endif
}

/***************************************************************************
 * Function 'GetInvalidMessage'.
 * This creates a "value is invalid" error message.
 *
 * Parameters:
 *   leadingtext - the text of what it is that is invalid
 *   symbol - the symbol that is invalid
**/
string Assembler::GetInvalidMessage(string leadingtext, string symbol) {
  string returnvalue = "";
  return returnvalue;
}

/***************************************************************************
 * Function 'GetInvalidMessage'.
 * This creates a "value is invalid" error message.
 *
 * Parameters:
 *   leadingtext - the text of what it is that is invalid
 *   hex - the hex operand that is invalid
**/
string Assembler::GetInvalidMessage(string leadingtext, Hex hex) {
  string returnvalue = "";

  return returnvalue;
}

/***************************************************************************
 * Function 'GetUndefinedMessage'.
 * This creates a "symbol is undefined" error message.
 *
 * Parameters:
 *   badtext - the undefined symbol text
**/
string Assembler::GetUndefinedMessage(string badtext) {
  string returnvalue = "";
  return returnvalue;
}

/***************************************************************************
 * Function 'PassOne'.
 * Produce the symbol table and detect multiply defined symbols.
 *
 * CAVEAT: We have deliberately forced symbols and mnemonics to have
 *         blank spaces at the end and thus to be all the same length.
 *         Symbols are three characters, possibly with one or two blank at end.
 *         Mnemonics are three characters, possibly with one blank at end.
 *
 * Parameters:
 *   in_scanner - the input stream from which to read
 *   out-stream - the output stream to which to write
**/
void Assembler::PassOne(Scanner& in_scanner) {
#ifdef EBUG
  Utils::log_stream << "enter PassOne" << endl;
#endif
  int line_counter = 0; // stores a line location from the source file
  pc_in_assembler_ = 0;  // our place in program memory
  string assembly_code = "";  // a line of code from the source file
  string symbol_text = "";  // stores a label
  string nextLine = in_scanner.NextLine();
  while(nextLine != "") {
    // Reading in the next line of code
    assembly_code = nextLine;
    CodeLine new_line = CodeLine(line_counter, pc_in_assembler_, assembly_code);
    ++line_counter;  // moving to the next line
    codelines_.push_back(new_line);  // adding the code line to program memory
    cout << "ASSEMBLY: " << assembly_code << endl;

      if (new_line.IsAllComment()) {
      
      } else {
        // Adding to the symbol table
        if(new_line.HasLabel()) {
          symbol_text = new_line.GetLabel();
          UpdateSymbolTable(pc_in_assembler_, symbol_text);
        }
        ++pc_in_assembler_;  // moving to the next location in memory
      }
    nextLine = in_scanner.NextLine();
  }
PrintSymbolTable();
#ifdef EBUG
  Utils::log_stream << "leave PassOne" << endl;
#endif
}

/***************************************************************************
 * Function 'PassTwo'.
 * This function does pass two of the assembly process.
**/
void Assembler::PassTwo() {
#ifdef EBUG
  Utils::log_stream << "enter PassTwo" << endl;
#endif
  cout << "Entering Second Pass" << endl; 
  for (int i = 0; i < codelines_.size(); i++) {
    string machine_code;
    if (codelines_.at(i).IsAllComment() == false) {
      // First 3 bits
      cout << "CODELINE : " << codelines_.at(i).ToString() << endl;
      string mnemonic = codelines_.at(i).GetMnemonic();
      if (mnemonic != "nullmnemonic") {
        cout << "MNEMONIC: " << mnemonic << endl;
        string mnemonic_opcode = opcodes_.at(mnemonic);
        cout << "OPCODE FROM MAP: " << endl;
        // If opcode is RD/WRT/STP
        if (mnemonic_opcode == "111") {
          if (mnemonic == "RD ") {
            machine_code = "1110000000000001";
          } else if (mnemonic == "WRT") {
            machine_code = "1110000000000011";
          } else {
            machine_code = "1110000000000010";
          }
        } else if (mnemonic == "HEX"){
          cout << "I'M IN tHE HEX IF STATEMENT" << endl;
            int hex = codelines_.at(i).GetHexObject().GetValue();
            string bitstring_hex = DABnamespace::DecToBitString(hex, 16);
            machine_code = bitstring_hex;
            machine_code_lines_.push_back(machine_code);
        } else if (mnemonic == "ORG") {
            SetNewPC(codelines_.at(i + 1));  // ORG the next line
        } else if (mnemonic == "END") {
            i = codelines_.size();
        } else if (mnemonic == "DS") {
            int hex = codelines_.at(i).GetHexObject().GetValue();
            for(int i = 0; i < hex; i++) {
              machine_code_lines_.push_back(kDummyCodeA);
            }
        } else {
          machine_code += mnemonic_opcode;
          // 4th Bit
          string addr = codelines_.at(i).GetAddr();
          if (addr == "indirect") {
          machine_code += "1";
          } else if(addr == "direct") {
            machine_code += "0";
          }
          // Last 12 bits
          string symbol_text = codelines_.at(i).GetSymOperand();
          int symbol_loc = symboltable_.at(symbol_text).GetLocation();
          string symbol_bit_string = DABnamespace::DecToBitString(symbol_loc, 12);
          machine_code += symbol_bit_string;
          // Push onto vector
          machine_code_lines_.push_back(machine_code);
          cout << machine_code << endl;
        }
      }
    }
  }
  cout << "EXITING SECOND PASS" << endl;
#ifdef EBUG
  Utils::log_stream << "leave PassTwo" << endl;
#endif
}

/***************************************************************************
 * Function 'PrintCodeLines'.
 * This function prints the code lines.
**/
void Assembler::PrintCodeLines() {
#ifdef EBUG
  Utils::log_stream << "enter PrintCodeLines" << endl;
#endif
  string s = "";

  for (auto iter = codelines_.begin(); iter != codelines_.end(); ++iter) {
    s += (*iter).ToString() + '\n';
  }

  if (!found_end_statement_) {
    s += "\n***** ERROR -- NO 'END' STATEMENT\n";
    has_an_error_ = true;
  }

#ifdef EBUG
  Utils::log_stream << "leave PrintCodeLines" << endl;
#endif
  Utils::log_stream << s << endl;
}

/***************************************************************************
 * Function 'PrintMachineCode'.
 * This function prints the machine code.
**/
void Assembler::PrintMachineCode(string binary_filename,
                                 ofstream& out_stream) {
#ifdef EBUG
  Utils::log_stream << "enter PrintMachineCode" << " "
                    << binary_filename << endl;
#endif
  string s = "";

#ifdef EBUG
  Utils::log_stream << "leave PrintMachineCode" << endl;
#endif
}

/******************************************************************************
 * Function 'PrintSymbolTable'.
 * This function prints the symbol table.
**/
void Assembler::PrintSymbolTable() {
#ifdef EBUG
  Utils::log_stream << "enter PrintSymbolTable" << endl;
#endif
  for (auto a = symboltable_.begin(); a != symboltable_.end(); a++) {
    cout << " Symbol " << a->second.ToString() << endl;
  }
#ifdef EBUG
  Utils::log_stream << "leave PrintSymbolTable" << endl;
#endif
}

/******************************************************************************
 * Function 'SetNewPC'.
 * This function gets a new value for the program counter.
 *
 * No return value--this sets the class's PC variable.
 *
 * Parameters:
 *   codeline - the line of code from which to update
**/
void Assembler::SetNewPC(CodeLine codeline) {
#ifdef EBUG
  Utils::log_stream << "enter SetNewPC" << endl;
#endif

int hex = codeline.GetHexObject().GetValue();
codeline.SetPC(hex);

#ifdef EBUG
  Utils::log_stream << "leave SetNewPC" << endl;
#endif
}

/******************************************************************************
 * Function 'UpdateSymbolTable'.
 * This function updates the symbol table for a putative symbol.
 * Note that there is a hack here, in that the default value is 0
 * and that would mean we can't store a symbol at location zero.
 * So we add one, and then back that out after the full first pass is done.
**/
void Assembler::UpdateSymbolTable(int pc, string symboltext) {
#ifdef EBUG
  Utils::log_stream << "enter UpdateSymbolTable" << endl;
#endif
Symbol new_symbol = Symbol(symboltext, pc);  // creating the new symbol
 // Checking to see if there is a duplicate symbol 
if (symboltable_.count(symboltext) == 0) {
	symboltable_.insert( std::pair<string, Symbol>(symboltext, new_symbol));
  // found no duplicates
} else {
	// found a duplicate, so store the pc to keep track of when the duplicate
	// appeared during execution
	duplicates_.push_back(pc);
}
#ifdef EBUG
  Utils::log_stream << "leave UpdateSymbolTable" << endl;
#endif
}

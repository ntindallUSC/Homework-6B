#include "codeline.h"
/***************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
 * Class 'CodeLine' as a container for one line of code.
 *
 * Author/copyright:  Duncan A. Buell.  All rights reserved.
 * Used with permission and modified by: Jane Random Hacker
 * Date: 17 August 2018
**/

/***************************************************************************
 * Constructor
**/
CodeLine::CodeLine() {
}
/***************************************************************************
 * Constructor
**/
CodeLine::CodeLine(int linecounter, int pc, string assemblycode) {
 // First check to see whether the code is all comment or instruction
  string comment_indicator = assemblycode.substr(0, 1);
  if (comment_indicator.compare("*") == 0 || assemblycode.size() == 1) {
    SetCommentsOnly(linecounter, assemblycode);
  } else {
    // The following lines of code break up the input assembly code into its
    // corresponding parts as outlined in the homework pdf if the code isn't
    // a comment.
    std::string label = assemblycode.substr(0, 3);
    std::string mnemonic = assemblycode.substr(4, 3);
     if (assemblycode.length() > 20) {
      std::string addr = assemblycode.substr(8, 1);
      std::string symoperand = assemblycode.substr(10, 3);
      std::string hexoperand = assemblycode.substr(14, 5);
      // The comment substring should run from the begining of the comment at
      // spot 20 until it ends. Thus the length of the whole line minus 19
      // gives the value of spaces remaining from the 20th position.
      int length_of_comment = assemblycode.length()-19;
      std::string comments = assemblycode.substr(20, length_of_comment);
      SetCodeLine(linecounter, pc, label, mnemonic, addr, symoperand, hexoperand, comments);
    } else if (assemblycode.length() > 7 && assemblycode.length() < 20) {
      std::string addr = assemblycode.substr(8,1);
      std::string symoperand = assemblycode.substr(10, 3);
      std::string hexoperand = assemblycode.substr(14, 5);
      SetCodeLine(linecounter, pc, label, mnemonic, addr, symoperand, 
                  hexoperand, "nullcomments");
    } else {
    SetCodeLine(linecounter, pc, label, mnemonic, "nulladdr", "nullsymoperand",
    "     ", "nullcomments");
    }
  }
}
/***************************************************************************
 * Destructor
**/
CodeLine::~CodeLine() {
}

/***************************************************************************
 * Accessors and Mutators
**/

/***************************************************************************
 * Accessor for the 'addr_'.
**/
string CodeLine::GetAddr() const {
  string returnvalue;
  if (addr_.compare("*") == 0) {
    returnvalue = "indirect";
  } else {
    returnvalue = "direct";
  }
  return returnvalue;
}

/***************************************************************************
 * Accessor for the 'comments_'.
**/
string CodeLine::GetComments() const {
  return comments_;
}

/***************************************************************************
 * Accessor for the 'code_'
**/
string CodeLine::GetMachineCode() const {
  return code_;
}

/***************************************************************************
 * Accessor for the 'error_messages_'.
**/
string CodeLine::GetErrorMessages() const {
  return error_messages_;
}

/***************************************************************************
 * Accessor for the 'hex_'.
**/
Hex CodeLine::GetHexObject() const {
  return hex_;
}

/***************************************************************************
 * Accessor for the 'label_'.
**/
string CodeLine::GetLabel() const {
  return label_;
}

/***************************************************************************
 * Accessor for the 'mnemonic_'.
**/
string CodeLine::GetMnemonic() const {
  return mnemonic_;
}

/***************************************************************************
 * Accessor for the 'pc_'
**/
int CodeLine::GetPC() const {
  return pc_;
}
/***************************************************************************
 * Accessor for the 'symoperand_'.
**/
string CodeLine::GetSymOperand() const {
  return symoperand_;
}

/***************************************************************************
 * Boolean indicator of the presence of a label.
**/
bool CodeLine::HasLabel() const {
  bool something = true;
  if (label_ == "nulllabel") {
    something = false;
  }
  return something;
}

/***************************************************************************
 * Boolean indicator of the presence of a symbolic operand.
**/
bool CodeLine::HasSymOperand() const {
  bool something = true;
  if ( symoperand_ == "nullsymoperand") {
    something = false;
  }
  return something;
}

/***************************************************************************
 * Accessor for 'is_all_comment_'.
**/
bool CodeLine::IsAllComment() const {
  return is_all_comment_;
}

/***************************************************************************
 * General functions.
**/

/***************************************************************************
 * Function 'SetCodeLine'.
 * Sets the values for a line of code that isn't all comments.
 *
 * Parameters:
 *   linecounter - the line counter for this line in source
 *   pc - the program counter if it is there
 *   label - the label if it is there
 *   mnemonic - the mnemonic of the opcode
 *   addr - the direct/indirect addresssing asterisk
 *   symoperand - the symbolic operand if it is there
 *   hexoperand - the hexoperand if it is there
 *   comments - comments if any
**/
void CodeLine::SetCodeLine(int linecounter, int pc, string label,
                           string mnemonic, string addr, string symoperand,
                           string hexoperand, string comments) {
  linecounter_ = linecounter;
  pc_ = pc;
  if (label == "   ") {
    label_ = "nulllabel";
  } else {
    label_ = label;
  }
  mnemonic_ = mnemonic;
  addr_ = addr;
  if (symoperand == "   ") {
    symoperand_ = "nullsymoperand";
  } else {
    symoperand_ = symoperand;
  }
  hex_ = Hex(hexoperand);
  comments_ = comments;
  code_ = "nullcode";
  is_all_comment_ = false;
}

/***************************************************************************
 * Function 'SetCommentsOnly'.
 * Sets a line of code that is all comments.
 *
 * Parameters:
 *   line - the code line that is taken to be all comments
**/
void CodeLine::SetCommentsOnly(int linecounter, string line) {
  comments_ = line;
  bool is_all_comment_ = true;
  label_ = "nulllabel";
  pc_ = -1;
  linecounter_ = linecounter;
  mnemonic_ = "nullmnemonic";
  addr_ = "nulladdr_";
  symoperand_ = "nullsymoperand";
  hex_ = Hex("     ");
  code_ = "nullcode";
}

/***************************************************************************
 * Function 'SetErrorMessages'.
 * Sets the 'error_messages_' variable for later printing.
 *
 * Parameters:
 *   messages - the string of messages
**/
void CodeLine::SetErrorMessages(string messages) {
  comments_ = comments_ + " " + messages;
}

/***************************************************************************
 * Function 'SetMachineCode'.
 * Set the 'code_' for this line of code.
 *
 * Parameters:
 *   code - the code to set
**/
void CodeLine::SetMachineCode(string code) {
  code_ = code;
}

/***************************************************************************
 * Function 'SetPC'.
 * Set the PC for this line of code.
 *
 * Parameters:
 *   what - the value to set as the PC
**/
void CodeLine::SetPC(int what) {
  if ( what >= 0 && what < 4096) {
    pc_= what;
  }
}

/***************************************************************************
 * Function 'ToString'.
 * This function formats a 'CodeLine' for prettyprinting.
 *
 * Returns:
 *   the prettyprint string for printing
**/
string CodeLine::ToString() const {
#ifdef EBUG
  Utils::log_stream << "enter ToString" << endl;
#endif
  string s = "";


  s += Utils::Format(linecounter_, 5) + " ";
  if ( pc_ < 0 ) {
    s += Utils::Format("    ", 5) + " ";
   // s += Utils::Format(" " , 12) + " ";
  } else {
    s += Utils::Format(pc_, 4) + "  ";
    //s += DABnamespace::DecToBitString(pc_, 12) + " ";
  }

  if (code_ == "nullcode") {
    s += Utils::Format("xxxx xxxx xxxx xxxx", 19);
  } else {
    s = s + Utils::Format((code_).substr(0 ,4), 4)
          + " " + Utils::Format((code_).substr(4 ,4), 4)
          + " " + Utils::Format((code_).substr(8 ,4), 4)
          + " " + Utils::Format((code_).substr(12 ,4), 4);
  }

  if (label_ == "nulllabel") {
    s += " " + Utils::Format("...", 3);
  } else {
    s += " " + Utils::Format(label_, 3);
  }

  if (mnemonic_ == "nullmnemonic") {
    s += " " + Utils::Format("...", 3);
  } else {
    s += " " + Utils::Format(mnemonic_, 6);
  }

  if (addr_ == "indirect") {
    s += " " + Utils::Format("*", 1);
  } else {
    s += " " + Utils::Format(" ", 1);
  }

  if (symoperand_ == "nullsymoperand") {
    s += " " + Utils::Format("...", 3);
  } else {
    s += " " + Utils::Format(symoperand_, 3);
  }

  s += " " + hex_.ToString();
  

  if (comments_ != "nullcomments") {
    if (is_all_comment_) {
      s = Utils::Format(linecounter_, 5) + " ";
      s += Utils::Format(" ", 41);
      s += " " + comments_;
    } else {
      s += " " + comments_;
    }
  }

  if ((error_messages_.length() > 0) &&
      (error_messages_ != "nullerrormessages")) {
    s += error_messages_;
  }

#ifdef EBUG
  Utils::log_stream << "leave ToString" << endl;
#endif
  return s;
}

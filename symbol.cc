#include "symbol.h"
/******************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
 * Class 'Symbol' as a container for one symbol.
 *
 * Author/copyright:  Duncan A. Buell.  All rights reserved.
 * Used with permission and modified by: Anthony M. Castellano
 * Date: 16 November 2018
**/

/******************************************************************************
 * Constructor
**/
Symbol::Symbol() {
}

/******************************************************************************
 * Constructor
**/
Symbol::Symbol(string text, int programcounter) {
  text_ = text;
  location_ = programcounter;
  is_multiply_ = false;
}

/******************************************************************************
 * Destructor
**/
Symbol::~Symbol() {
}

/******************************************************************************
 * Accessors and Mutators
**/

/******************************************************************************
 * Accessor for 'error_messages_'.
**/
string Symbol::GetErrorMessages() const {
  bool previouserror = false;
  string error_messages = "";

  return error_messages;
}

/******************************************************************************
 * Accessor for the 'location_'.
**/
int Symbol::GetLocation() const {
  return location_;
}

/******************************************************************************
 * Accessor for the existence of errors.
**/
bool Symbol::HasAnError() const {
  return (is_invalid_ || is_multiply_);
}

/******************************************************************************
 * Mutator 'SetMultiply'.
 * Sets the 'is_multiply' value to 'true'.
**/
void Symbol::SetMultiply() {
  is_multiply_ = true;
}

/******************************************************************************
 * General functions.
**/

/******************************************************************************
 * Function 'CheckInvalid'.
 * Returns the boolean to say whether a symbol is invalid.
**/
bool Symbol::CheckInvalid() const {
  bool returnvalue = false;  // false means no, not invalid

  // Confirm symbol is one to three characters long
  if (text_.length() < 1 || text_.length() > 3) {
    returnvalue = true;
  }

  // Check if first character is alpha character
  char testchar = text_.at(0);
  if (!isalpha(testchar)) {
    returnvalue = true;
  }

  // Check if all characters are alphanumeric
  for (int i = 0; i < text_.length(); ++i) {
    char testchar = text_.at(i);
    if (!isalnum(testchar) && testchar != ' ') {
      returnvalue = true;
    }
  }

  // Check if symbol contains space in the middle
  for (int i = 1; i < text_.length()-1; ++i) {
    if (text_.at(i) == ' ') {
      returnvalue = true;
    }
  }

  return returnvalue;
}

/******************************************************************************
 * Function 'ToString'.
 * This function formats an 'Symbol' for prettyprinting.
 *
 * Returns:
 *   the prettyprint string for printing
**/
string Symbol::ToString() const {
#ifdef EBUG
  Utils::log_stream << "enter ToString" << endl;
#endif
  string s = "";

  if (text_ == "nullsymbol") {
    s += Utils::Format("sss", 3);
  } else {
    s += Utils::Format(text_, 3);
  }

  s += Utils::Format(location_, 4);
  if (is_invalid_) {
    s += " INVALID";
  }
  if (is_multiply_) {
    s += " MULTIPLY";
  }

#ifdef EBUG
  Utils::log_stream << "leave ToString" << endl;
#endif
  return s;
}
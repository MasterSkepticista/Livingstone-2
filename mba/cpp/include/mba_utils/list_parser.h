/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

//
// A simple set of functions to parse lists.
// $Id: list_parser.h,v 1.5 2004/01/28 23:31:37 lbrown Exp $
//

// The List_parser class provides a set of methods to read in Lisp-like
// lists.  The class can be used to read in whole lists, as well as to
// identify lexical items related to lists, i.e., it can be used both as a
// parser and a lexer.  
// 
// The BNF for a list is the following:
//   LIST ::= ATOM | (LIST+)
//
// An ATOM is a contiguous sequence of non-space, non-parentheses
// characters.  The class can be configured with a set of punctuation
// characters that can also delimit atoms.  However, punctuation characters
// cannot be embedded within lists.
//
// Types:
//  enum Lexicon
//   Enumerates different return values from the parsing functions.
//   LIST_EOF          End of file has been encountered; no parsing errors
//   LIST_LEXER_ERROR  Lexer has encountered a parsing error
//   LIST_LEFT_PAREN   Lexer has encountered a left parenthesis
//   LIST_RIGHT_PAREN  Lexer has encountered a right parenthesis
//   LIST_ATOM         Lexer has encountered an atom
//   LIST_PUNCTUATION  Lexer or parser has encountered a punctuation character
//   LIST_PARSER_ERROR Parser has encountered a parsing error
//   LIST_PARSER_DONE  Parser has parsed a complete list
//
// Public interface:
//
//  List_parser(long lexer_buffer_size = 1024, 
//              long parser_buffer_size = 1024,
//              long number_of_punctuations = 10)
//   Construct a parser object with specified sizes for the buffers used by
//   the lexer and the parser, and providing for a maximum number of
//   punctuation characters.
//
//  ~List_parser()
//    Destructor.
//
//  Lexicon extract_next_item(_STD_ istream& is) 
//   Extract the next lexical item from input stream is.  Return value can
//   be LIST_EOF, LIST_LEXER_ERROR, LIST_LEFT_PAREN, LIST_RIGHT_PAREN,
//   LIST_ATOM, or LIST_PUNCTUATION.  Method last_lexical_item() returns
//   the string representing the lexical item just extracted.
//
//  const_char_p last_lexical_item()
//   Returns a pointer to a character array that holds the string
//   representing the last lexical item extracted by extract_next_item().
//
//  Lexicon parse(_STD_ istream& is)
//   Extract the next list from input stream is.  If the next item
//   encountered is a punctuation mark, then simply extract the punctuation
//   mark.  Return value can be LIST_EOF, LIST_PUNCTUATION,
//   LIST_PARSER_ERROR, or LIST_PARSER_DONE.  When a list is successfully
//   extracted, then it returns LIST_PARSER_DONE.  The actual list being
//   extracted can be found by calling last_parsed_list().
//
//  const_char_p last_parsed_item()
//   Returns a pointer to a character array that holds the string
//   representing the last extracted list or last punctuation character.
//
//  bool add_punctuation(char c)
//   Adds c as a new punctuation character.  Returns false if too many
//   punctuation characters have been added.


#ifndef LIST_PARSER_H
#define LIST_PARSER_H

#include <livingstone/l2conf.h>


#include <livingstone/L2_iostream.h>
#include <assert.h>

#include "utils.h"

class List_parser {

public:

  enum Lexicon  { LIST_EOF = -1, 
		  LIST_LEXER_ERROR, 
		  LIST_LEFT_PAREN, 
		  LIST_RIGHT_PAREN, 
		  LIST_ATOM,
		  LIST_PUNCTUATION,
		  LIST_PARSER_ERROR,
		  LIST_PARSER_DONE
  };

  List_parser(long lexer_buffer_size = 1024,
	     long parser_buffer_size = 1024,
	     long number_of_punctuations = 10) 
    : lexer_size(lexer_buffer_size),
      parser_size(parser_buffer_size),
      punctuation_size(number_of_punctuations),
      punctuation_end(0)
  {
    lexer_buffer = new char[lexer_buffer_size];
    if (lexer_buffer)
      lexer_buffer[0] = '\0';
    parser_buffer = new char[parser_buffer_size];
    if (parser_buffer)
      parser_buffer[0] = '\0';
    punctuations = new char[punctuation_size];
    if (punctuations)
      punctuations[0] = '\0';
    assert(lexer_buffer && parser_buffer && punctuations);
  }

  ~List_parser() {
    delete lexer_buffer;
    delete parser_buffer;
  }

  Lexicon extract_next_item(_STD_ istream& is);

  const_char_p last_lexical_item() { return lexer_buffer; }

  Lexicon parse(_STD_ istream& is);

  const_char_p last_parsed_item() { return parser_buffer; }

  bool add_punctuation(char c) {
    if (punctuation_end >= punctuation_size - 1)
      return false;
    else {
      punctuations[punctuation_end++] = c;
      punctuations[punctuation_end] = '\0';
      return true;
    }
  }

private:

  char_p lexer_buffer;
  char_p parser_buffer;
  char_p punctuations;
  long   lexer_size;
  long   parser_size;
  long   punctuation_size;
  long   punctuation_end;
  
  Lexicon read_list_atom(_STD_ istream& is);

  Lexicon copy_lexer_into_parser_buffer(long& index);

  bool is_punctuation(char c) {
    char_p punc_p = punctuations;
    for (; *punc_p; ++punc_p)
      if (*punc_p == c)
	return true;
    return false;
  }
};



#endif // LIST_PARSER_H

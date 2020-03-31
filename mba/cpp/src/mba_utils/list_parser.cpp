/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

//
// Implementation of the list parser.
// $Id: list_parser.cpp,v 1.5 2004/01/28 23:31:48 lbrown Exp $
//

#include <ctype.h>
#include <iomanip.h>

#include <mba_utils/list_parser.h>

List_parser::Lexicon 
List_parser::extract_next_item(_STD_ istream& is)
{
  char ch;

  lexer_buffer[0] = '\0';
  if (is.eof())
    return LIST_EOF;
  else if (!is)
    return LIST_LEXER_ERROR;

  if (!(is >> ch))
    return LIST_EOF;

  if (ch == '(') {
    lexer_buffer[0] = '(';
    lexer_buffer[1] = '\0';
    return LIST_LEFT_PAREN;
  } else if (ch == ')') {
    lexer_buffer[0] = '(';
    lexer_buffer[1] = '\0';
    return LIST_RIGHT_PAREN;
  } else if (is_punctuation(ch)) {
    lexer_buffer[0] = ch;
    lexer_buffer[1] = '\0';
    return LIST_PUNCTUATION;
  } else { // it's an atom
    is.putback(ch);
    return read_list_atom(is);
  }
}

List_parser::Lexicon
List_parser::read_list_atom(_STD_ istream& is)
{
  int i = 0;
  char ch;

  for (i = 0; i < lexer_size - 1 && is.get(ch); ++i) {
    if (!isspace(ch) && ch != '(' && ch != ')' && !is_punctuation(ch))
      lexer_buffer[i] = ch;
    else {
      lexer_buffer[i] = '\0';
      is.putback(ch);
      return LIST_ATOM;
    }
  }
  lexer_buffer[i] = '\0';       // definitely end the atom's name
  if (i == lexer_size - 1) {
    int next = is.peek();
    if (next == EOF || isspace(next) || 
	next == '(' || next == ')'|| is_punctuation(next))
      return LIST_ATOM;
    else  // atom name is too long for the lexer buffer size
      return LIST_LEXER_ERROR;
  } else  // the atom is complete
    return LIST_ATOM;
}

List_parser::Lexicon
List_parser::copy_lexer_into_parser_buffer(long& index)
{
  long i = 0;

  for (; i < lexer_size && index < parser_size; ++i, ++index) {
    parser_buffer[index] = lexer_buffer[i];
    if (lexer_buffer[i] == '\0')
      break;
  }
  if (index == parser_size) {
    parser_buffer[--index] = '\0';
    return LIST_PARSER_ERROR;
  } else {
    return LIST_PARSER_DONE;
  }
}

List_parser::Lexicon
List_parser::parse(_STD_ istream& is)
{
  Lexicon item;
  long    index = 0;        // points to next empty spot < parser_size
  int     paren_count = 0;

  do {
    switch (extract_next_item(is)) {
    case LIST_EOF: 
      parser_buffer[index] = '\0';
      if (paren_count == 0)
	return LIST_EOF;
      else
	return LIST_PARSER_ERROR;
      break;
    case LIST_LEXER_ERROR:
      parser_buffer[index] = '\0';
      return LIST_PARSER_ERROR;
      break;
    case LIST_LEFT_PAREN:
      if (index >= parser_size - 1) {
	parser_buffer[parser_size - 1] = '\0';
	return LIST_PARSER_ERROR;
      }
      ++paren_count;
      if (index > 0)
	if (parser_buffer[index - 1] != '(')
	  parser_buffer[index++] = ' ';
      parser_buffer[index++] = '(';
      break;
    case LIST_RIGHT_PAREN:
      if (index >= parser_size - 1) {
	parser_buffer[parser_size - 1] = '\0';
	return LIST_PARSER_ERROR;
      }
      --paren_count;
      parser_buffer[index++] = ')';
      break;
    case LIST_PUNCTUATION:
      if (paren_count == 0) {
	parser_buffer[0] = lexer_buffer[0];
	parser_buffer[1] = '\0';
	return LIST_PUNCTUATION;
      } else 
	return LIST_PARSER_ERROR;
      break;
    case LIST_ATOM:
      if (index >= parser_size - 1) {
	parser_buffer[parser_size - 1] = '\0';
	return LIST_PARSER_ERROR;
      }
      if (index > 0)
	if (parser_buffer[index - 1] != '(')
	  parser_buffer[index++] = ' ';
      item = copy_lexer_into_parser_buffer(index);
      if (item == LIST_PARSER_ERROR)
	return LIST_PARSER_ERROR;
      break;
    }
  } while (paren_count > 0);
  parser_buffer[index] = '\0';
  if (paren_count == 0)
    return LIST_PARSER_DONE;
  else
    return LIST_PARSER_ERROR;
}

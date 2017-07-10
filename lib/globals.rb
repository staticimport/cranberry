
require './lib/util/regexp_utils.rb'

module Cranberry
  $CURRENT_FILENAME = nil
  $CURRENT_LINE     = nil
  $CURRENT_LINENUM  = 0
  $CURRENT_LINECOL  = 0

  # Definitions
  KEYWORDS = ['and','bool','catch','def','elsif','fcn','flt','if','int','next','nil',
              'no','not','or','self','str','super','unless','until','while',
              'yes','yield','val','var']
  OPERATORS = ['+','-','*','/','=','==','<','<=','>','>=','<=>','(',
               ')','[',']','.','->','#']

  # Sortings
  KEYWORDS_ASCENDING_LENGTH   = KEYWORDS.sort{|a,b| a.length <=> b.length}
  KEYWORDS_DESCENDING_LENGTH  = KEYWORDS_ASCENDING_LENGTH.reverse
  OPERATORS_ASCENDING_LENGTH  = OPERATORS.sort{|a,b| a.length <=> b.length}
  OPERATORS_DESCENDING_LENGTH = OPERATORS_ASCENDING_LENGTH.reverse

  # Regular expressions (use ascending length to match longest first)
  KEYWORDS_REGEXP       = values_regexp(KEYWORDS_ASCENDING_LENGTH.sort)
  #TODO needed? KEYWORDS_START_REGEXP = start_values_regexp(KEYWORDS_ASCENDING_LENGTH)
  #TODO needed? KEYWORDS_FULL_REGEXP  = full_values_regexp(KEYWORDS_ASCENDING_LENGTH)
  OPERATORS_REGEXP      = values_regexp(OPERATORS_ASCENDING_LENGTH)
end


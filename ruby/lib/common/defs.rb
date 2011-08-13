
require 'lib/util/regexp_utils.rb'

module Cranberry
  # Definitions
  KEYWORDS = ['and','catch','class','elsif','false','if','next','nil','not','or','self','super','true','unless','until','while','yield']
  OPERATORS = ['+','-','*','/','=','==','<','<=','>','>=','<=>','(',
               ')','[',']','.','->']

  # Sortings
  KEYWORDS_ASCENDING_LENGTH = KEYWORDS.sort{|a,b| -a.length <=> -b.length}
  KEYWORDS_DESCENDING_LENGTH = KEYWORDS_ASCENDING_LENGTH.reverse
  OPERATORS_ASCENDING_LENGTH = OPERATORS.sort{|a,b| -a.length <=> -b.length}
  OPERATORS_DESCENDING_LENGTH = OPERATORS_ASCENDING_LENGTH.reverse

  # Regular expressions (use ascending length to match longest first)
  KEYWORDS_REGEXP = values_regexp(KEYWORDS_ASCENDING_LENGTH.sort)
  KEYWORDS_START_REGEXP = start_values_regexp(KEYWORDS_ASCENDING_LENGTH)
  KEYWORDS_FULL_REGEXP = full_values_regexp(KEYWORDS_ASCENDING_LENGTH)
  OPERATORS_REGEXP = values_regexp(OPERATORS_ASCENDING_LENGTH)
end


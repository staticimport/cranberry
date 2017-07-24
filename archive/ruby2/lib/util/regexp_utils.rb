
module Cranberry
  REGEXP_META_CHARS = ['/','^','[',']','.','$','{','}','*','(',')',
                       '+','|','?','<','>']
  # TODO: need this? REGEXP_META_CHARS_ESCAPED = ['\+'] #REGEXP_META_CHARS.collect{|c| '\\' + c}

  def self.escape_regexp_meta_chars(s)
    s = s.clone
    REGEXP_META_CHARS.each do |c|
      s.gsub!(c, "\\\\" + c)
    end
    s
  end

  def self.escape_regexp_meta_chars_all(strs)
    strs.collect{|s| escape_regexp_meta_chars(s)}
  end

  def self.values_regexp(values)
    values = escape_regexp_meta_chars_all(values)
    Regexp.new("(#{values.join('|')})")
  end

  ## TODO any of these needed? defs.rb
  #def self.end_values_regexp(values)
  #  values = escape_regexp_meta_chars_all(values)
  #  Regexp.new("(#{values.join('|')})$")
  #end

  #def self.full_values_regexp(values)
  #  values = escape_regexp_meta_chars_all(values)
  #  Regexp.new("^(#{values.join('|')})$")
  #end

  #def self.start_values_regexp(values)
  #  values = escape_regexp_meta_chars_all(values)
  #  Regexp.new("^(#{values.join('|')})")
  #end
end


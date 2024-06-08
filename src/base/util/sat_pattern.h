#ifndef sat_pattern_included
#define sat_pattern_included
//----------------------------------------------------------------------

//TODO: rename to regex or something similar?
// not really a regex.. ?

// based on code from: http://c.snippets.org/browser.php
// "All source code free as noted."

//----------------------------------------------------------------------

// Match the pattern PATTERN against the string TEXT;
//
//     match() returns true if pattern matches, false otherwise.
//     matche() returns MATCH_VALID if pattern matches, or an errorcode
//     as follows otherwise:
//
//         MATCH_PATTERN  - bad pattern
//         MATCH_LITERAL  - match failure on literal mismatch
//         MATCH_RANGE    - match failure on [..] construct
//         MATCH_ABORT    - premature end of text string
//         MATCH_END      - premature end of pattern string
//         MATCH_VALID    - valid match
//
// A match means the entire string TEXT is used up in matching.
//
// In the pattern string:
//     `*' matches any sequence of characters (zero or more)
//     `?' matches any character
//     [SET] matches any character in the specified set,
//     [!SET] or [^SET] matches any character not in the specified set.
//
// A set is composed of characters or ranges; a range looks like
// character hyphen character (as in 0-9 or A-Z).  [0-9a-zA-Z_] is the
// minimal set of characters allowed in the [..] pattern construct.
// Other characters are allowed (ie. 8 bit characters) if your system
// will support them.
//
// To suppress the special syntactic significance of any of `[]*?!^-\',
// and match the character exactly, precede it with a `\'.

// *[a-z] - match

//----------------------------------------------------------------------

// match defines
#define SAT_MATCH_PATTERN  6    // bad pattern
#define SAT_MATCH_LITERAL  5    // match failure on literal match
#define SAT_MATCH_RANGE    4    // match failure on [..] construct
#define SAT_MATCH_ABORT    3    // premature end of text string
#define SAT_MATCH_END      2    // premature end of pattern string
#define SAT_MATCH_VALID    1    // valid match

// pattern defines
#define SAT_PATTERN_VALID  0    // valid pattern
#define SAT_PATTERN_ESC   -1    // literal escape at end of pattern
#define SAT_PATTERN_RANGE -2    // malformed range in [..] construct
#define SAT_PATTERN_CLOSE -3    // no end bracket in [..] construct
#define SAT_PATTERN_EMPTY -4    // [..] construct is empty

//----------

const char* sat_pattern_error_text[] =
{
  "[..] construct is empty",
  "no end bracket in [..] construct",
  "malformed range in [..] construct",
  "literal escape at end of pattern",
  "valid pattern",
  "valid match",
  "premature end of pattern string",
  "premature end of text string",
  "match failure on [..] construct",
  "match failure on literal match",
  "bad pattern"
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Pattern {

//------------------------------
private:
//------------------------------

  int   m_Error;
  bool  m_ValidPattern;
  char* m_Pattern;

//------------------------------
public:
//------------------------------

  SAT_Pattern() {
    m_Error = 0;
    m_ValidPattern = false;
    m_Pattern = nullptr;//"*";
  }

  //----------

  ~SAT_Pattern() {
  }

  //----------

  SAT_Pattern(char* a_Pattern) {
    m_Error = 0;
    m_Pattern = a_Pattern;
    m_ValidPattern = _is_valid_pattern(a_Pattern);
  }

  //----------

  bool setPattern(char* a_Pattern) {
    bool valid = _is_valid_pattern(a_Pattern);
    if (valid) m_Pattern = a_Pattern;
    else m_Pattern = nullptr;
    m_ValidPattern = valid;
    return valid;
  }

  //----------

  bool match(char* a_Text) {
    if (m_ValidPattern) {
      m_Error = _match(m_Pattern,a_Text);
      return (m_Error==SAT_MATCH_VALID ) ? true : false;
    }
    else return false;
  }

  //----------

  // warning, hardcoded error numbers!

  const char* getErrorText(void) {
    int index = m_Error+4;
    if (index>=0 && index<11) return sat_pattern_error_text[index+4];
    return "";
  }

  //----------

  //inline
  bool validPattern(void) {
    return m_ValidPattern;
  }

  //----------

  // helpers...

  bool setPattern(const char* a_Pattern) {
    return setPattern((char*)a_Pattern);
  }

  bool match(const char* a_Text) {
    return match((char*)a_Text);
  }

//------------------------------
private:
//------------------------------

  bool _is_valid_pattern(char* a_Pattern) {
    //int error_type = SAT_PATTERN_VALID;
    m_Error = SAT_PATTERN_VALID;
    char* p = a_Pattern;
    while (*p) {
      switch (*p) {
        // check literal escape, it cannot be at end of pattern
        case '\\':
          if (!*++p) { 
            m_Error = SAT_PATTERN_ESC;
            return false;
          }
          p++;
          break;
        // the [..] construct must be well formed
        case '[':
          p++;
          // if the next character is ']' then bad pattern
          if (*p==']') {
            m_Error = SAT_PATTERN_EMPTY;
            return false;
          }
          // if end of pattern here then bad pattern
          if (!*p) {
            m_Error = SAT_PATTERN_CLOSE;
            return false;
          }
          // loop to end of [..] construct
          while (*p != ']') {
            // check for literal escape
            if (*p=='\\') {
              p++;
              // if end of pattern here then bad pattern
              if (!*p++) {
                m_Error = SAT_PATTERN_ESC;
                return false;
              }
            }
            else  p++;
            // if end of pattern here then bad pattern
            if (!*p) {
              m_Error = SAT_PATTERN_CLOSE;
              return false;
            }
            // if this a range
            if (*p=='-') {
              // we must have an end of range
              if (!*++p || *p==']') {
                m_Error = SAT_PATTERN_RANGE;
                return false;
              }
              else {
                // check for literal escape
                if (*p == '\\') p++;
                // if end of pattern here, then bad pattern
                if (!*p++) {
                  m_Error = SAT_PATTERN_ESC;
                  return false;
                }
              }
            }
          } // while
          break;
        // all other characters are valid pattern elements
        case '*':
        case '?':
        default:
          p++;
          break;
      }
    }
    return true;
  }

  //----------

  int _match_after_star(char *p, char *t) {
    int mtch = 0;
    char nextp; // ccernn: added 'char'
    //* pass over existing ? and * in pattern
    while ( *p == '?' || *p == '*' ) {
      // take one char for each ? and +
      if (*p == '?') {
        // if end of text then no match
        if (!*t++) return SAT_MATCH_ABORT;
      }
      // move to next char in pattern
      p++;
    }
    // if end of pattern we have matched regardless of text left
    if (!*p) return SAT_MATCH_VALID;
    // get the next character to match which must be a literal or '['
    nextp = *p;
    if (nextp == '\\') {
      nextp = p[1];
      // if end of text then we have a bad pattern
      if (!nextp) return SAT_MATCH_PATTERN;
    }
    // Continue until we run out of text or definite result seen
    do {
      // a precondition for matching is that the next character
      // in the pattern match the next character in the text or that
      // the next pattern char is the beginning of a range.  Increment
      // text pointer as we go here
      if (nextp == *t || nextp == '[') mtch = _match(p, t);
      // if the end of text is reached then no match
      if (!*t++) mtch = SAT_MATCH_ABORT;
    } while ( mtch != SAT_MATCH_VALID &&
              mtch != SAT_MATCH_ABORT &&
              mtch != SAT_MATCH_PATTERN);
    // return result
    return mtch;
  }

  //----------

  int _match(char *p, char *t) {
    char range_start, range_end;  // start and end in range
    bool invert;                  // is this [..] or [!..]
    bool member_match;            // have I matched the [..] construct?
    bool loop;                    // should I terminate?
    for (; *p; p++,t++)
    {
      // if this is the end of the text, then this is the end of the match
      if (!*t) {
        return ( *p=='*' && *++p =='\0' ) ? SAT_MATCH_VALID : SAT_MATCH_ABORT;
      }
      // determine and react to pattern type
      switch (*p) {
        // single any character match
        case '?':
          break;
        // multiple any character match
        case '*':
          return _match_after_star(p,t);
        // [..] construct, single member/exclusion character match
        case '[':
          {
          // move to beginning of range
          p++;
          // check if this is a member match or exclusion match
          invert = false;
          if (*p =='!' || *p =='^') {
            invert = true;
            p++;
          }
          // if closing bracket here or at range start
          // then we have a malformed pattern
          if (*p==']') { return SAT_MATCH_PATTERN; }
          member_match = false;
          loop = true;
          while (loop) {
            // if end of construct then loop is done
            if (*p == ']') {
              loop = false;
              continue;
            }
            // matching a '!', '^', '-', '\' or a ']'
            if (*p=='\\') {
              range_start = range_end = *++p;
            }
            else range_start = range_end = *p;
            // if end of pattern then bad pattern (Missing ']')
            if (!*p) return SAT_MATCH_PATTERN;
            // check for range bar
            if (*++p=='-') {
              // get the range end
              range_end = *++p;
              // if end of pattern or construct then bad pattern
              if (range_end=='\0' || range_end==']') return SAT_MATCH_PATTERN;
              // special character range end
              if (range_end=='\\') {
                range_end = *++p;
                // if end of text then we have a bad pattern
                if (!range_end) return SAT_MATCH_PATTERN;
              }
              // move just beyond this range
              p++;
            }
            // if the text character is in range then match found.
            // make sure the range letters have the proper
            // relationship to one another before comparison
            if (range_start < range_end) {
              if (*t>=range_start && *t<=range_end) {
                member_match = true;
                loop = false;
              }
            }
            else {
              if (*t>=range_end && *t<=range_start) {
                member_match = true;
                loop = false;
              }
            }
          }
          // if there was a match in an exclusion set then no match
          // if there was no match in a member set then no match
          if ((invert && member_match) || !(invert || member_match))
            return SAT_MATCH_RANGE;
          // if this is not an exclusion then skip the rest of
          // the [...] construct that already matched.
          if (member_match) {
            while (*p != ']') {
              // bad pattern (Missing ']')
              if (!*p) return SAT_MATCH_PATTERN;
              // skip exact match
              if (*p == '\\') {
                p++;
                // if end of text then we have a bad pattern
                if (!*p) return SAT_MATCH_PATTERN;
              }
              // move to next pattern char
              p++;
            }
          }
          break;
          } // ']'
        case '\\':
          // next character is quoted and must match exactly
          // move pattern pointer to quoted char and fall through
          p++;
          // if end of text then we have a bad pattern
          if (!*p) return SAT_MATCH_PATTERN;
          // must match this character exactly
        default:
          if (*p != *t) return SAT_MATCH_LITERAL;
      } // switch (*p)
    }
    // if end of text not reached then the pattern fails
    if (*t) return SAT_MATCH_END;
    else  return SAT_MATCH_VALID;
  }

};

//----------------------------------------------------------------------
#endif

// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_I18N_WORD_ITERATOR_H_
#define BASE_I18N_WORD_ITERATOR_H_
#pragma once

#include <vector>

#include "unicode/ubrk.h"
#include "unicode/uchar.h"

#include "base/basictypes.h"
#include "base/string16.h"

// The WordIterator class iterates through the words and word breaks
// in a string.  (In the string " foo bar! ", the word breaks are at the
// periods in ". .foo. .bar.!. .".)
//
// To extract the words from a string, move a WordIterator through the
// string and test whether IsWord() is true.  E.g.,
//   WordIterator iter(&str, WordIterator::BREAK_WORD);
//   if (!iter.Init()) return false;
//   while (iter.Advance()) {
//     if (iter.IsWord()) {
//       // region [iter.prev(),iter.pos()) contains a word.
//       VLOG(1) << "word: " << iter.GetWord();
//     }
//   }


class WordIterator {
 public:
  enum BreakType {
    BREAK_WORD,
    BREAK_LINE
  };

  // Requires |str| to live as long as the WordIterator does.
  WordIterator(const string16* str, BreakType break_type);
  ~WordIterator();

  // Init() must be called before any of the iterators are valid.
  // Returns false if ICU failed to initialize.
  bool Init();

  // Return the current break position within the string,
  // or WordIterator::npos when done.
  size_t pos() const { return pos_; }
  // Return the value of pos() returned before Advance() was last called.
  size_t prev() const { return prev_; }

  // Advance to the next break.  Returns false if we've run past the end of
  // the string.  (Note that the very last "word break" is after the final
  // character in the string, and when we advance to that position it's the
  // last time Advance() returns true.)
  bool Advance();

  // Returns true if the break we just hit is the end of a word.
  // (Otherwise, the break iterator just skipped over e.g. whitespace
  // or punctuation.)
  bool IsWord() const;

  // Return the word between prev() and pos().
  // Advance() must have been called successfully at least once
  // for pos() to have advanced to somewhere useful.
  string16 GetWord() const;

 private:
  // ICU iterator.
  UBreakIterator* iter_;
#if !defined(WCHAR_T_IS_UTF16)
  std::vector<UChar> chars_;
#endif

  // The string we're iterating over.
  const string16* string_;

  // The breaking style (word/line).
  BreakType break_type_;

  // Previous and current iterator positions.
  size_t prev_, pos_;

  DISALLOW_COPY_AND_ASSIGN(WordIterator);
};

#endif  // BASE_I18N_WORD_ITERATOR_H__

// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file specifies a recursive data storage class called Value intended for
// storing setting and other persistable data.  It includes the ability to
// specify (recursive) lists and dictionaries, so it's fairly expressive.
// However, the API is optimized for the common case, namely storing a
// hierarchical tree of simple values.  Given a DictionaryValue root, you can
// easily do things like:
//
// root->SetString("global.pages.homepage", "http://goateleporter.com");
// std::string homepage = "http://google.com";  // default/fallback value
// root->GetString("global.pages.homepage", &homepage);
//
// where "global" and "pages" are also DictionaryValues, and "homepage" is a
// string setting.  If some elements of the path didn't exist yet, the
// SetString() method would create the missing elements and attach them to root
// before attaching the homepage value.

#ifndef BASE_VALUES_H_
#define BASE_VALUES_H_
#pragma once

#include <iterator>
#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/string16.h"
#include "build/build_config.h"

class Value;
class FundamentalValue;
class StringValue;
class BinaryValue;
class DictionaryValue;
class ListValue;

typedef std::vector<Value*> ValueVector;
typedef std::map<std::string, Value*> ValueMap;

// The Value class is the base class for Values.  A Value can be
// instantiated via the Create*Value() factory methods, or by directly
// creating instances of the subclasses.
class Value {
 public:
  virtual ~Value();

  // Convenience methods for creating Value objects for various
  // kinds of values without thinking about which class implements them.
  // These can always be expected to return a valid Value*.
  static Value* CreateNullValue();
  static Value* CreateBooleanValue(bool in_value);
  static Value* CreateIntegerValue(int in_value);
  static Value* CreateRealValue(double in_value);
  static Value* CreateStringValue(const std::string& in_value);
  static Value* CreateStringValue(const string16& in_value);

  // This one can return NULL if the input isn't valid.  If the return value
  // is non-null, the new object has taken ownership of the buffer pointer.
  static BinaryValue* CreateBinaryValue(char* buffer, size_t size);

  typedef enum {
    TYPE_NULL = 0,
    TYPE_BOOLEAN,
    TYPE_INTEGER,
    TYPE_REAL,
    TYPE_STRING,
    TYPE_BINARY,
    TYPE_DICTIONARY,
    TYPE_LIST
  } ValueType;

  // Returns the type of the value stored by the current Value object.
  // Each type will be implemented by only one subclass of Value, so it's
  // safe to use the ValueType to determine whether you can cast from
  // Value* to (Implementing Class)*.  Also, a Value object never changes
  // its type after construction.
  ValueType GetType() const { return type_; }

  // Returns true if the current object represents a given type.
  bool IsType(ValueType type) const { return type == type_; }

  // These methods allow the convenient retrieval of settings.
  // If the current setting object can be converted into the given type,
  // the value is returned through the |out_value| parameter and true is
  // returned;  otherwise, false is returned and |out_value| is unchanged.
  virtual bool GetAsBoolean(bool* out_value) const;
  virtual bool GetAsInteger(int* out_value) const;
  virtual bool GetAsReal(double* out_value) const;
  virtual bool GetAsString(std::string* out_value) const;
  virtual bool GetAsString(string16* out_value) const;

  // This creates a deep copy of the entire Value tree, and returns a pointer
  // to the copy.  The caller gets ownership of the copy, of course.
  virtual Value* DeepCopy() const;

  // Compares if two Value objects have equal contents.
  virtual bool Equals(const Value* other) const;

 protected:
  // This isn't safe for end-users (they should use the Create*Value()
  // static methods above), but it's useful for subclasses.
  explicit Value(ValueType type);

 private:
  Value();

  ValueType type_;

  DISALLOW_COPY_AND_ASSIGN(Value);
};

// FundamentalValue represents the simple fundamental types of values.
class FundamentalValue : public Value {
 public:
  explicit FundamentalValue(bool in_value);
  explicit FundamentalValue(int in_value);
  explicit FundamentalValue(double in_value);
  ~FundamentalValue();

  // Subclassed methods
  virtual bool GetAsBoolean(bool* out_value) const;
  virtual bool GetAsInteger(int* out_value) const;
  virtual bool GetAsReal(double* out_value) const;
  virtual Value* DeepCopy() const;
  virtual bool Equals(const Value* other) const;

 private:
  union {
    bool boolean_value_;
    int integer_value_;
    double real_value_;
  };

  DISALLOW_COPY_AND_ASSIGN(FundamentalValue);
};

class StringValue : public Value {
 public:
  // Initializes a StringValue with a UTF-8 narrow character string.
  explicit StringValue(const std::string& in_value);

  // Initializes a StringValue with a string16.
  explicit StringValue(const string16& in_value);

  ~StringValue();

  // Subclassed methods
  bool GetAsString(std::string* out_value) const;
  bool GetAsString(string16* out_value) const;
  Value* DeepCopy() const;
  virtual bool Equals(const Value* other) const;

 private:
  std::string value_;

  DISALLOW_COPY_AND_ASSIGN(StringValue);
};

class BinaryValue: public Value {
 public:
  // Creates a Value to represent a binary buffer.  The new object takes
  // ownership of the pointer passed in, if successful.
  // Returns NULL if buffer is NULL.
  static BinaryValue* Create(char* buffer, size_t size);

  // For situations where you want to keep ownership of your buffer, this
  // factory method creates a new BinaryValue by copying the contents of the
  // buffer that's passed in.
  // Returns NULL if buffer is NULL.
  static BinaryValue* CreateWithCopiedBuffer(const char* buffer, size_t size);

  ~BinaryValue();

  // Subclassed methods
  Value* DeepCopy() const;
  virtual bool Equals(const Value* other) const;

  size_t GetSize() const { return size_; }
  char* GetBuffer() { return buffer_; }
  const char* GetBuffer() const { return buffer_; }

 private:
  // Constructor is private so that only objects with valid buffer pointers
  // and size values can be created.
  BinaryValue(char* buffer, size_t size);

  char* buffer_;
  size_t size_;

  DISALLOW_COPY_AND_ASSIGN(BinaryValue);
};

// DictionaryValue provides a key-value dictionary with (optional) "path"
// parsing for recursive access; see the comment at the top of the file. Keys
// are |std::string|s and should be UTF-8 encoded.
class DictionaryValue : public Value {
 public:
  DictionaryValue();
  ~DictionaryValue();

  // Subclassed methods
  Value* DeepCopy() const;
  virtual bool Equals(const Value* other) const;

  // Returns true if the current dictionary has a value for the given key.
  bool HasKey(const std::string& key) const;

  // Returns the number of Values in this dictionary.
  size_t size() const { return dictionary_.size(); }

  // Returns whether the dictionary is empty.
  bool empty() const { return dictionary_.empty(); }

  // Clears any current contents of this dictionary.
  void Clear();

  // Sets the Value associated with the given path starting from this object.
  // A path has the form "<key>" or "<key>.<key>.[...]", where "." indexes
  // into the next DictionaryValue down.  Obviously, "." can't be used
  // within a key, but there are no other restrictions on keys.
  // If the key at any step of the way doesn't exist, or exists but isn't
  // a DictionaryValue, a new DictionaryValue will be created and attached
  // to the path in that location.
  // Note that the dictionary takes ownership of the value referenced by
  // |in_value|, and therefore |in_value| must be non-NULL.
  void Set(const std::string& path, Value* in_value);

  // Convenience forms of Set().  These methods will replace any existing
  // value at that path, even if it has a different type.
  void SetBoolean(const std::string& path, bool in_value);
  void SetInteger(const std::string& path, int in_value);
  void SetReal(const std::string& path, double in_value);
  void SetString(const std::string& path, const std::string& in_value);
  void SetString(const std::string& path, const string16& in_value);

  // Like Set(), but without special treatment of '.'.  This allows e.g. URLs to
  // be used as paths.
  void SetWithoutPathExpansion(const std::string& key, Value* in_value);

  // Gets the Value associated with the given path starting from this object.
  // A path has the form "<key>" or "<key>.<key>.[...]", where "." indexes
  // into the next DictionaryValue down.  If the path can be resolved
  // successfully, the value for the last key in the path will be returned
  // through the |out_value| parameter, and the function will return true.
  // Otherwise, it will return false and |out_value| will be untouched.
  // Note that the dictionary always owns the value that's returned.
  bool Get(const std::string& path, Value** out_value) const;

  // These are convenience forms of Get().  The value will be retrieved
  // and the return value will be true if the path is valid and the value at
  // the end of the path can be returned in the form specified.
  bool GetBoolean(const std::string& path, bool* out_value) const;
  bool GetInteger(const std::string& path, int* out_value) const;
  bool GetReal(const std::string& path, double* out_value) const;
  bool GetString(const std::string& path, std::string* out_value) const;
  bool GetString(const std::string& path, string16* out_value) const;
  bool GetStringASCII(const std::string& path, std::string* out_value) const;
  bool GetBinary(const std::string& path, BinaryValue** out_value) const;
  bool GetDictionary(const std::string& path,
                     DictionaryValue** out_value) const;
  bool GetList(const std::string& path, ListValue** out_value) const;

  // Like Get(), but without special treatment of '.'.  This allows e.g. URLs to
  // be used as paths.
  bool GetWithoutPathExpansion(const std::string& key,
                               Value** out_value) const;
  bool GetIntegerWithoutPathExpansion(const std::string& key,
                                      int* out_value) const;
  bool GetRealWithoutPathExpansion(const std::string& key,
                                   double* out_value) const;
  bool GetStringWithoutPathExpansion(const std::string& key,
                                     std::string* out_value) const;
  bool GetStringWithoutPathExpansion(const std::string& key,
                                     string16* out_value) const;
  bool GetDictionaryWithoutPathExpansion(const std::string& key,
                                         DictionaryValue** out_value) const;
  bool GetListWithoutPathExpansion(const std::string& key,
                                   ListValue** out_value) const;

  // Removes the Value with the specified path from this dictionary (or one
  // of its child dictionaries, if the path is more than just a local key).
  // If |out_value| is non-NULL, the removed Value AND ITS OWNERSHIP will be
  // passed out via out_value.  If |out_value| is NULL, the removed value will
  // be deleted.  This method returns true if |path| is a valid path; otherwise
  // it will return false and the DictionaryValue object will be unchanged.
  bool Remove(const std::string& path, Value** out_value);

  // Like Remove(), but without special treatment of '.'.  This allows e.g. URLs
  // to be used as paths.
  bool RemoveWithoutPathExpansion(const std::string& key, Value** out_value);

  // Makes a copy of |this| but doesn't include empty dictionaries and lists in
  // the copy.  This never returns NULL, even if |this| itself is empty.
  DictionaryValue* DeepCopyWithoutEmptyChildren();

  // Merge a given dictionary into this dictionary. This is done recursively,
  // i.e. any subdictionaries will be merged as well. In case of key collisions,
  // the passed in dictionary takes precedence and data already present will be
  // replaced.
  void MergeDictionary(const DictionaryValue* dictionary);

  // Builds a vector containing all of the paths that are different between
  // the dictionary and a second specified dictionary. These are paths of
  // values that are either in one dictionary or the other but not both, OR
  // paths that are present in both dictionaries but differ in value.
  // Path strings are in ascending lexicographical order in the generated
  // vector. |different_paths| is cleared before added any paths.
  void GetDifferingPaths(
      const DictionaryValue* other,
      std::vector<std::string>* different_paths) const;

  // This class provides an iterator for the keys in the dictionary.
  // It can't be used to modify the dictionary.
  //
  // YOU SHOULD ALWAYS USE THE XXXWithoutPathExpansion() APIs WITH THESE, NOT
  // THE NORMAL XXX() APIs.  This makes sure things will work correctly if any
  // keys have '.'s in them.
  class key_iterator
      : private std::iterator<std::input_iterator_tag, const std::string> {
   public:
    explicit key_iterator(ValueMap::const_iterator itr) { itr_ = itr; }
    key_iterator operator++() {
      ++itr_;
      return *this;
    }
    const std::string& operator*() { return itr_->first; }
    bool operator!=(const key_iterator& other) { return itr_ != other.itr_; }
    bool operator==(const key_iterator& other) { return itr_ == other.itr_; }

   private:
    ValueMap::const_iterator itr_;
  };

  key_iterator begin_keys() const { return key_iterator(dictionary_.begin()); }
  key_iterator end_keys() const { return key_iterator(dictionary_.end()); }

 private:
  // Does the actual heavy lifting for GetDifferingPaths.
  // Returns true if a path is added to different_paths, otherwise false.
  // The difference compuation is calculated recursively. The keys for
  // dictionaries that are handled by recursive calls more shallow than
  // the current one are concatenated and passed through to deeper calls in
  // |path_prefix|.
  bool GetDifferingPathsHelper(
      const std::string& path_prefix,
      const DictionaryValue* other,
      std::vector<std::string>* different_paths) const;

  ValueMap dictionary_;

  DISALLOW_COPY_AND_ASSIGN(DictionaryValue);
};

// This type of Value represents a list of other Value values.
class ListValue : public Value {
 public:
  ListValue();
  ~ListValue();

  // Subclassed methods
  Value* DeepCopy() const;
  virtual bool Equals(const Value* other) const;

  // Clears the contents of this ListValue
  void Clear();

  // Returns the number of Values in this list.
  size_t GetSize() const { return list_.size(); }

  // Returns whether the list is empty.
  bool empty() const { return list_.empty(); }

  // Sets the list item at the given index to be the Value specified by
  // the value given.  If the index beyond the current end of the list, null
  // Values will be used to pad out the list.
  // Returns true if successful, or false if the index was negative or
  // the value is a null pointer.
  bool Set(size_t index, Value* in_value);

  // Gets the Value at the given index.  Modifies |out_value| (and returns true)
  // only if the index falls within the current list range.
  // Note that the list always owns the Value passed out via |out_value|.
  bool Get(size_t index, Value** out_value) const;

  // Convenience forms of Get().  Modifies |out_value| (and returns true)
  // only if the index is valid and the Value at that index can be returned
  // in the specified form.
  bool GetBoolean(size_t index, bool* out_value) const;
  bool GetInteger(size_t index, int* out_value) const;
  bool GetReal(size_t index, double* out_value) const;
  bool GetString(size_t index, std::string* out_value) const;
  bool GetString(size_t index, string16* out_value) const;
  bool GetBinary(size_t index, BinaryValue** out_value) const;
  bool GetDictionary(size_t index, DictionaryValue** out_value) const;
  bool GetList(size_t index, ListValue** out_value) const;

  // Removes the Value with the specified index from this list.
  // If |out_value| is non-NULL, the removed Value AND ITS OWNERSHIP will be
  // passed out via |out_value|.  If |out_value| is NULL, the removed value will
  // be deleted.  This method returns true if |index| is valid; otherwise
  // it will return false and the ListValue object will be unchanged.
  bool Remove(size_t index, Value** out_value);

  // Removes the first instance of |value| found in the list, if any, and
  // deletes it.  Returns the index that it was located at (-1 for not present).
  int Remove(const Value& value);

  // Appends a Value to the end of the list.
  void Append(Value* in_value);

  // Appends a Value if it's not already present.
  // Returns true if successful, or false if the value was already present.
  bool AppendIfNotPresent(Value* in_value);

  // Insert a Value at index.
  // Returns true if successful, or false if the index was out of range.
  bool Insert(size_t index, Value* in_value);

  // Swaps contents with the |other| list.
  void Swap(ListValue* other) {
    list_.swap(other->list_);
  }

  // Iteration
  typedef ValueVector::iterator iterator;
  typedef ValueVector::const_iterator const_iterator;

  ListValue::iterator begin() { return list_.begin(); }
  ListValue::iterator end() { return list_.end(); }

  ListValue::const_iterator begin() const { return list_.begin(); }
  ListValue::const_iterator end() const { return list_.end(); }

 private:
  ValueVector list_;

  DISALLOW_COPY_AND_ASSIGN(ListValue);
};

// This interface is implemented by classes that know how to serialize and
// deserialize Value objects.
class ValueSerializer {
 public:
  virtual ~ValueSerializer();

  virtual bool Serialize(const Value& root) = 0;

  // This method deserializes the subclass-specific format into a Value object.
  // If the return value is non-NULL, the caller takes ownership of returned
  // Value. If the return value is NULL, and if error_code is non-NULL,
  // error_code will be set with the underlying error.
  // If |error_message| is non-null, it will be filled in with a formatted
  // error message including the location of the error if appropriate.
  virtual Value* Deserialize(int* error_code, std::string* error_str) = 0;
};

#endif  // BASE_VALUES_H_

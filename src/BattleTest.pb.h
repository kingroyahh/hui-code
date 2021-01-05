// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: BattleTest.proto

#ifndef PROTOBUF_BattleTest_2eproto__INCLUDED
#define PROTOBUF_BattleTest_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
// @@protoc_insertion_point(includes)

namespace pb {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_BattleTest_2eproto();
void protobuf_AssignDesc_BattleTest_2eproto();
void protobuf_ShutdownFile_BattleTest_2eproto();

class BattleTest;

// ===================================================================

/*FOR2LUA
<Record>
	<name>BattleTest</name>
	<Function>
		<name>new</name>
		<replaceName>BattleTest.proto</replaceName>
		<handler>descripter = pb_new_cpp()</handler>
	</Function>
	<Function>
		<name>ParsePartialFromArray</name>
		<replaceName>ParseFromString</replaceName>
		<handler>descripter = pb_decode_cpp()</handler>
	</Function>
	<Function>
		<name>SerializePartialToArray</name>
		<replaceName>SerializeToString</replaceName>
		<handler>descripter = pb_encode_cpp()</handler>
	</Function>
</Record>
FOR2LUA*/
class BattleTest : public ::google::protobuf::MessageLite {
 public:
  BattleTest();
  virtual ~BattleTest();

  BattleTest(const BattleTest& from);

  inline BattleTest& operator=(const BattleTest& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::std::string& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::std::string* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const BattleTest& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const BattleTest* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(BattleTest* other);

  // implements Message ----------------------------------------------

  BattleTest* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const BattleTest& from);
  void MergeFrom(const BattleTest& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  void DiscardUnknownFields();
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional float test_value = 1;
  inline bool has_test_value() const;
  inline void clear_test_value();
  static const int kTestValueFieldNumber = 1;
  /*FOR2LUA
  <Record>
  	<name>BattleTest</name>
  	<Function>
  		<name>test_value</name>
  		<replaceName>test_value</replaceName>
  		<handler>descripter = pb_cpp()</handler>
  	</Function>
  	<Function>
  		<name>set_test_value</name>
  		<replaceName>test_value</replaceName>
  		<handler>descripter = pb_set_cpp()</handler>
  	</Function>
  	<Function>
  		<name>has_test_value</name>
  		<replaceName>test_value</replaceName>
  		<handler>descripter = pb_has_cpp()</handler>
  	</Function>
  </Record>
  FOR2LUA*/
  inline float test_value() const;
  inline void set_test_value(float value);

  // @@protoc_insertion_point(class_scope:pb.BattleTest)
 private:
  inline void set_has_test_value();
  inline void clear_has_test_value();

  ::std::string _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  float test_value_;
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_BattleTest_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_BattleTest_2eproto();
  #endif
  friend void protobuf_AssignDesc_BattleTest_2eproto();
  friend void protobuf_ShutdownFile_BattleTest_2eproto();

  void InitAsDefaultInstance();
  static BattleTest* default_instance_;
};
// ===================================================================


// ===================================================================

// BattleTest

// optional float test_value = 1;
inline bool BattleTest::has_test_value() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void BattleTest::set_has_test_value() {
  _has_bits_[0] |= 0x00000001u;
}
inline void BattleTest::clear_has_test_value() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void BattleTest::clear_test_value() {
  test_value_ = 0;
  clear_has_test_value();
}
inline float BattleTest::test_value() const {
  // @@protoc_insertion_point(field_get:pb.BattleTest.test_value)
  return test_value_;
}
inline void BattleTest::set_test_value(float value) {
  set_has_test_value();
  test_value_ = value;
  // @@protoc_insertion_point(field_set:pb.BattleTest.test_value)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace pb

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_BattleTest_2eproto__INCLUDED

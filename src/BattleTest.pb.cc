// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: BattleTest.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "BattleTest.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
// @@protoc_insertion_point(includes)

namespace pb {

void protobuf_ShutdownFile_BattleTest_2eproto() {
  delete BattleTest::default_instance_;
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
void protobuf_AddDesc_BattleTest_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#else
void protobuf_AddDesc_BattleTest_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#endif
  BattleTest::default_instance_ = new BattleTest();
  BattleTest::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_BattleTest_2eproto);
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_BattleTest_2eproto_once_);
void protobuf_AddDesc_BattleTest_2eproto() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_BattleTest_2eproto_once_,
                 &protobuf_AddDesc_BattleTest_2eproto_impl);
}
#else
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_BattleTest_2eproto {
  StaticDescriptorInitializer_BattleTest_2eproto() {
    protobuf_AddDesc_BattleTest_2eproto();
  }
} static_descriptor_initializer_BattleTest_2eproto_;
#endif

// ===================================================================

#ifndef _MSC_VER
const int BattleTest::kTestValueFieldNumber;
#endif  // !_MSC_VER

BattleTest::BattleTest()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:pb.BattleTest)
}

void BattleTest::InitAsDefaultInstance() {
}

BattleTest::BattleTest(const BattleTest& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:pb.BattleTest)
}

void BattleTest::SharedCtor() {
  _cached_size_ = 0;
  test_value_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

BattleTest::~BattleTest() {
  // @@protoc_insertion_point(destructor:pb.BattleTest)
  SharedDtor();
}

void BattleTest::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void BattleTest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const BattleTest& BattleTest::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_BattleTest_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_BattleTest_2eproto();
#endif
  return *default_instance_;
}

BattleTest* BattleTest::default_instance_ = NULL;

BattleTest* BattleTest::New() const {
  return new BattleTest;
}

void BattleTest::Clear() {
  test_value_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->clear();
}

bool BattleTest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::io::StringOutputStream unknown_fields_string(
      mutable_unknown_fields());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_string);
  // @@protoc_insertion_point(parse_start:pb.BattleTest)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional float test_value = 1;
      case 1: {
        if (tag == 13) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &test_value_)));
          set_has_test_value();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(
            input, tag, &unknown_fields_stream));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:pb.BattleTest)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:pb.BattleTest)
  return false;
#undef DO_
}

void BattleTest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:pb.BattleTest)
  // optional float test_value = 1;
  if (has_test_value()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(1, this->test_value(), output);
  }

  output->WriteRaw(unknown_fields().data(),
                   unknown_fields().size());
  // @@protoc_insertion_point(serialize_end:pb.BattleTest)
}

int BattleTest::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional float test_value = 1;
    if (has_test_value()) {
      total_size += 1 + 4;
    }

  }
  total_size += unknown_fields().size();

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void BattleTest::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const BattleTest*>(&from));
}

void BattleTest::MergeFrom(const BattleTest& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_test_value()) {
      set_test_value(from.test_value());
    }
  }
  mutable_unknown_fields()->append(from.unknown_fields());
}

void BattleTest::CopyFrom(const BattleTest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool BattleTest::IsInitialized() const {

  return true;
}

void BattleTest::Swap(BattleTest* other) {
  if (other != this) {
    std::swap(test_value_, other->test_value_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.swap(other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string BattleTest::GetTypeName() const {
  return "pb.BattleTest";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace pb

// @@protoc_insertion_point(global_scope)

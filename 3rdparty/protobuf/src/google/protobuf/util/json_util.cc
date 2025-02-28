// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <google/protobuf/util/json_util.h>

#include <google/protobuf/stubs/common.h>
#include "absl/base/call_once.h"
#include "absl/status/status.h"
#include <google/protobuf/stubs/bytestream.h>
#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_sink.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/util/internal/default_value_objectwriter.h>
#include <google/protobuf/util/internal/error_listener.h>
#include <google/protobuf/util/internal/json_objectwriter.h>
#include <google/protobuf/util/internal/json_stream_parser.h>
#include <google/protobuf/util/internal/protostream_objectsource.h>
#include <google/protobuf/util/internal/protostream_objectwriter.h>
#include <google/protobuf/util/type_resolver.h>
#include <google/protobuf/util/type_resolver_util.h>
#include <google/protobuf/stubs/status_macros.h>


// clang-format off
#include <google/protobuf/port_def.inc>
// clang-format on


namespace google {
namespace protobuf {
namespace util {
using ::google::protobuf::io::zc_sink_internal::ZeroCopyStreamByteSink;

absl::Status BinaryToJsonStream(TypeResolver* resolver,
                                const std::string& type_url,
                                io::ZeroCopyInputStream* binary_input,
                                io::ZeroCopyOutputStream* json_output,
                                const JsonPrintOptions& options) {

  io::CodedInputStream in_stream(binary_input);
  google::protobuf::Type type;
  RETURN_IF_ERROR(resolver->ResolveMessageType(type_url, &type));
  converter::ProtoStreamObjectSource::RenderOptions render_options;
  render_options.use_ints_for_enums = options.always_print_enums_as_ints;
  render_options.preserve_proto_field_names =
      options.preserve_proto_field_names;
  converter::ProtoStreamObjectSource proto_source(&in_stream, resolver, type,
                                                  render_options);
  io::CodedOutputStream out_stream(json_output);
  converter::JsonObjectWriter json_writer(options.add_whitespace ? " " : "",
                                          &out_stream);
  if (options.always_print_primitive_fields) {
    converter::DefaultValueObjectWriter default_value_writer(resolver, type,
                                                             &json_writer);
    default_value_writer.set_preserve_proto_field_names(
        options.preserve_proto_field_names);
    default_value_writer.set_print_enums_as_ints(
        options.always_print_enums_as_ints);
    return proto_source.WriteTo(&default_value_writer);
  } else {
    return proto_source.WriteTo(&json_writer);
  }
}

absl::Status BinaryToJsonString(TypeResolver* resolver,
                                const std::string& type_url,
                                const std::string& binary_input,
                                std::string* json_output,
                                const JsonPrintOptions& options) {
  io::ArrayInputStream input_stream(binary_input.data(), binary_input.size());
  io::StringOutputStream output_stream(json_output);
  return BinaryToJsonStream(resolver, type_url, &input_stream, &output_stream,
                            options);
}

namespace {
class StatusErrorListener : public converter::ErrorListener {
 public:
  StatusErrorListener() {}
  StatusErrorListener(const StatusErrorListener&) = delete;
  StatusErrorListener& operator=(const StatusErrorListener&) = delete;
  ~StatusErrorListener() override {}

  absl::Status GetStatus() { return status_; }

  void InvalidName(const converter::LocationTrackerInterface& loc,
                   absl::string_view unknown_name,
                   absl::string_view message) override {
    std::string loc_string = GetLocString(loc);
    if (!loc_string.empty()) {
      loc_string.append(" ");
    }
    status_ = absl::InvalidArgumentError(
        absl::StrCat(loc_string, unknown_name, ": ", message));
  }

  void InvalidValue(const converter::LocationTrackerInterface& loc,
                    absl::string_view type_name,
                    absl::string_view value) override {
    status_ = absl::InvalidArgumentError(
        absl::StrCat(GetLocString(loc), ": invalid value ", std::string(value),
                     " for type ", std::string(type_name)));
  }

  void MissingField(const converter::LocationTrackerInterface& loc,
                    absl::string_view missing_name) override {
    status_ = absl::InvalidArgumentError(absl::StrCat(
        GetLocString(loc), ": missing field ", std::string(missing_name)));
  }

 private:
  absl::Status status_;

  std::string GetLocString(const converter::LocationTrackerInterface& loc) {
    std::string loc_string = loc.ToString();
    StripWhitespace(&loc_string);
    if (!loc_string.empty()) {
      loc_string = absl::StrCat("(", loc_string, ")");
    }
    return loc_string;
  }
};
}  // namespace

absl::Status JsonToBinaryStream(TypeResolver* resolver,
                                const std::string& type_url,
                                io::ZeroCopyInputStream* json_input,
                                io::ZeroCopyOutputStream* binary_output,
                                const JsonParseOptions& options) {

  google::protobuf::Type type;
  RETURN_IF_ERROR(resolver->ResolveMessageType(type_url, &type));
  ZeroCopyStreamByteSink sink(binary_output);
  StatusErrorListener listener;
  converter::ProtoStreamObjectWriter::Options proto_writer_options;
  proto_writer_options.ignore_unknown_fields = options.ignore_unknown_fields;
  proto_writer_options.ignore_unknown_enum_values =
      options.ignore_unknown_fields;
  proto_writer_options.case_insensitive_enum_parsing =
      options.case_insensitive_enum_parsing;
  converter::ProtoStreamObjectWriter proto_writer(
      resolver, type, &sink, &listener, proto_writer_options);
  proto_writer.set_use_strict_base64_decoding(false);

  converter::JsonStreamParser parser(&proto_writer);
  const void* buffer;
  int length;
  while (json_input->Next(&buffer, &length)) {
    if (length == 0) continue;
    RETURN_IF_ERROR(parser.Parse(
        absl::string_view(static_cast<const char*>(buffer), length)));
  }
  RETURN_IF_ERROR(parser.FinishParse());

  return listener.GetStatus();
}

absl::Status JsonToBinaryString(TypeResolver* resolver,
                                const std::string& type_url,
                                absl::string_view json_input,
                                std::string* binary_output,
                                const JsonParseOptions& options) {
  io::ArrayInputStream input_stream(json_input.data(), json_input.size());
  io::StringOutputStream output_stream(binary_output);
  return JsonToBinaryStream(resolver, type_url, &input_stream, &output_stream,
                            options);
}

namespace {
const char* kTypeUrlPrefix = "type.googleapis.com";
TypeResolver* generated_type_resolver_ = nullptr;
absl::once_flag generated_type_resolver_init_;

std::string GetTypeUrl(const Message& message) {
  return std::string(kTypeUrlPrefix) + "/" +
         message.GetDescriptor()->full_name();
}

void DeleteGeneratedTypeResolver() {  // NOLINT
  delete generated_type_resolver_;
}

void InitGeneratedTypeResolver() {
  generated_type_resolver_ = NewTypeResolverForDescriptorPool(
      kTypeUrlPrefix, DescriptorPool::generated_pool());
  ::google::protobuf::internal::OnShutdown(&DeleteGeneratedTypeResolver);
}

TypeResolver* GetGeneratedTypeResolver() {
  absl::call_once(generated_type_resolver_init_, InitGeneratedTypeResolver);
  return generated_type_resolver_;
}
}  // namespace

absl::Status MessageToJsonString(const Message& message, std::string* output,
                                 const JsonOptions& options) {

  const DescriptorPool* pool = message.GetDescriptor()->file()->pool();
  TypeResolver* resolver =
      pool == DescriptorPool::generated_pool()
          ? GetGeneratedTypeResolver()
          : NewTypeResolverForDescriptorPool(kTypeUrlPrefix, pool);
  absl::Status result =
      BinaryToJsonString(resolver, GetTypeUrl(message),
                         message.SerializeAsString(), output, options);
  if (pool != DescriptorPool::generated_pool()) {
    delete resolver;
  }
  return result;
}

absl::Status JsonStringToMessage(absl::string_view input, Message* message,
                                 const JsonParseOptions& options) {

  const DescriptorPool* pool = message->GetDescriptor()->file()->pool();
  TypeResolver* resolver =
      pool == DescriptorPool::generated_pool()
          ? GetGeneratedTypeResolver()
          : NewTypeResolverForDescriptorPool(kTypeUrlPrefix, pool);
  std::string binary;
  absl::Status result = JsonToBinaryString(resolver, GetTypeUrl(*message),
                                           input, &binary, options);
  if (result.ok() && !message->ParseFromString(binary)) {
    result = absl::InvalidArgumentError(
        "JSON transcoder produced invalid protobuf output.");
  }
  if (pool != DescriptorPool::generated_pool()) {
    delete resolver;
  }
  return result;
}

}  // namespace util
}  // namespace protobuf
}  // namespace google

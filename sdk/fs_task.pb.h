// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: fs_task.proto

#ifndef PROTOBUF_INCLUDED_fs_5ftask_2eproto
#define PROTOBUF_INCLUDED_fs_5ftask_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_fs_5ftask_2eproto 

namespace protobuf_fs_5ftask_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_fs_5ftask_2eproto
namespace fs {
namespace proto {
class Task;
class TaskDefaultTypeInternal;
extern TaskDefaultTypeInternal _Task_default_instance_;
}  // namespace proto
}  // namespace fs
namespace google {
namespace protobuf {
template<> ::fs::proto::Task* Arena::CreateMaybeMessage<::fs::proto::Task>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace fs {
namespace proto {

enum Task_TaskStatus {
  Task_TaskStatus_UPLOAD_INIT = 0,
  Task_TaskStatus_UPLOADING = 1,
  Task_TaskStatus_UPLOADED = 2,
  Task_TaskStatus_UPLOAD_PAUSED = 3,
  Task_TaskStatus_UPLOAD_PAUSING = 4,
  Task_TaskStatus_UPLOAD_RESUME = 5,
  Task_TaskStatus_DOWNLOAD_INIT = 6,
  Task_TaskStatus_DOWNLOADING = 7,
  Task_TaskStatus_DOWNLOADED = 8,
  Task_TaskStatus_DOWNLOAD_PAUSED = 9,
  Task_TaskStatus_DOWNLOAD_PAUSING = 10,
  Task_TaskStatus_DOWNLOAD_RESUME = 11,
  Task_TaskStatus_CANCELING = 12,
  Task_TaskStatus_CANCELED = 13,
  Task_TaskStatus_FAILED = 14,
  Task_TaskStatus_FAILING = 15
};
bool Task_TaskStatus_IsValid(int value);
const Task_TaskStatus Task_TaskStatus_TaskStatus_MIN = Task_TaskStatus_UPLOAD_INIT;
const Task_TaskStatus Task_TaskStatus_TaskStatus_MAX = Task_TaskStatus_FAILING;
const int Task_TaskStatus_TaskStatus_ARRAYSIZE = Task_TaskStatus_TaskStatus_MAX + 1;

const ::google::protobuf::EnumDescriptor* Task_TaskStatus_descriptor();
inline const ::std::string& Task_TaskStatus_Name(Task_TaskStatus value) {
  return ::google::protobuf::internal::NameOfEnum(
    Task_TaskStatus_descriptor(), value);
}
inline bool Task_TaskStatus_Parse(
    const ::std::string& name, Task_TaskStatus* value) {
  return ::google::protobuf::internal::ParseNamedEnum<Task_TaskStatus>(
    Task_TaskStatus_descriptor(), name, value);
}
// ===================================================================

class Task : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:fs.proto.Task) */ {
 public:
  Task();
  virtual ~Task();

  Task(const Task& from);

  inline Task& operator=(const Task& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Task(Task&& from) noexcept
    : Task() {
    *this = ::std::move(from);
  }

  inline Task& operator=(Task&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Task& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Task* internal_default_instance() {
    return reinterpret_cast<const Task*>(
               &_Task_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(Task* other);
  friend void swap(Task& a, Task& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Task* New() const final {
    return CreateMaybeMessage<Task>(NULL);
  }

  Task* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Task>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Task& from);
  void MergeFrom(const Task& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Task* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  typedef Task_TaskStatus TaskStatus;
  static const TaskStatus UPLOAD_INIT =
    Task_TaskStatus_UPLOAD_INIT;
  static const TaskStatus UPLOADING =
    Task_TaskStatus_UPLOADING;
  static const TaskStatus UPLOADED =
    Task_TaskStatus_UPLOADED;
  static const TaskStatus UPLOAD_PAUSED =
    Task_TaskStatus_UPLOAD_PAUSED;
  static const TaskStatus UPLOAD_PAUSING =
    Task_TaskStatus_UPLOAD_PAUSING;
  static const TaskStatus UPLOAD_RESUME =
    Task_TaskStatus_UPLOAD_RESUME;
  static const TaskStatus DOWNLOAD_INIT =
    Task_TaskStatus_DOWNLOAD_INIT;
  static const TaskStatus DOWNLOADING =
    Task_TaskStatus_DOWNLOADING;
  static const TaskStatus DOWNLOADED =
    Task_TaskStatus_DOWNLOADED;
  static const TaskStatus DOWNLOAD_PAUSED =
    Task_TaskStatus_DOWNLOAD_PAUSED;
  static const TaskStatus DOWNLOAD_PAUSING =
    Task_TaskStatus_DOWNLOAD_PAUSING;
  static const TaskStatus DOWNLOAD_RESUME =
    Task_TaskStatus_DOWNLOAD_RESUME;
  static const TaskStatus CANCELING =
    Task_TaskStatus_CANCELING;
  static const TaskStatus CANCELED =
    Task_TaskStatus_CANCELED;
  static const TaskStatus FAILED =
    Task_TaskStatus_FAILED;
  static const TaskStatus FAILING =
    Task_TaskStatus_FAILING;
  static inline bool TaskStatus_IsValid(int value) {
    return Task_TaskStatus_IsValid(value);
  }
  static const TaskStatus TaskStatus_MIN =
    Task_TaskStatus_TaskStatus_MIN;
  static const TaskStatus TaskStatus_MAX =
    Task_TaskStatus_TaskStatus_MAX;
  static const int TaskStatus_ARRAYSIZE =
    Task_TaskStatus_TaskStatus_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  TaskStatus_descriptor() {
    return Task_TaskStatus_descriptor();
  }
  static inline const ::std::string& TaskStatus_Name(TaskStatus value) {
    return Task_TaskStatus_Name(value);
  }
  static inline bool TaskStatus_Parse(const ::std::string& name,
      TaskStatus* value) {
    return Task_TaskStatus_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // optional string localbasepath = 2;
  bool has_localbasepath() const;
  void clear_localbasepath();
  static const int kLocalbasepathFieldNumber = 2;
  const ::std::string& localbasepath() const;
  void set_localbasepath(const ::std::string& value);
  #if LANG_CXX11
  void set_localbasepath(::std::string&& value);
  #endif
  void set_localbasepath(const char* value);
  void set_localbasepath(const char* value, size_t size);
  ::std::string* mutable_localbasepath();
  ::std::string* release_localbasepath();
  void set_allocated_localbasepath(::std::string* localbasepath);

  // required string remotebasepath = 3;
  bool has_remotebasepath() const;
  void clear_remotebasepath();
  static const int kRemotebasepathFieldNumber = 3;
  const ::std::string& remotebasepath() const;
  void set_remotebasepath(const ::std::string& value);
  #if LANG_CXX11
  void set_remotebasepath(::std::string&& value);
  #endif
  void set_remotebasepath(const char* value);
  void set_remotebasepath(const char* value, size_t size);
  ::std::string* mutable_remotebasepath();
  ::std::string* release_remotebasepath();
  void set_allocated_remotebasepath(::std::string* remotebasepath);

  // required string filename = 4;
  bool has_filename() const;
  void clear_filename();
  static const int kFilenameFieldNumber = 4;
  const ::std::string& filename() const;
  void set_filename(const ::std::string& value);
  #if LANG_CXX11
  void set_filename(::std::string&& value);
  #endif
  void set_filename(const char* value);
  void set_filename(const char* value, size_t size);
  ::std::string* mutable_filename();
  ::std::string* release_filename();
  void set_allocated_filename(::std::string* filename);

  // required uint64 task_id = 1;
  bool has_task_id() const;
  void clear_task_id();
  static const int kTaskIdFieldNumber = 1;
  ::google::protobuf::uint64 task_id() const;
  void set_task_id(::google::protobuf::uint64 value);

  // required uint64 total_packet_no = 5;
  bool has_total_packet_no() const;
  void clear_total_packet_no();
  static const int kTotalPacketNoFieldNumber = 5;
  ::google::protobuf::uint64 total_packet_no() const;
  void set_total_packet_no(::google::protobuf::uint64 value);

  // optional uint64 received_packet_no = 7;
  bool has_received_packet_no() const;
  void clear_received_packet_no();
  static const int kReceivedPacketNoFieldNumber = 7;
  ::google::protobuf::uint64 received_packet_no() const;
  void set_received_packet_no(::google::protobuf::uint64 value);

  // optional uint64 sent_packet_no = 8;
  bool has_sent_packet_no() const;
  void clear_sent_packet_no();
  static const int kSentPacketNoFieldNumber = 8;
  ::google::protobuf::uint64 sent_packet_no() const;
  void set_sent_packet_no(::google::protobuf::uint64 value);

  // optional uint64 last_packet_time = 9;
  bool has_last_packet_time() const;
  void clear_last_packet_time();
  static const int kLastPacketTimeFieldNumber = 9;
  ::google::protobuf::uint64 last_packet_time() const;
  void set_last_packet_time(::google::protobuf::uint64 value);

  // required .fs.proto.Task.TaskStatus task_status = 6;
  bool has_task_status() const;
  void clear_task_status();
  static const int kTaskStatusFieldNumber = 6;
  ::fs::proto::Task_TaskStatus task_status() const;
  void set_task_status(::fs::proto::Task_TaskStatus value);

  // @@protoc_insertion_point(class_scope:fs.proto.Task)
 private:
  void set_has_task_id();
  void clear_has_task_id();
  void set_has_localbasepath();
  void clear_has_localbasepath();
  void set_has_remotebasepath();
  void clear_has_remotebasepath();
  void set_has_filename();
  void clear_has_filename();
  void set_has_total_packet_no();
  void clear_has_total_packet_no();
  void set_has_task_status();
  void clear_has_task_status();
  void set_has_received_packet_no();
  void clear_has_received_packet_no();
  void set_has_sent_packet_no();
  void clear_has_sent_packet_no();
  void set_has_last_packet_time();
  void clear_has_last_packet_time();

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr localbasepath_;
  ::google::protobuf::internal::ArenaStringPtr remotebasepath_;
  ::google::protobuf::internal::ArenaStringPtr filename_;
  ::google::protobuf::uint64 task_id_;
  ::google::protobuf::uint64 total_packet_no_;
  ::google::protobuf::uint64 received_packet_no_;
  ::google::protobuf::uint64 sent_packet_no_;
  ::google::protobuf::uint64 last_packet_time_;
  int task_status_;
  friend struct ::protobuf_fs_5ftask_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Task

// required uint64 task_id = 1;
inline bool Task::has_task_id() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Task::set_has_task_id() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Task::clear_has_task_id() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Task::clear_task_id() {
  task_id_ = GOOGLE_ULONGLONG(0);
  clear_has_task_id();
}
inline ::google::protobuf::uint64 Task::task_id() const {
  // @@protoc_insertion_point(field_get:fs.proto.Task.task_id)
  return task_id_;
}
inline void Task::set_task_id(::google::protobuf::uint64 value) {
  set_has_task_id();
  task_id_ = value;
  // @@protoc_insertion_point(field_set:fs.proto.Task.task_id)
}

// optional string localbasepath = 2;
inline bool Task::has_localbasepath() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Task::set_has_localbasepath() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Task::clear_has_localbasepath() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Task::clear_localbasepath() {
  localbasepath_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_localbasepath();
}
inline const ::std::string& Task::localbasepath() const {
  // @@protoc_insertion_point(field_get:fs.proto.Task.localbasepath)
  return localbasepath_.GetNoArena();
}
inline void Task::set_localbasepath(const ::std::string& value) {
  set_has_localbasepath();
  localbasepath_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:fs.proto.Task.localbasepath)
}
#if LANG_CXX11
inline void Task::set_localbasepath(::std::string&& value) {
  set_has_localbasepath();
  localbasepath_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:fs.proto.Task.localbasepath)
}
#endif
inline void Task::set_localbasepath(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_localbasepath();
  localbasepath_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:fs.proto.Task.localbasepath)
}
inline void Task::set_localbasepath(const char* value, size_t size) {
  set_has_localbasepath();
  localbasepath_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:fs.proto.Task.localbasepath)
}
inline ::std::string* Task::mutable_localbasepath() {
  set_has_localbasepath();
  // @@protoc_insertion_point(field_mutable:fs.proto.Task.localbasepath)
  return localbasepath_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Task::release_localbasepath() {
  // @@protoc_insertion_point(field_release:fs.proto.Task.localbasepath)
  if (!has_localbasepath()) {
    return NULL;
  }
  clear_has_localbasepath();
  return localbasepath_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Task::set_allocated_localbasepath(::std::string* localbasepath) {
  if (localbasepath != NULL) {
    set_has_localbasepath();
  } else {
    clear_has_localbasepath();
  }
  localbasepath_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), localbasepath);
  // @@protoc_insertion_point(field_set_allocated:fs.proto.Task.localbasepath)
}

// required string remotebasepath = 3;
inline bool Task::has_remotebasepath() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Task::set_has_remotebasepath() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Task::clear_has_remotebasepath() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Task::clear_remotebasepath() {
  remotebasepath_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_remotebasepath();
}
inline const ::std::string& Task::remotebasepath() const {
  // @@protoc_insertion_point(field_get:fs.proto.Task.remotebasepath)
  return remotebasepath_.GetNoArena();
}
inline void Task::set_remotebasepath(const ::std::string& value) {
  set_has_remotebasepath();
  remotebasepath_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:fs.proto.Task.remotebasepath)
}
#if LANG_CXX11
inline void Task::set_remotebasepath(::std::string&& value) {
  set_has_remotebasepath();
  remotebasepath_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:fs.proto.Task.remotebasepath)
}
#endif
inline void Task::set_remotebasepath(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_remotebasepath();
  remotebasepath_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:fs.proto.Task.remotebasepath)
}
inline void Task::set_remotebasepath(const char* value, size_t size) {
  set_has_remotebasepath();
  remotebasepath_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:fs.proto.Task.remotebasepath)
}
inline ::std::string* Task::mutable_remotebasepath() {
  set_has_remotebasepath();
  // @@protoc_insertion_point(field_mutable:fs.proto.Task.remotebasepath)
  return remotebasepath_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Task::release_remotebasepath() {
  // @@protoc_insertion_point(field_release:fs.proto.Task.remotebasepath)
  if (!has_remotebasepath()) {
    return NULL;
  }
  clear_has_remotebasepath();
  return remotebasepath_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Task::set_allocated_remotebasepath(::std::string* remotebasepath) {
  if (remotebasepath != NULL) {
    set_has_remotebasepath();
  } else {
    clear_has_remotebasepath();
  }
  remotebasepath_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), remotebasepath);
  // @@protoc_insertion_point(field_set_allocated:fs.proto.Task.remotebasepath)
}

// required string filename = 4;
inline bool Task::has_filename() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Task::set_has_filename() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Task::clear_has_filename() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Task::clear_filename() {
  filename_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_filename();
}
inline const ::std::string& Task::filename() const {
  // @@protoc_insertion_point(field_get:fs.proto.Task.filename)
  return filename_.GetNoArena();
}
inline void Task::set_filename(const ::std::string& value) {
  set_has_filename();
  filename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:fs.proto.Task.filename)
}
#if LANG_CXX11
inline void Task::set_filename(::std::string&& value) {
  set_has_filename();
  filename_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:fs.proto.Task.filename)
}
#endif
inline void Task::set_filename(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_filename();
  filename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:fs.proto.Task.filename)
}
inline void Task::set_filename(const char* value, size_t size) {
  set_has_filename();
  filename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:fs.proto.Task.filename)
}
inline ::std::string* Task::mutable_filename() {
  set_has_filename();
  // @@protoc_insertion_point(field_mutable:fs.proto.Task.filename)
  return filename_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Task::release_filename() {
  // @@protoc_insertion_point(field_release:fs.proto.Task.filename)
  if (!has_filename()) {
    return NULL;
  }
  clear_has_filename();
  return filename_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Task::set_allocated_filename(::std::string* filename) {
  if (filename != NULL) {
    set_has_filename();
  } else {
    clear_has_filename();
  }
  filename_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), filename);
  // @@protoc_insertion_point(field_set_allocated:fs.proto.Task.filename)
}

// required uint64 total_packet_no = 5;
inline bool Task::has_total_packet_no() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void Task::set_has_total_packet_no() {
  _has_bits_[0] |= 0x00000010u;
}
inline void Task::clear_has_total_packet_no() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void Task::clear_total_packet_no() {
  total_packet_no_ = GOOGLE_ULONGLONG(0);
  clear_has_total_packet_no();
}
inline ::google::protobuf::uint64 Task::total_packet_no() const {
  // @@protoc_insertion_point(field_get:fs.proto.Task.total_packet_no)
  return total_packet_no_;
}
inline void Task::set_total_packet_no(::google::protobuf::uint64 value) {
  set_has_total_packet_no();
  total_packet_no_ = value;
  // @@protoc_insertion_point(field_set:fs.proto.Task.total_packet_no)
}

// required .fs.proto.Task.TaskStatus task_status = 6;
inline bool Task::has_task_status() const {
  return (_has_bits_[0] & 0x00000100u) != 0;
}
inline void Task::set_has_task_status() {
  _has_bits_[0] |= 0x00000100u;
}
inline void Task::clear_has_task_status() {
  _has_bits_[0] &= ~0x00000100u;
}
inline void Task::clear_task_status() {
  task_status_ = 0;
  clear_has_task_status();
}
inline ::fs::proto::Task_TaskStatus Task::task_status() const {
  // @@protoc_insertion_point(field_get:fs.proto.Task.task_status)
  return static_cast< ::fs::proto::Task_TaskStatus >(task_status_);
}
inline void Task::set_task_status(::fs::proto::Task_TaskStatus value) {
  assert(::fs::proto::Task_TaskStatus_IsValid(value));
  set_has_task_status();
  task_status_ = value;
  // @@protoc_insertion_point(field_set:fs.proto.Task.task_status)
}

// optional uint64 received_packet_no = 7;
inline bool Task::has_received_packet_no() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void Task::set_has_received_packet_no() {
  _has_bits_[0] |= 0x00000020u;
}
inline void Task::clear_has_received_packet_no() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void Task::clear_received_packet_no() {
  received_packet_no_ = GOOGLE_ULONGLONG(0);
  clear_has_received_packet_no();
}
inline ::google::protobuf::uint64 Task::received_packet_no() const {
  // @@protoc_insertion_point(field_get:fs.proto.Task.received_packet_no)
  return received_packet_no_;
}
inline void Task::set_received_packet_no(::google::protobuf::uint64 value) {
  set_has_received_packet_no();
  received_packet_no_ = value;
  // @@protoc_insertion_point(field_set:fs.proto.Task.received_packet_no)
}

// optional uint64 sent_packet_no = 8;
inline bool Task::has_sent_packet_no() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void Task::set_has_sent_packet_no() {
  _has_bits_[0] |= 0x00000040u;
}
inline void Task::clear_has_sent_packet_no() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void Task::clear_sent_packet_no() {
  sent_packet_no_ = GOOGLE_ULONGLONG(0);
  clear_has_sent_packet_no();
}
inline ::google::protobuf::uint64 Task::sent_packet_no() const {
  // @@protoc_insertion_point(field_get:fs.proto.Task.sent_packet_no)
  return sent_packet_no_;
}
inline void Task::set_sent_packet_no(::google::protobuf::uint64 value) {
  set_has_sent_packet_no();
  sent_packet_no_ = value;
  // @@protoc_insertion_point(field_set:fs.proto.Task.sent_packet_no)
}

// optional uint64 last_packet_time = 9;
inline bool Task::has_last_packet_time() const {
  return (_has_bits_[0] & 0x00000080u) != 0;
}
inline void Task::set_has_last_packet_time() {
  _has_bits_[0] |= 0x00000080u;
}
inline void Task::clear_has_last_packet_time() {
  _has_bits_[0] &= ~0x00000080u;
}
inline void Task::clear_last_packet_time() {
  last_packet_time_ = GOOGLE_ULONGLONG(0);
  clear_has_last_packet_time();
}
inline ::google::protobuf::uint64 Task::last_packet_time() const {
  // @@protoc_insertion_point(field_get:fs.proto.Task.last_packet_time)
  return last_packet_time_;
}
inline void Task::set_last_packet_time(::google::protobuf::uint64 value) {
  set_has_last_packet_time();
  last_packet_time_ = value;
  // @@protoc_insertion_point(field_set:fs.proto.Task.last_packet_time)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace proto
}  // namespace fs

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::fs::proto::Task_TaskStatus> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::fs::proto::Task_TaskStatus>() {
  return ::fs::proto::Task_TaskStatus_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_fs_5ftask_2eproto

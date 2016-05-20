PROTOBUF_3_DIST:=${.CURDIR} 

.PATH: ${PROTOBUF_3_DIST}/src

CXXFLAGS+= -I${PROTOBUF_3_DIST}/src -DHAVE_PTHREAD -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare

#CXXFLAGS += -I${PROTOBUF_3_DIST}/src -DHAVE_PTHREAD

# NO_THREAD_SAFETY needed for powerpc builds.  TBD.
.if ${MACHINE_ARCH} == "powerpc"
CXXFLAGS+= -DGOOGLE_PROTOBUF_NO_THREAD_SAFETY
.endif


#DPLIBS+= ${LIBSTDCPLUSPLUS}

SRCS = \
  google/protobuf/stubs/atomicops_internals_x86_gcc.cc         \
  google/protobuf/stubs/atomicops_internals_x86_msvc.cc        \
  google/protobuf/stubs/int128.cc			       \
  google/protobuf/stubs/common.cc                              \
  google/protobuf/stubs/once.cc                                \
  google/protobuf/stubs/hash.h                                 \
  google/protobuf/stubs/map_util.h                             \
  google/protobuf/stubs/shared_ptr.h                           \
  google/protobuf/stubs/stringprintf.cc                        \
  google/protobuf/stubs/stringprintf.h                         \
  google/protobuf/stubs/status.cc                              \
  google/protobuf/stubs/statusor.cc                            \
  google/protobuf/stubs/stringpiece.cc                         \
  google/protobuf/stubs/time.cc                           \
  google/protobuf/arena.cc                                     \
  google/protobuf/arenastring.cc                               \
  google/protobuf/extension_set.cc                             \
  google/protobuf/generated_message_util.cc                    \
  google/protobuf/message_lite.cc                              \
  google/protobuf/repeated_field.cc                            \
  google/protobuf/wire_format_lite.cc                          \
  google/protobuf/io/coded_stream.cc                           \
  google/protobuf/io/coded_stream_inl.h                        \
  google/protobuf/io/zero_copy_stream.cc                       \
  google/protobuf/io/zero_copy_stream_impl_lite.cc

SRCS += \
  google/protobuf/any.cc                              \
  google/protobuf/any.pb.cc                              \
  google/protobuf/api.pb.cc                       \
  google/protobuf/duration.pb.cc                                \
  google/protobuf/empty.pb.cc                       \
  google/protobuf/field_mask.pb.cc                             \
  google/protobuf/source_context.pb.cc                              \
  google/protobuf/struct.pb.cc                             \
  google/protobuf/timestamp.pb.cc                             \
  google/protobuf/type.pb.cc                              \
  google/protobuf/struct.pb.cc                             \
  google/protobuf/wrappers.pb.cc \
  google/protobuf/stubs/mathlimits.cc                              \
  google/protobuf/stubs/strutil.cc                             \
  google/protobuf/stubs/strutil.h                              \
  google/protobuf/stubs/substitute.cc                          \
  google/protobuf/stubs/substitute.h                           \
  google/protobuf/stubs/structurally_valid.cc                  \
  google/protobuf/descriptor.cc                                \
  google/protobuf/descriptor_database.cc                       \
  google/protobuf/descriptor.pb.cc                             \
  google/protobuf/dynamic_message.cc                           \
  google/protobuf/extension_set_heavy.cc                       \
  google/protobuf/generated_message_reflection.cc              \
  google/protobuf/map_field.cc                                 \
  google/protobuf/message.cc                                   \
  google/protobuf/reflection_internal.h                        \
  google/protobuf/reflection_ops.cc                            \
  google/protobuf/service.cc                                   \
  google/protobuf/text_format.cc                               \
  google/protobuf/unknown_field_set.cc                         \
  google/protobuf/wire_format.cc                               \
  google/protobuf/io/gzip_stream.cc                            \
  google/protobuf/io/printer.cc                                \
  google/protobuf/io/strtod.cc                                 \
  google/protobuf/io/tokenizer.cc                              \
  google/protobuf/io/zero_copy_stream_impl.cc                  \
  google/protobuf/compiler/importer.cc                         \
  google/protobuf/compiler/parser.cc                          \
  google/protobuf/util/field_comparator.cc                         \
  google/protobuf/util/field_mask_util.cc \
  google/protobuf/util/internal/datapiece.cc                         \
  google/protobuf/util/internal/default_value_objectwriter.cc \
  google/protobuf/util/internal/error_listener.cc                       \
  google/protobuf/util/internal/field_mask_utility.cc \
  google/protobuf/util/internal/json_escaping.cc                         \
  google/protobuf/util/internal/json_objectwriter.cc \
  google/protobuf/util/internal/json_stream_parser.cc                     \
  google/protobuf/util/internal/object_writer.cc \
  google/protobuf/util/internal/protostream_objectsource.cc                \
  google/protobuf/util/internal/protostream_objectwriter.cc \
  google/protobuf/util/internal/type_info.cc                         \
  google/protobuf/util/internal/type_info_test_helper.cc \
  google/protobuf/util/internal/utility.cc                         \
  google/protobuf/util/json_util.cc \
  google/protobuf/util/message_differencer.cc                         \
  google/protobuf/util/time_util.cc \
  google/protobuf/util/type_resolver_util.cc

.include <bsd.lib.mk>

CC := /volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/3.5/20150720/i386-unknown-freebsd/bin/i386-unknown-freebsd10.1-clang
CXX := /volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/3.5/20150720/i386-unknown-freebsd/bin/i386-unknown-freebsd10.1-clang++
NO_INLINE_FUNCTIONS_CALLED_ONCE := ""
CXXFLAGS_LAST += -std=c++11 -stdlib=libc++ 
CXXINCLUDES := -I/volume/publish/dev/occam/llvm-3.5/sysroot/stable_10/current/i386-unknown-freebsd10.1/usr/include/c++/v1
AR := /volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/3.5/20150720/i386-unknown-freebsd/bin/i386-unknown-freebsd10.1-ar

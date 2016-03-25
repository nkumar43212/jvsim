# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: packet_stats.proto

from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


import jvision_top_pb2 as jvision__top__pb2


DESCRIPTOR = _descriptor.FileDescriptor(
  name='packet_stats.proto',
  package='',
  syntax='proto2',
  serialized_pb=b'\n\x12packet_stats.proto\x1a\x11jvision_top.proto\"y\n\x10PacketStatistics\x12\'\n\x0cpacket_stats\x18\x01 \x03(\x0b\x32\x11.PacketStatsClass\x12<\n\x10packet_stats_pfe\x18\x02 \x03(\x0b\x32\".PacketStatsPacketForwardingEngine\"d\n!PacketStatsPacketForwardingEngine\x12\x16\n\x0epfe_identifier\x18\x01 \x02(\t\x12\'\n\x0cpacket_stats\x18\x02 \x03(\x0b\x32\x11.PacketStatsClass\"F\n\x10PacketStatsClass\x12\x0c\n\x04name\x18\x01 \x02(\t\x12$\n\x07\x63ounter\x18\x02 \x02(\x0b\x32\x13.PacketStatsCounter\"S\n\x12PacketStatsCounter\x12\x14\n\x0cpacket_count\x18\x01 \x02(\x04\x12\x12\n\nbyte_count\x18\x02 \x01(\x04\x12\x13\n\x0bpacket_rate\x18\x03 \x01(\x04:N\n\x1ajnpr_packet_statistics_ext\x12\x17.JuniperNetworksSensors\x18\n \x01(\x0b\x32\x11.PacketStatistics'
  ,
  dependencies=[jvision__top__pb2.DESCRIPTOR,])
_sym_db.RegisterFileDescriptor(DESCRIPTOR)


JNPR_PACKET_STATISTICS_EXT_FIELD_NUMBER = 10
jnpr_packet_statistics_ext = _descriptor.FieldDescriptor(
  name='jnpr_packet_statistics_ext', full_name='jnpr_packet_statistics_ext', index=0,
  number=10, type=11, cpp_type=10, label=1,
  has_default_value=False, default_value=None,
  message_type=None, enum_type=None, containing_type=None,
  is_extension=True, extension_scope=None,
  options=None)


_PACKETSTATISTICS = _descriptor.Descriptor(
  name='PacketStatistics',
  full_name='PacketStatistics',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='packet_stats', full_name='PacketStatistics.packet_stats', index=0,
      number=1, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='packet_stats_pfe', full_name='PacketStatistics.packet_stats_pfe', index=1,
      number=2, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=41,
  serialized_end=162,
)


_PACKETSTATSPACKETFORWARDINGENGINE = _descriptor.Descriptor(
  name='PacketStatsPacketForwardingEngine',
  full_name='PacketStatsPacketForwardingEngine',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='pfe_identifier', full_name='PacketStatsPacketForwardingEngine.pfe_identifier', index=0,
      number=1, type=9, cpp_type=9, label=2,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='packet_stats', full_name='PacketStatsPacketForwardingEngine.packet_stats', index=1,
      number=2, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=164,
  serialized_end=264,
)


_PACKETSTATSCLASS = _descriptor.Descriptor(
  name='PacketStatsClass',
  full_name='PacketStatsClass',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='name', full_name='PacketStatsClass.name', index=0,
      number=1, type=9, cpp_type=9, label=2,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='counter', full_name='PacketStatsClass.counter', index=1,
      number=2, type=11, cpp_type=10, label=2,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=266,
  serialized_end=336,
)


_PACKETSTATSCOUNTER = _descriptor.Descriptor(
  name='PacketStatsCounter',
  full_name='PacketStatsCounter',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='packet_count', full_name='PacketStatsCounter.packet_count', index=0,
      number=1, type=4, cpp_type=4, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='byte_count', full_name='PacketStatsCounter.byte_count', index=1,
      number=2, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='packet_rate', full_name='PacketStatsCounter.packet_rate', index=2,
      number=3, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=338,
  serialized_end=421,
)

_PACKETSTATISTICS.fields_by_name['packet_stats'].message_type = _PACKETSTATSCLASS
_PACKETSTATISTICS.fields_by_name['packet_stats_pfe'].message_type = _PACKETSTATSPACKETFORWARDINGENGINE
_PACKETSTATSPACKETFORWARDINGENGINE.fields_by_name['packet_stats'].message_type = _PACKETSTATSCLASS
_PACKETSTATSCLASS.fields_by_name['counter'].message_type = _PACKETSTATSCOUNTER
DESCRIPTOR.message_types_by_name['PacketStatistics'] = _PACKETSTATISTICS
DESCRIPTOR.message_types_by_name['PacketStatsPacketForwardingEngine'] = _PACKETSTATSPACKETFORWARDINGENGINE
DESCRIPTOR.message_types_by_name['PacketStatsClass'] = _PACKETSTATSCLASS
DESCRIPTOR.message_types_by_name['PacketStatsCounter'] = _PACKETSTATSCOUNTER
DESCRIPTOR.extensions_by_name['jnpr_packet_statistics_ext'] = jnpr_packet_statistics_ext

PacketStatistics = _reflection.GeneratedProtocolMessageType('PacketStatistics', (_message.Message,), dict(
  DESCRIPTOR = _PACKETSTATISTICS,
  __module__ = 'packet_stats_pb2'
  # @@protoc_insertion_point(class_scope:PacketStatistics)
  ))
_sym_db.RegisterMessage(PacketStatistics)

PacketStatsPacketForwardingEngine = _reflection.GeneratedProtocolMessageType('PacketStatsPacketForwardingEngine', (_message.Message,), dict(
  DESCRIPTOR = _PACKETSTATSPACKETFORWARDINGENGINE,
  __module__ = 'packet_stats_pb2'
  # @@protoc_insertion_point(class_scope:PacketStatsPacketForwardingEngine)
  ))
_sym_db.RegisterMessage(PacketStatsPacketForwardingEngine)

PacketStatsClass = _reflection.GeneratedProtocolMessageType('PacketStatsClass', (_message.Message,), dict(
  DESCRIPTOR = _PACKETSTATSCLASS,
  __module__ = 'packet_stats_pb2'
  # @@protoc_insertion_point(class_scope:PacketStatsClass)
  ))
_sym_db.RegisterMessage(PacketStatsClass)

PacketStatsCounter = _reflection.GeneratedProtocolMessageType('PacketStatsCounter', (_message.Message,), dict(
  DESCRIPTOR = _PACKETSTATSCOUNTER,
  __module__ = 'packet_stats_pb2'
  # @@protoc_insertion_point(class_scope:PacketStatsCounter)
  ))
_sym_db.RegisterMessage(PacketStatsCounter)

jnpr_packet_statistics_ext.message_type = _PACKETSTATISTICS
jvision__top__pb2.JuniperNetworksSensors.RegisterExtension(jnpr_packet_statistics_ext)

# @@protoc_insertion_point(module_scope)

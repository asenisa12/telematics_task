/* Generated by the protocol buffer compiler.  DO NOT EDIT! */

#ifndef PROTOBUF_C_message_2eproto__INCLUDED
#define PROTOBUF_C_message_2eproto__INCLUDED

#include <google/protobuf-c/protobuf-c.h>

//PROTOBUF_C_BEGIN_DECLS
#define PROTOBUF_C_OFFSETOF(struct, member) offsetof(struct, member)
#define PROTOBUF_C_MESSAGE_DESCRIPTOR_MAGIC 0x28aaeef9
#define PROTOBUF_C_ASSERT(condition) assert(condition)


typedef struct _CalcMessage CalcMessage;


/* --- enums --- */


/* --- messages --- */

struct  _CalcMessage
{
  ProtobufCMessage base;
  int32_t id;
  char *msg;
  protobuf_c_boolean has_num;
  double num;
};
#define CALC_MESSAGE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&calc_message__descriptor) \
    , 0, NULL, 0,0 }


/* CalcMessage methods */
void   calc_message__init
                     (CalcMessage         *message);
size_t calc_message__get_packed_size
                     (const CalcMessage   *message);
size_t calc_message__pack
                     (const CalcMessage   *message,
                      uint8_t             *out);
size_t calc_message__pack_to_buffer
                     (const CalcMessage   *message,
                      ProtobufCBuffer     *buffer);
CalcMessage *
       calc_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   calc_message__free_unpacked
                     (CalcMessage *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*CalcMessage_Closure)
                 (const CalcMessage *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor calc_message__descriptor;

//PROTOBUF_C_END_DECLS


#endif  /* PROTOBUF_message_2eproto__INCLUDED */

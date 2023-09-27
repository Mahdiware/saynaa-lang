/*
 * Copyright (c) 2022-2023 Mohamed Abdifatah. All rights reserved.
 * Distributed Under The MIT License
 */

#ifndef __SAYNAA_BUFFERS__
#define __SAYNAA_BUFFERS__

#include "saynaa_internal.h"

// The macro 'DECLARE_BUFFER()' emulate the C++ template to declare and define
// different types of buffer objects.

// A buffer of type 'T' will contain a heap allocated array of 'T' with the
// capacity of 'Tbuffer.capacity' as 'T* Tbuffer.data'. When the capacity is
// filled with 'T' values (ie. Tbuffer.count == Tbuffer.capacity) the buffer's
// internal data array will be reallocate to a capacity of 'GROW_FACTOR' times
// it's last capacity.

#define DECLARE_BUFFER(m_name, m_type)                                        \
  typedef struct {                                                            \
    m_type* data;                                                             \
    uint32_t count;                                                           \
    uint32_t capacity;                                                        \
  } m_name##Buffer;                                                           \
                                                                              \
  /* Initialize a new buffer int instance. */                                 \
  void m_name##BufferInit(m_name##Buffer* this);                              \
                                                                              \
  /* Clears the allocated elements from the VM's realloc function. */         \
  void m_name##BufferClear(m_name##Buffer* this, VM* vm);                     \
                                                                              \
  /* Ensure the capacity is greater than [size], if not resize. */            \
  void m_name##BufferReserve(m_name##Buffer* this, VM* vm,                    \
                                 size_t size);                                \
                                                                              \
  /* Fill the buffer at the end of it with provided data if the capacity */   \
  /* isn't enough using VM's realloc function. */                             \
  void m_name##BufferFill(m_name##Buffer* this, VM* vm,                       \
                              m_type data, int count);                        \
                                                                              \
  /* Write to the buffer with provided data at the end of the buffer.*/       \
  void m_name##BufferWrite(m_name##Buffer* this,                              \
                               VM* vm, m_type data);                          \
                                                                              \
  /* Concatenate the contents of another buffer at the end of this buffer.*/  \
  void m_name##BufferConcat(m_name##Buffer* this, VM* vm,                     \
                                m_name##Buffer* other);                       \

// The buffer "template" implementation of different types.
#define DEFINE_BUFFER(m_name, m_type)                                         \
  void m_name##BufferInit(m_name##Buffer* this) {                             \
    this->data = NULL;                                                        \
    this->count = 0;                                                          \
    this->capacity = 0;                                                       \
  }                                                                           \
                                                                              \
  void m_name##BufferClear(m_name##Buffer* this,                              \
              VM* vm) {                                                       \
    vmRealloc(vm, this->data, this->capacity * sizeof(m_type), 0);            \
    this->data = NULL;                                                        \
    this->count = 0;                                                          \
    this->capacity = 0;                                                       \
  }                                                                           \
                                                                              \
  void m_name##BufferReserve(m_name##Buffer* this, VM* vm,                    \
                                 size_t size) {                               \
    if (this->capacity < size) {                                              \
      int capacity = utilPowerOf2Ceil((int)size);                             \
      if (capacity < MIN_CAPACITY) capacity = MIN_CAPACITY;                   \
      this->data = (m_type*) vmRealloc(vm, this->data,                        \
        this->capacity * sizeof(m_type), capacity * sizeof(m_type));          \
      this->capacity = capacity;                                              \
    }                                                                         \
  }                                                                           \
                                                                              \
  void m_name##BufferFill(m_name##Buffer* this, VM* vm,                       \
                              m_type data, int count) {                       \
                                                                              \
    m_name##BufferReserve(this, vm, this->count + count);                     \
                                                                              \
    for (int i = 0; i < count; i++) {                                         \
      this->data[this->count++] = data;                                       \
    }                                                                         \
  }                                                                           \
                                                                              \
  void m_name##BufferWrite(m_name##Buffer* this,                              \
                               VM* vm, m_type data) {                         \
    m_name##BufferFill(this, vm, data, 1);                                    \
  }                                                                           \
                                                                              \
  void m_name##BufferConcat(m_name##Buffer* this, VM* vm,                     \
                                m_name##Buffer* other) {                      \
    m_name##BufferReserve(this, vm, this->count + other->count);              \
                                                                              \
    memcpy(this->data + this->count,                                          \
           other->data,                                                       \
           other->count * sizeof(m_type));                                    \
    this->count += other->count;                                              \
  }

#endif // __SAYNAA_BUFFERS__
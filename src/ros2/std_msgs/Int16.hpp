// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _STD_MSGS_INT16_HPP_
#define _STD_MSGS_INT16_HPP_

#include "../topic.hpp"


namespace std_msgs {


class Int16 : public ros2::Topic<Int16>
{
public:
  int16_t data;

  Int16():
    Topic("std_msgs::msg::dds_::Int16_", "Int16"),
    data(0)
  { 
  }

  bool serialize(void* msg_buf, const Int16* topic)
  {
    ucdrBuffer* writer = (ucdrBuffer*)msg_buf;
    (void) ucdr_serialize_int16_t(writer, topic->data);

    return !writer->error;
  }

  bool deserialize(void* msg_buf, Int16* topic)
  {
    ucdrBuffer* reader = (ucdrBuffer*)msg_buf;
    (void) ucdr_deserialize_int16_t(reader, &topic->data);

    return !reader->error;
  }

  uint32_t size_of_topic(const Int16* topic, uint32_t size)
  {
    (void)(topic);

    uint32_t previousSize = size;
    size += ucdr_alignment(size, 2) + 2;

    return size - previousSize;
  }

};

} // namespace std_msgs


#endif // _STD_MSGS_INT16_HPP_

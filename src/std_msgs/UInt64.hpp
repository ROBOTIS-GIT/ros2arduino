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

#ifndef _STD_MSGS_UINT64_HPP_
#define _STD_MSGS_UINT64_HPP_


#include "micrortps.hpp"
#include <topic_config.h>
#include <topic.hpp>


namespace std_msgs {


class UInt64 : public ros2::Topic<UInt64>
{
public:
  uint64_t data;

  UInt64():
    Topic("std_msgs::msg::dds_::UInt64_", STD_MSGS_UINT64_TOPIC),
    data(0)
  { 
  }

  bool serialize(struct MicroBuffer* writer, const UInt64* topic)
  {
    (void) serialize_uint64_t(writer, topic->data);

    return writer->error == BUFFER_OK;
  }

  bool deserialize(struct MicroBuffer* reader, UInt64* topic)
  {
    (void) deserialize_uint64_t(reader, &topic->data);

    return reader->error == BUFFER_OK;
  }

  uint32_t size_of_topic(const UInt64* topic, uint32_t size)
  {
    (void)(topic);

    size += 8 + get_alignment(size, 8);

    return size;
  }

};

} // namespace std_msgs


#endif // _STD_MSGS_UINT64_HPP_

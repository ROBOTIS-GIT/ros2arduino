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

#ifndef _STD_MSGS_EMPTY_HPP_
#define _STD_MSGS_EMPTY_HPP_

#include "../topic.hpp"


namespace std_msgs
{

class Empty: public ros2::Topic<Empty>
{
  public:
    bool _dummy;

    Empty()
        : Topic("std_msgs::msg::dds_::Empty_", "Empty"), _dummy(0)
    {
    }

    bool serialize(void* msg_buf, const Empty* topic)
    {
      ucdrBuffer* writer = (ucdrBuffer*)msg_buf;
      ucdr_serialize_bool(writer, topic->_dummy);

      return !writer->error;
    }

    bool deserialize(void* msg_buf, Empty* topic)
    {
      ucdrBuffer* reader = (ucdrBuffer*)msg_buf;
      ucdr_deserialize_bool(reader, &topic->_dummy);

      return !reader->error;
    }

    uint32_t size_of_topic(const Empty* topic, uint32_t size)
    {
      (void) (topic);

      uint32_t previousSize = size;
      size += ucdr_alignment(size, 1) + 1;

      return size - previousSize;
    }

};

} // namespace std_msgs

#endif // _STD_MSGS_EMPTY_HPP_

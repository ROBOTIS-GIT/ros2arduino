/*
 * micro_xrce_dds_xml.h
 *
 *  Created on: Dec 14, 2018
 *      Author: Kei
 */

#ifndef MICRO_XRCE_DDS_XML_H_
#define MICRO_XRCE_DDS_XML_H_

#include <stdio.h>
#include <uxr/client/config.h>

#ifdef UXR_CREATE_ENTITIES_USING_XML
#define UXR_PARTICIPANT_XML "<dds><participant><rtps><name>%s</name></rtps></participant></dds>"
#define UXR_WRITER_XML "<dds><data_writer><topic><kind>NO_KEY</kind><name>%s</name><dataType>%s</dataType></topic></data_writer></dds>"
#define UXR_READER_XML "<dds><data_reader><topic><kind>NO_KEY</kind><name>%s</name><dataType>%s</dataType></topic></data_reader></dds>"
#define UXR_TOPIC_XML "<dds><topic><name>%s</name><dataType>%s</dataType></topic></dds>"
#endif






#endif /* MICRO_XRCE_DDS_XML_H_ */

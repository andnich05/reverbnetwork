/*
* ComponentMessage:
*
* Copyright (C) 2015  Andrej Nichelmann
*                     Klaus Michael Indlekofer
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
// UNUSED

#ifndef COMPONENTMESSAGE_H
#define COMPONENTMESSAGE_H

#include "..\pluginterfaces\vst\ivstmessage.h"
#include "public.sdk\source\vst\hosting\hostclasses.h"

namespace Steinberg {
namespace Vst {

class ComponentMessage : public IMessage {

public:

	ComponentMessage::ComponentMessage();
	ComponentMessage::~ComponentMessage();

	/** Returns the message ID (for example "TextMessage"). */
	virtual FIDString PLUGIN_API getMessageID();

	/** Sets a message ID (for example "TextMessage"). */
	virtual void PLUGIN_API setMessageID(FIDString id /*in*/);

	/** Returns the attribute list associated to the message. */
	virtual IAttributeList* PLUGIN_API getAttributes();



	virtual inline void setMessagePointer(void* messagePtr) { message = messagePtr; }
	//virtual inline void* getMessagePointer() { if(message) return message; }

	HostAttributeList* attributeList;
	void* message;

private:
	

};
}}

#endif // COMPONENTMESSAGE_H
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
	virtual inline void* getMessagePointer() { if(message) return message; }

	HostAttributeList* attributeList;
	void* message;

private:
	

};
}}

#endif // COMPONENTMESSAGE_H
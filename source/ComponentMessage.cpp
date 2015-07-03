#include "ComponentMessage.h"


namespace Steinberg {
namespace Vst {
	ComponentMessage::ComponentMessage()
		: message(nullptr)
	{
	}

	ComponentMessage::~ComponentMessage()
	{
	}

	FIDString PLUGIN_API ComponentMessage::getMessageID() {
		FIDString bla = "bla";
		return bla;
	}

	void PLUGIN_API ComponentMessage::setMessageID(FIDString id) {

	}

	IAttributeList* PLUGIN_API ComponentMessage::getAttributes() {
		if (!attributeList)
			attributeList = new HostAttributeList;
		return attributeList;
	}

}
}
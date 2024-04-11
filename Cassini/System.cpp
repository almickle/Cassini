#include "System.h"

System::System(Graphics& gfx, ResourceManager& manager, const string& in_systemID)
	: systemID(in_systemID)
{
	if (!manager.CheckForSystem(systemID)) {
		manager.RegisterSystem(systemID);
	}

	manager.RegisterSystemInstance(systemID, this);
}

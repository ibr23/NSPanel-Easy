// ha_components.cpp

#include "ha_components.h"

namespace esphome {
namespace nspanel_easy {

    // Function Definition
    HomeAssistantEntity extractHomeAssistantEntity(const std::string& entity_id) {
        size_t dotPos = entity_id.find(".");
        HomeAssistantEntity result;
        
        if (dotPos != std::string::npos && dotPos > 0 && dotPos + 1 < entity_id.size()) {
            // Extract domain and id from the entity_id string
            result.domain = entity_id.substr(0, dotPos);
            result.id = entity_id.substr(dotPos + 1);
            if (result.domain == "alarm_control_panel") {
                result.domain = "alarm";
            }
        } else {
            // No dot found, the entire entity_id is considered as id.
            result.domain = "invalid";
            result.id = entity_id;
        }
        return result;
    }

}  // namespace nspanel_easy
}  // namespace esphome

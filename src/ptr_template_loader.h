#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ReputationMgr.h"
#include "ScriptMgr.h"
#include "Realm.h"
#include "TaskScheduler.h"
#include "World.h"

#define module_string "ptr-template"

TaskScheduler scheduler;

// Temucraft: RBAC permissions (auth database, data/sql/db-auth/ptr_template_rbac.sql) instead of security levels
enum PtrTemplatePermissions : uint32
{
    RBAC_PTR_TEMPLATE_ENABLE   = 104101, // .template enable
    RBAC_PTR_TEMPLATE_DISABLE  = 104102, // .template disable
    RBAC_PTR_TEMPLATE_DISABLED = 104103, // see, and apply, disabled templates
    RBAC_PTR_TEMPLATE_APPLY    = 104150, // .template apply
    RBAC_PTR_TEMPLATE_LIST     = 104151  // .template list
};

// Temucraft: templates work only where PtrTemplate.Enable is on (the PTR), and when PtrTemplate.RealmId is set, only
// on that realm: a second guard in case the module is built into Live's worldserver too.
inline bool PtrTemplateRealmAllowed()
{
    if (!sConfigMgr->GetOption<bool>("PtrTemplate.Enable", false))
        return false;
    uint32 only = sConfigMgr->GetOption<uint32>("PtrTemplate.RealmId", 0);
    return !only || only == realm.Id.Realm;
}

// Temucraft: every key of this module is read as PtrTemplate.<name> (all modules share one config space)
template <typename T>
inline T PtrTemplateOption(std::string const& name, T def)
{
    return sConfigMgr->GetOption<T>("PtrTemplate." + name, def);
}

enum checkCodes
{
    MISSING_TEMPLATE_INFO       = 1,
    NOT_INITIAL_LEVEL           = 2,
    TEMPLATE_DISABLED_GLOBAL    = 3,
    INSUFFICIENT_SECURITY_LEVEL = 4,
    TEMPLATE_DISABLED_LOCAL     = 5,
    CHECK_PASSED                = 0
};

enum templateStrings
{
    FEEDBACK_TEMPLATE_ENABLE     = 0,
    FEEDBACK_TEMPLATE_MISSING    = 1,
    FEEDBACK_TEMPLATE_DISABLE    = 2,
    ERROR_TEMPLATE_INFO          = 3,
    ERROR_TEMPLATE_LEVEL         = 4,
    ERROR_TEMPLATE_DIABLE_GLOBAL = 5,
    ERROR_TEMPLATE_SECURITY      = 6,
    ERROR_TEMPLATE_DISABLE_LOCAL = 7,
    MESSAGE_TEMPLATE_LIST        = 8,
    MESSAGE_TEMPLATE_LIST_DETAIL = 9,
    MESSAGE_TEMPLATE_LIST_SIMPLE = 10,
    MESSAGE_TEMPLATE_LIST_EMPTY  = 11,
    DETAIL_ENABLE                = 12,
    DETAIL_DISABLE               = 13,
    ALERT_MODULE_PRESENCE        = 14,
    MAIL_ERROR_EQUIP_BODY        = 15,
    MAIL_BOOST_SUBJECT           = 16,
    MAIL_BOOST_BODY              = 17,
    MAIL_RESURRECTION_SUBJECT    = 18,
    MAIL_RESURRECTION_BODY       = 19
};

enum templateMail
{
    MAIL_BOOST        = 403,
    MAIL_RESURRECTION = 344
};

enum itemCleanupCodes
{
    METHOD_DELETE  = 0,
    METHOD_BOOST   = 1,
    METHOD_SCROLL  = 2,
    SCOPE_EQUIPPED = 0,
    SCOPE_BAGS     = 1,
    SCOPE_ALL      = 2
};

enum TemplateEnums
{
    APPLY_DELAY          = 25,
    APPLY_RATE           = 50,
    HORDE_SIMILAR        = -1,
    ACTION_BUTTON_BEGIN  = 0,
    CONTAINER_BACKPACK   = 0,
    CONTAINER_FINISH     = 5,
    ITEM_GOLD            = 8,
    INVENTORY_SLOT_START = 0,
    MAILED_ITEM_DELAY    = 180
};

void SendTalentReset(Player* player)
{
    WorldPacket data(SMSG_TALENTS_INVOLUNTARILY_RESET, 1);
    data << uint8(0);
    player->SendMessageToSet(&data, true);
}

void Add_ptr_template();
void AddSC_ptr_template_commandscript();

void Addmod_ptr_templateScripts()
{
    Add_ptr_template();
    AddSC_ptr_template_commandscript();
}

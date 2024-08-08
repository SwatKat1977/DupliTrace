#include <string>
#include "gtest/gtest.h"
#include "ConfigManager.h"
#include "ConfigSetup.h"
#include "ConfigSetupItem.h"

using duplitrace::common::ConfigSetup;
using duplitrace::common::ConfigSetupItem;
using duplitrace::common::SectionList;
using duplitrace::common::StringList;
using duplitrace::common::CONFIG_ITEM_TYPE_INTEGER;
using duplitrace::common::CONFIG_ITEM_TYPE_STRING;

const SectionList ValidConfigOptions = {
    {
        "str_value_with_valid_values",
        ConfigSetupItem("str_value_with_valid_values",
                        CONFIG_ITEM_TYPE_STRING)
            .DefaultValue("value_1")
            .ValidValues(StringList{ "value_1", "value_2" })
    },
    {
        "str_value",
        ConfigSetupItem("str_value",
                        CONFIG_ITEM_TYPE_STRING)
            .DefaultValue ("")
    },
    {
        "int_value",
        ConfigSetupItem("int_value",
                        CONFIG_ITEM_TYPE_INTEGER)
            .DefaultValue("12")
    },
};

duplitrace::common::SectionsMap CONFIGURATION_LAYOUT = {
    { "test_section", ValidConfigOptions }
};

class ConfigManagerTest : public ::testing::Test {
protected:
    duplitrace::common::ConfigManager config_manager;
};

TEST_F(ConfigManagerTest, ValidConfigDefaultValues) {

    ConfigSetup layout = ConfigSetup(CONFIGURATION_LAYOUT);

    config_manager.Configure(&layout);

    EXPECT_TRUE(config_manager.processConfig());

    EXPECT_EQ(config_manager.GetStringEntry("test_section",
                                            "str_value_with_valid_values"),
              "value_1");
    EXPECT_EQ (config_manager.GetStringEntry("test_section",
                                             "str_value"),
              "");
    EXPECT_EQ (config_manager.GetIntEntry ("test_section",
        "int_value"),
        9999);
}

/*
TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}
*/

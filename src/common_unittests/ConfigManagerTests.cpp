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
            .DefaultValue("<empty>")
    },
    {
        "int_value",
        ConfigSetupItem("int_value",
                        CONFIG_ITEM_TYPE_INTEGER)
            .DefaultValue(12)
    },
};

duplitrace::common::SectionsMap CONFIGURATION_LAYOUT = {
    { "test_section", ValidConfigOptions }
};

class ConfigManagerTest : public ::testing::Test {
protected:
    duplitrace::common::ConfigManager config_manager;
};

TEST_F(ConfigManagerTest, ValidConfigDefaultValuesNoFile) {

    ConfigSetup layout = ConfigSetup(CONFIGURATION_LAYOUT);

    config_manager.Configure(&layout);

    EXPECT_TRUE(config_manager.processConfig());

    EXPECT_EQ(config_manager.GetStringEntry("test_section",
                                            "str_value_with_valid_values"),
              "value_1");
    EXPECT_EQ (config_manager.GetStringEntry("test_section",
                                             "str_value"),
              "<empty>");
    EXPECT_EQ(config_manager.GetIntEntry("test_section", "int_value"), 12);
}

TEST_F (ConfigManagerTest, ValidConfigDefaultValuesWithFile) {

    ConfigSetup layout = ConfigSetup(CONFIGURATION_LAYOUT);

    config_manager.Configure(&layout, "valid_config.cfg");

    EXPECT_TRUE (config_manager.processConfig());

    EXPECT_EQ (config_manager.GetStringEntry("test_section",
        "str_value_with_valid_values"),
        "value_2");
    EXPECT_EQ (config_manager.GetStringEntry("test_section", "str_value"),
                                             "ValidStr");
    EXPECT_EQ (config_manager.GetIntEntry("test_section", "int_value"), 1701);
}

TEST_F (ConfigManagerTest, InvalidConfigStringNotInValidList) {
    std::stringstream buffer;
    std::streambuf* oldCerr = std::cerr.rdbuf (buffer.rdbuf ());

    ConfigSetup layout = ConfigSetup (CONFIGURATION_LAYOUT);

    config_manager.Configure (&layout, "invalid_config_wrong_str_value.cfg");

    config_manager.processConfig ();

    // Restore the original std::cerr
    std::cerr.rdbuf (oldCerr);

    // Check that the content of std::cerr is what we expect
    EXPECT_EQ (
        "Read config item failed: Config item test_section::str_value_with_valid_values value 'value_20' is not valid!\n",
        buffer.str ());
}

TEST_F (ConfigManagerTest, InvalidConfigInvalidIntValue) {
    std::stringstream buffer;
    std::streambuf* oldCerr = std::cerr.rdbuf (buffer.rdbuf ());

    ConfigSetup layout = ConfigSetup (CONFIGURATION_LAYOUT);

    config_manager.Configure(&layout, "invalid_config_non_int.cfg");

    config_manager.processConfig();

    // Restore the original std::cerr
    std::cerr.rdbuf (oldCerr);

    // Check that the content of std::cerr is what we expect
    EXPECT_EQ(
        "Read config item failed: Cannot convert int value 'NotAnInt' for test_section::int_value\n",
        buffer.str());
}

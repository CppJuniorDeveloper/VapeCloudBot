#pragma once

#include <unordered_map>
#include <string>

namespace Utils
{
    inline const std::unordered_map<std::string, std::string> COMMAND_MESSAGE_TABLE =
    {
        {"onCommandStart",

        "👋 Welcome to VapeCloud!\n\n"
        "I'm your personal assistant for buying e-cigarettes, vapes, and liquids.\n\n"
        "🛒 What can I do for you?\n"
        "• Show catalog — /catalog\n"
        "• View your cart — /cart\n"
        "• See your orders — /orders\n"
        "• Get help — /help\n\n"
        "Let's get started! 🚀"

        },
        {"onCommandLanguage",

        "🌍 Please, select your language"

        },
        {"onCommandAdmin",

        "🛡️ ADMIN PANEL\n\n"
        "Status: [ADMIN]\n"
        "ID: "

        }
    };

    inline const std::unordered_map<std::string, std::string> BUTTON_LABEL_TABLE =
    {
        {"onButtonChooseOwnLanguageCode",

        "Other language (enter 2-letter code)"

        },
        {"onButtonEnableTranslation",

        "Enable translation"

        },
        {"onButtonDisableTranslation",

        "Disable translation"

        }
    };

    inline const std::unordered_map<std::string, std::string> ERROR_MESSAGE_TABLE =
    {
        {"onDenyAdminRoots",

        "❌ Access denied. You are not an administrator."

        }
    };
}

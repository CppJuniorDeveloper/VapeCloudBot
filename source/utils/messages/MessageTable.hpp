#pragma once

#include <unordered_map>
#include <string>

namespace Utils
{
    inline const std::unordered_map<std::string, std::string> MESSAGE_TABLE =
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

        }
    };
}

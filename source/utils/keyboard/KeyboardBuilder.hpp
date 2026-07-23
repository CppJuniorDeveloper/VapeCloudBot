#pragma once

#include <tgbot/tgbot.h>
#include <vector>
#include <string>

namespace Utils
{
    class KeyboardBuilder
    {
    public:

        class InlineBuilder
        {
        public:

            InlineBuilder() = default;

        public:

        InlineBuilder& addButton(const std::string& text, const std::string& callback_data);
        InlineBuilder& addUrlButton(const std::string& text, const std::string& url);
        InlineBuilder& addSwitchButton(const std::string& text, const std::string& query = "");
        InlineBuilder& newRow();

        TgBot::InlineKeyboardMarkup::Ptr build() const;

        private:

            std::vector<std::vector<TgBot::InlineKeyboardButton::Ptr>> m_rows{};
            std::vector<TgBot::InlineKeyboardButton::Ptr> m_currentRow{};

        };

        class ReplyBuilder
        {
        public:

            ReplyBuilder() = default;

        public:

            ReplyBuilder& addButton(const std::string& text);
            ReplyBuilder& newRow();

            ReplyBuilder& resizeKeyboard(bool resize = true);
            ReplyBuilder& oneTimeKeyboard(bool one_time = true);
            ReplyBuilder& selective(bool selective = true);

            TgBot::ReplyKeyboardMarkup::Ptr build() const;

        private:

            std::vector<std::vector<TgBot::KeyboardButton::Ptr>> m_rows{};
            std::vector<TgBot::KeyboardButton::Ptr> m_currentRow{};

            bool m_resizeKeyboard  = true;
            bool m_oneTimeKeyboard = false;
            bool m_selective       = false;
        };

        static TgBot::InlineKeyboardMarkup::Ptr oneInlineButton(
            const std::string& text,
            const std::string& callbackData
        );
        static TgBot::InlineKeyboardMarkup::Ptr twoInlineButtons(
            const std::string& text1, const std::string& data1,
            const std::string& text2, const std::string& data2
        );

        static TgBot::ReplyKeyboardMarkup::Ptr oneReplyButton(const std::string& text);
        static TgBot::ReplyKeyboardMarkup::Ptr twoReplyButtons(
            const std::string& text1,
            const std::string& text2
        );

        static TgBot::ReplyKeyboardRemove::Ptr removeKeyboard();

    };
}

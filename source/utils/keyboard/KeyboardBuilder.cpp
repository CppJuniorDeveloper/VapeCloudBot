#include "KeyboardBuilder.hpp"

Utils::KeyboardBuilder::InlineBuilder &Utils::KeyboardBuilder::InlineBuilder::addButton(const std::string &text, const std::string &callback_data)
{
    auto button = std::make_shared<TgBot::InlineKeyboardButton>();
    button->text = text;
    button->callbackData = callback_data;
    m_currentRow.push_back(std::move(button));
    return *this;
}

Utils::KeyboardBuilder::InlineBuilder &Utils::KeyboardBuilder::InlineBuilder::addUrlButton(const std::string &text, const std::string &url)
{
    auto button = std::make_shared<TgBot::InlineKeyboardButton>();
    button->text = text;
    button->url = url;
    m_currentRow.push_back(std::move(button));
    return *this;
}

Utils::KeyboardBuilder::InlineBuilder &Utils::KeyboardBuilder::InlineBuilder::addSwitchButton(const std::string &text, const std::string &query)
{
    auto button = std::make_shared<TgBot::InlineKeyboardButton>();
    button->text = text;
    button->switchInlineQueryCurrentChat = query;
    m_currentRow.push_back(std::move(button));
    return *this;
}

Utils::KeyboardBuilder::InlineBuilder &Utils::KeyboardBuilder::InlineBuilder::newRow()
{
    if (!m_currentRow.empty())
    {
        m_rows.push_back(std::move(m_currentRow));
    }
    return *this;
}

TgBot::InlineKeyboardMarkup::Ptr Utils::KeyboardBuilder::InlineBuilder::build() const
{
    auto keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();

    keyboard->inlineKeyboard = m_rows;
    if (!m_currentRow.empty())
    {
        keyboard->inlineKeyboard.push_back(m_currentRow);
    }

    return keyboard;
}

Utils::KeyboardBuilder::ReplyBuilder &Utils::KeyboardBuilder::ReplyBuilder::addButton(const std::string &text)
{
    auto button = std::make_shared<TgBot::KeyboardButton>();
    button->text = text;
    m_currentRow.push_back(std::move(button));
    return *this;
}

Utils::KeyboardBuilder::ReplyBuilder &Utils::KeyboardBuilder::ReplyBuilder::newRow()
{
    if (!m_currentRow.empty()) {
        m_rows.push_back(std::move(m_currentRow));
    }
    return *this;
}

Utils::KeyboardBuilder::ReplyBuilder &Utils::KeyboardBuilder::ReplyBuilder::resizeKeyboard(bool resize)
{
    m_resizeKeyboard = resize;
    return *this;
}

Utils::KeyboardBuilder::ReplyBuilder &Utils::KeyboardBuilder::ReplyBuilder::oneTimeKeyboard(bool one_time)
{
    m_oneTimeKeyboard = one_time;
    return *this;
}

Utils::KeyboardBuilder::ReplyBuilder &Utils::KeyboardBuilder::ReplyBuilder::selective(bool selective)
{
    m_selective = selective;
    return *this;
}

TgBot::ReplyKeyboardMarkup::Ptr Utils::KeyboardBuilder::ReplyBuilder::build() const
{
    auto keyboard = std::make_shared<TgBot::ReplyKeyboardMarkup>();

    keyboard->keyboard = m_rows;
    if (!m_currentRow.empty())
    {
        keyboard->keyboard.push_back(m_currentRow);
    }
    keyboard->resizeKeyboard = m_resizeKeyboard;
    keyboard->oneTimeKeyboard = m_oneTimeKeyboard;
    keyboard->selective = m_selective;

    return keyboard;
}

TgBot::InlineKeyboardMarkup::Ptr Utils::KeyboardBuilder::oneInlineButton(const std::string &text, const std::string &callbackData)
{
    return InlineBuilder()
        .addButton(text, callbackData)
        .build();
}

TgBot::InlineKeyboardMarkup::Ptr Utils::KeyboardBuilder::twoInlineButtons(const std::string &text1, const std::string &data1, const std::string &text2, const std::string &data2)
{
    return InlineBuilder()
        .addButton(text1, data1)
        .addButton(text2, data2)
        .build();
}

TgBot::ReplyKeyboardMarkup::Ptr Utils::KeyboardBuilder::oneReplyButton(const std::string &text)
{
    return ReplyBuilder()
        .addButton(text)
        .build();
}

TgBot::ReplyKeyboardMarkup::Ptr Utils::KeyboardBuilder::twoReplyButtons(const std::string &text1, const std::string &text2)
{
    return ReplyBuilder()
        .addButton(text1)
        .addButton(text2)
        .build();
}

TgBot::ReplyKeyboardRemove::Ptr Utils::KeyboardBuilder::removeKeyboard()
{
    auto keyboard = std::make_shared<TgBot::ReplyKeyboardRemove>();
    keyboard->removeKeyboard = true;
    return keyboard;
}

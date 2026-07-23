#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <unordered_set>

using json = nlohmann::json;

namespace Utils
{
    class TranslationClient
    {
    public:

        static inline const std::string URL = "http://127.0.0.1:8000";
        static inline const std::unordered_set<std::string> SUPPORTED_LANGUAGES = {
            "ab", "af", "ak", "am", "ar", "as", "ay", "az", "ba", "be",
            "bg", "bh", "bi", "bn", "bo", "br", "bs", "ca", "ce", "ch",
            "co", "cr", "cs", "cy", "da", "de", "dv", "dz", "ee", "el",
            "en", "eo", "es", "et", "eu", "fa", "fi", "fj", "fo", "fr",
            "fy", "ga", "gd", "gl", "gn", "gu", "ha", "he", "hi", "hr",
            "ht", "hu", "hy", "ia", "id", "ig", "is", "it", "ja", "jv",
            "ka", "kk", "km", "kn", "ko", "ku", "ky", "la", "lb", "lg",
            "ln", "lo", "lt", "lv", "mg", "mi", "mk", "ml", "mn", "mr",
            "ms", "mt", "my", "na", "ne", "nl", "no", "ny", "oc", "om",
            "or", "pa", "pl", "ps", "pt", "ro", "ru", "rw", "sa", "sc",
            "sd", "se", "sg", "si", "sk", "sl", "sm", "sn", "so", "sq",
            "sr", "ss", "st", "su", "sv", "sw", "ta", "te", "tg", "th",
            "ti", "tk", "tl", "tn", "to", "tr", "ts", "tt", "tw", "ty",
            "ug", "uk", "ur", "uz", "vi", "vo", "wa", "wo", "xh", "yi",
            "yo", "za", "zh", "zu"
        };

        static std::string translate(const std::string& text, const std::string& target = "ru");
        static bool isValidLang(const std::string& lang);
        static bool isAvailable();

    private:

        TranslationClient() = delete;
        ~TranslationClient() = delete;
        TranslationClient(const TranslationClient&) = delete;
        TranslationClient& operator=(const TranslationClient&) = delete;

    private:

        static size_t m_writeCallback(void* contents, size_t size, size_t nmemb, std::string* output);
        static std::string m_postRequest(const json& payload);

    };
}

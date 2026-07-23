#include "TranslationClient.hpp"
#include "../logger/Logger.hpp"

std::string Utils::TranslationClient::translate(const std::string& text, const std::string& target)
{
    auto& logger = Logger::getInstance();

    if (m_disableTranslation.load())
    {
        logger.warning("Translation state: disabled", "TRANSLATE");
        return text;
    }

    if (text.empty()) {
        logger.warning("Translation: Empty text received", "TRANSLATE");
        return text;
    }

    if (text.find_first_not_of(" \t\n\r") == std::string::npos) {
        logger.warning("Translation: Text contains only whitespace", "TRANSLATE");
        return text;
    }

    if (target.size() != 2) {
        logger.warning("Translation: Invalid language code \"" + target + "\" (must be 2 letters)", "TRANSLATE");
        return text;
    }

    json payload = {
        {"text", text},
        {"target", target}
    };

    std::string response = m_postRequest(payload);

    if (response.find("Error:") == 0) {
        logger.error("Translation: POST request failed - " + response, "TRANSLATE");
        return text;
    }

    try {
        json result = json::parse(response);

        if (result.contains("success") && !result["success"].get<bool>()) {
            std::string error = result.value("error", "Unknown error");
            logger.error("Translation: Server error - " + error, "TRANSLATE");
            return text;
        }

        if (!result.contains("translation")) {
            logger.error("Translation: No translation field in response", "TRANSLATE");
            return text;
        }

        std::string translation = result["translation"].get<std::string>();

        if (translation.empty()) {
            logger.warning("Translation: Empty translation received", "TRANSLATE");
            return text;
        }

        return translation;
    }
    catch (const json::parse_error& e) {
        logger.error("Translation: Failed to parse JSON - " + std::string(e.what()), "TRANSLATE");
        return text;
    }
    catch (const json::type_error& e) {
        logger.error("Translation: Invalid JSON type - " + std::string(e.what()), "TRANSLATE");
        return text;
    }
    catch (const std::exception& e) {
        logger.error("Translation: Exception - " + std::string(e.what()), "TRANSLATE");
        return text;
    }
    catch (...) {
        logger.error("Translation: Unknown exception", "TRANSLATE");
        return text;
    }
}

bool Utils::TranslationClient::isValidLang(const std::string &lang)
{
    if (lang.size() != 2) {
        return false;
    }

    return SUPPORTED_LANGUAGES.find(lang) != SUPPORTED_LANGUAGES.end();
}

bool Utils::TranslationClient::isAvailable()
{
    CURL* curl = curl_easy_init();

    if (!curl) {
        return false;
    }

    std::string response;
    std::string url = URL + "/health";

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 2L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &TranslationClient::m_writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        return false;
    }

    try {
        json result = json::parse(response);
        return result.contains("status") && result["status"] == "ok";
    } catch (...) {
        return false;
    }
}

void Utils::TranslationClient::enable()
{
    m_disableTranslation.store(false);
}

void Utils::TranslationClient::disable()
{
    m_disableTranslation.store(true);
}

size_t Utils::TranslationClient::m_writeCallback(void *contents, size_t size, size_t nmemb, std::string *output)
{
    size_t total = size * nmemb;
    output->append((char*)contents, total);
    return total;
}

std::string Utils::TranslationClient::m_postRequest(const json &payload)
{
    CURL* curl = curl_easy_init();

    if (!curl)
    {
        return "Error: curl init failed";
    }

    std::string url = URL + "/translate";

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    std::string postData = payload.dump();

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postData.size());

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    std::string response{};
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &TranslationClient::m_writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        return "Error: " + std::string(curl_easy_strerror(res));
    }

    return response;
}

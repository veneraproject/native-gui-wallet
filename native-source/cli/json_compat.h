#pragma once

#include <cstdint>
#include <string>
#include <limits>
#include <stdexcept>
#include <json-c/json.h>
#include <json-c/json_c_version.h>

 
 
static inline json_object* venera_json_new_uint64(uint64_t value) {
#if defined(JSON_C_VERSION_NUM) && JSON_C_VERSION_NUM >= 0x000e00
    return json_object_new_uint64(value);
#else
    return json_object_new_string(("__VENERA_UINT64_" + std::to_string(value) + "__").c_str());
#endif
}

static inline uint64_t venera_json_get_uint64(const json_object* value) {
#if defined(JSON_C_VERSION_NUM) && JSON_C_VERSION_NUM >= 0x000e00
    return json_object_get_uint64(value);
#else
    if (value && json_object_get_type(value) == json_type_string) {
        const char* text = json_object_get_string(const_cast<json_object*>(value));
        const std::string prefix = "__VENERA_UINT64_";
        if (text && std::string(text).compare(0, prefix.size(), prefix) == 0) {
            const std::string digits = std::string(text).substr(prefix.size());
            if (digits.size() < 3 || digits.compare(digits.size() - 2, 2, "__") != 0)
                throw std::runtime_error("Invalid unsigned JSON integer");
            const std::string number = digits.substr(0, digits.size() - 2);
            if (number.empty() || number.find_first_not_of("0123456789") != std::string::npos)
                throw std::runtime_error("Invalid unsigned JSON integer");
            try { return static_cast<uint64_t>(std::stoull(number)); }
            catch (...) { throw std::runtime_error("Unsigned JSON integer is out of range"); }
        }
    }
    const int64_t result = json_object_get_int64(value);
    if (result < 0)
        throw std::runtime_error("Expected a non-negative JSON integer");
    return static_cast<uint64_t>(result);
#endif
}

static inline std::string venera_json_prepare_parse(const std::string& input) {
#if defined(JSON_C_VERSION_NUM) && JSON_C_VERSION_NUM >= 0x000e00
    return input;
#else
    std::string output;
    output.reserve(input.size());
    bool quoted = false, escaped = false;
    for (size_t i = 0; i < input.size();) {
        const char c = input[i];
        if (quoted) {
            output += c;
            if (escaped) escaped = false;
            else if (c == '\\') escaped = true;
            else if (c == '"') quoted = false;
            ++i;
            continue;
        }
        if (c == '"') { quoted = true; output += c; ++i; continue; }
        if (c >= '0' && c <= '9') {
            const size_t start = i;
            while (i < input.size() && input[i] >= '0' && input[i] <= '9') ++i;
            if (i < input.size() && (input[i] == '.' || input[i] == 'e' || input[i] == 'E')) {
                while (i < input.size() && input[i] != ',' && input[i] != '}' && input[i] != ']' && input[i] != ' ' && input[i] != '\n' && input[i] != '\r' && input[i] != '\t') ++i;
                output.append(input, start, i - start);
                continue;
            }
            const std::string digits = input.substr(start, i - start);
            const std::string maxSigned = "9223372036854775807";
            if (digits.size() > maxSigned.size() || (digits.size() == maxSigned.size() && digits > maxSigned))
                output += "\"__VENERA_UINT64_" + digits + "__\"";
            else output += digits;
            continue;
        }
        output += c;
        ++i;
    }
    return output;
#endif
}

static inline std::string venera_json_restore_uint64(const std::string& input) {
#if defined(JSON_C_VERSION_NUM) && JSON_C_VERSION_NUM >= 0x000e00
    return input;
#else
    std::string output = input;
    const std::string prefix = "\"__VENERA_UINT64_";
    size_t at = 0;
    while ((at = output.find(prefix, at)) != std::string::npos) {
        const size_t end = output.find("__\"", at + prefix.size());
        if (end == std::string::npos) break;
        const std::string digits = output.substr(at + prefix.size(), end - (at + prefix.size()));
        if (digits.empty() || digits.find_first_not_of("0123456789") != std::string::npos) { at = end + 3; continue; }
        output.replace(at, end + 3 - at, digits);
        at += digits.size();
    }
    return output;
#endif
}

// text.cpp

#include "text.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>

namespace esphome {
namespace nspanel_easy {

std::string adjustDecimalSeparator(const std::string &input, char decimalSeparator) {
  if (decimalSeparator == '.') {
    return input;
  }

  // Find the end of the numeric part
  size_t numericEnd = 0;
  for (; numericEnd < input.size(); ++numericEnd) {
    const char c = input[numericEnd];
    if (!((c >= '0' && c <= '9') || c == '.' || c == '-' || c == ',')) {
      break;
    }
  }

  // If no numeric part found, return original
  if (numericEnd == 0) {
    return input;
  }

  std::string numericPart = input.substr(0, numericEnd);

  // Validate that numericPart is actually a valid number
  char *end;
  strtod(numericPart.c_str(), &end);  // Result unused, only checking validity

  if (end != numericPart.c_str() && *end == '\0') {
    // Find and replace decimal point
    size_t decimalPointPos = numericPart.find('.');
    if (decimalPointPos != std::string::npos) {
      numericPart[decimalPointPos] = decimalSeparator;
    }

    // Append suffix if any
    if (numericEnd < input.size()) {
      numericPart += input.substr(numericEnd);
    }
    return numericPart;
  }

  return input;
}

std::string wrapText(const std::string &text_to_display, uint8_t line_length_limit, uint8_t bytes_per_char) {
  // Safety check for overly long text
  if (text_to_display.length() > 1000) {
    return "ERROR: Text too long";
  }
  // Invalid parameters check
  if (line_length_limit == 0 || bytes_per_char == 0) {
    return "ERROR: Invalid line length";
  }
  // Early exit for already formatted text
  if (text_to_display.find("\\r") != std::string::npos) {
    return text_to_display;
  }

  const uint16_t max_line_length = line_length_limit * bytes_per_char;
  const uint16_t text_len = static_cast<uint16_t>(text_to_display.length());

  // If text is short enough, no wrapping needed
  if (text_len <= max_line_length) {
    return text_to_display;
  }

  // Pre-allocate string to avoid reallocations - optimize for flash memory
  std::string wrapped_text;
  wrapped_text.reserve(text_len + 20);  // Reserve space for line breaks

  uint16_t start = 0;

  while (start < text_len) {
    // Skip leading spaces
    while (start < text_len && text_to_display[start] == ' ') {
      ++start;
    }

    if (start >= text_len) {
      break;  // End of text reached
    }  // if

    // Find end position
    uint16_t end = start + max_line_length;
    if (end >= text_len) {
      end = text_len;
    } else {
      // Find word boundary by looking backwards for space
      uint16_t word_end = end;
      while (word_end > start && text_to_display[word_end] != ' ') {
        --word_end;
      }

      // If we found a space within reasonable distance, use it
      if (word_end > start) {
        end = word_end;
      }
      // Otherwise force break at max_line_length (handles long words)
    }  // else

    // Add text segment
    wrapped_text.append(text_to_display, start, end - start);

    // Add line break if not at end of text
    if (end < text_len) {
      wrapped_text += "\\r";

      // Skip spaces at the break point to avoid leading spaces on next line
      while (end < text_len && text_to_display[end] == ' ') {
        ++end;
      }
    }  // if

    start = end;
  }  // while

  return wrapped_text;
}

bool isStringInList(const std::string &strToSearch, std::initializer_list<std::string> list) {
  return std::any_of(list.begin(), list.end(), [&strToSearch](const std::string &str) { return strToSearch == str; });
}

uint32_t decode_nextion_icon_utf8(const char *bytes, size_t length) {
  // Nextion icons are encoded as 1-3 byte UTF-8 sequences only
  if (!bytes || length == 0 || length > 3) {
    return 0;  // Invalid: null, empty, or too long for Nextion icon
  }

  unsigned char byte = static_cast<unsigned char>(bytes[0]);
  uint32_t code_point = 0;

  // 1-byte sequence (0x00-0x7F)
  if ((byte & 0x80) == 0x00) {
    code_point = byte;
  }
  // 2-byte sequence (0xC0-0xDF)
  else if ((byte & 0xE0) == 0xC0) {
    if (length < 2 || (bytes[1] & 0xC0) != 0x80) {
      return 0;  // Invalid: insufficient length or bad continuation byte
    }
    code_point = ((byte & 0x1F) << 6) | (bytes[1] & 0x3F);
    // Reject overlong encodings
    if (code_point < 0x80) {
      return 0;
    }
  }
  // 3-byte sequence (0xE0-0xEF)
  else if ((byte & 0xF0) == 0xE0) {
    if (length < 3 || (bytes[1] & 0xC0) != 0x80 || (bytes[2] & 0xC0) != 0x80) {
      return 0;  // Invalid: insufficient length or bad continuation bytes
    }
    code_point = ((byte & 0x0F) << 12) | ((bytes[1] & 0x3F) << 6) | (bytes[2] & 0x3F);
    // Reject overlong encodings and surrogate code points
    if (code_point < 0x800 || (code_point >= 0xD800 && code_point <= 0xDFFF)) {
      return 0;
    }
  }
  // Invalid: 4-byte sequences or invalid leading byte
  else {
    return 0;
  }

  return code_point;
}

void replace_all(std::string &str, const char *token, const char *value) {
  const size_t token_len = strlen(token);
  if (token_len == 0)
    return;  // Nothing to replace
  const size_t value_len = strlen(value);
  size_t pos = 0;
  while ((pos = str.find(token, pos)) != std::string::npos) {
    str.replace(pos, token_len, value);
    pos += value_len;  // Advance past the replacement to avoid infinite loop
  }
}

}  // namespace nspanel_easy
}  // namespace esphome

#include <string>
#include <vector>
#include <charconv>
struct stoi_result {
    bool success = false;
    int value = 0;
    operator bool() const { return success; }
};

static std::string_view trim(std::string_view s) {
    while (!s.empty() && std::isspace((unsigned char)s.front())) s.remove_prefix(1);
    while (!s.empty() && std::isspace((unsigned char)s.back()))  s.remove_suffix(1);
    return s;
}

static int hex(char c) {
    if(c >= '0' && c <= '9') return c - '0';
    if(c >= 'a' && c <= 'f') return c - 'a' + 10;
    if(c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}

static stoi_result stoi(std::string_view s, int base = 10) {
    int result_num;
    auto result =
        std::from_chars(s.data(), s.data() + s.size(), result_num, base);
    if (result.ec == std::errc::invalid_argument) {
        return {};
    }
    return stoi_result{.value = result_num, .success = true};
}

static std::vector<std::string_view> lines(std::string_view s) {
    std::vector<std::string_view> lines;
    while (!s.empty()) {
        size_t pos = s.find_first_of("\r\n");
        lines.push_back(s.substr(0, pos));
        if (pos == std::string_view::npos) break;
        s.remove_prefix((s[pos] == '\r' && s[pos + 1] == '\n') ? pos + 2 : pos + 1);
    }
    return lines;
}
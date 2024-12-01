#pragma once

#include <exception>
#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <map>
#include <set>
#include <queue>
#include <memory>
#include <sstream>
#include <array>
#include <functional>

namespace utils
{

// custom exception
class MyException : public std::runtime_error
{
public:
    MyException(const std::string &msg) : std::runtime_error(msg) {}

    template <typename... Args>
    static MyException make(const char* message, const char *names, Args&&... args);
};




struct FileReader {
    struct LineReader {
        LineReader(const std::string_view& _line) : line(_line) {}
        const std::string_view get() { return line.substr(pos); }

        template<typename... Args>
        bool read(const char* fmt, Args... args);

        std::vector<std::string_view> split(char delim);

     private:
        std::string_view line;
        size_t pos = 0;
    };

    FileReader(const std::filesystem::path& fileName);
    ~FileReader();

    bool nextLine();
    LineReader getLine();
    std::vector<std::string> allLines();

  private:
    FILE* f;
    std::string line;
};


struct StringToIdMap {
    int set(std::string s);
    const std::string& get(int id);
    int get(const std::string& s) { return fwd.at(s); }

  private:
    std::unordered_map<std::string, int> fwd;
    std::unordered_map<int, const std::string> bwd;
};


std::vector<std::pair<int, int>> splitWork(int size);

void runWorkParallel(int size, std::function<void(int start, int end)> worker);

int digits(long long n);

namespace detail
{

template <typename Arg, typename... Args>
void print(std::ostream& os, const char* nl, Arg&& arg, Args&&... args);

template <typename T>
struct Coord2T {
    T i, j;
    constexpr Coord2T() : i{0}, j{0} {}
    constexpr Coord2T(T _i, T _j) :  i{_i}, j{_j} {}

    constexpr Coord2T operator+(const Coord2T& o) const {
        return {i + o.i, j + o.j};
    }
    constexpr Coord2T operator*(int factor) const {
        return {i * factor, j * factor};
    }
    constexpr bool operator==(const Coord2T& o) const {
        return i == o.i && j == o.j;
    }
    constexpr bool operator!=(const Coord2T& o) const {
        return !(*this == o);
    }
    constexpr bool operator<(const Coord2T& o) const {
        return i < o.i || (i == o.i && j < o.j);
    }
};

} // namespace detail

#define PRINT(...)  utils::detail::print(std::cout, #__VA_ARGS__, __VA_ARGS__)
#define EXCEPTION(msg, ...)  utils::MyException::make(msg, #__VA_ARGS__, __VA_ARGS__)

using Coord = detail::Coord2T<int>;
using Dir = detail::Coord2T<int>;

namespace dir {
    static constexpr Dir U = {-1, 0};
    static constexpr Dir D = { 1, 0};
    static constexpr Dir L = { 0,-1};
    static constexpr Dir R = { 0, 1};
    static constexpr std::array<Dir, 4> UDLR = {U, D, L, R};
    static constexpr std::array<Dir, 8> ALL8 = {U, U+L, U+R, D, D+L, D+R, L, R};
    const std::map<char, utils::Dir> char2Dir = { {'R', R}, {'D', D}, {'L', L}, {'U', U}};
};

//-----------------------------------------------------------------------------

template <typename... Args>
MyException MyException::make(const char* message, const char *names, Args&&... args) {
    std::stringstream ss;
    ss << message << "\nDetails:\n";
    detail::print(ss, names, std::forward<Args>(args)...);
    return MyException(ss.str());
}

template<typename... Args>
bool FileReader::LineReader::read(const char* fmt, Args... args) {
    int n = -1;
    const std::string format = std::string(fmt) + "%n";
    if (sscanf(line.data() + pos, format.c_str(), args..., &n) == sizeof...(args) && n != -1) {
        pos += n;
        return true;
    }
    return false;
}

namespace detail
{

void printNextName(std::ostream& os, const char*& p);

template <typename Arg, typename... Args>
void print(std::ostream& os, const char* names, Arg&& arg, Args&&... args) {
    printNextName(os, names); os << std::forward<Arg>(arg) << "\n";
    ((printNextName(os, names), os << std::forward<Args>(args) << "\n"), ...);
}

namespace is_stl_container_impl {
  template <typename T>       struct is_stl_container:std::false_type{};
  template <typename T, std::size_t N> struct is_stl_container<std::array    <T,N>>    :std::true_type{};
  template <typename... Args> struct is_stl_container<std::vector            <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::deque             <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::list              <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::set               <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::map               <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::unordered_map     <Args...>>:std::true_type{};
  // others removed for simplicity. see https://stackoverflow.com/questions/9407367/determine-if-a-type-is-an-stl-container-at-compile-time
}

template <typename T> struct is_stl_container {
  static constexpr bool const value = is_stl_container_impl::is_stl_container<std::decay_t<T>>::value;
};

inline std::size_t hash_combine(std::size_t seed, std::size_t val)
{
    return seed ^ (val + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

} // namespace detail


template <class Container, std::enable_if_t<detail::is_stl_container<Container>::value, bool> = true>
std::ostream& operator<<(std::ostream& os, const Container& c) {
    os << "[";
    for (const auto& el : c) {
        os << el << ", ";
        if constexpr (detail::is_stl_container<decltype(el)>::value || std::is_same_v<decltype(el), std::string>) {
            os << "\n";
        }
    }
    os << "]";
    return os;
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& p) {
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const detail::Coord2T<T>& p) {
    os << "(" << p.i << ", " << p.j << ")";
    return os;
}

// std::ostream& operator<<(std::ostream& os, const Dir& d) {
//     os << "UDLR?"[std::min(d.i * 2 + d.j, 4)];
//     return os;
// }

template <typename... Args>
std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& t) {
    os << "(" << "tuple: todo" << ")";
    return os;
}

} // namespace utils

template <typename T, typename U>
struct std::hash<std::pair<T, U>> {
    size_t operator()(const std::pair<T, U> & x) const {
        return utils::detail::hash_combine(hash<T>()(x.first), hash<U>()(x.second));
    }
};

template <typename T>
struct std::hash<utils::detail::Coord2T<T>> {
    size_t operator()(const utils::detail::Coord2T<T> & x) const {
        std::pair pair = {x.i, x.j};
        return hash<decltype(pair)>()({x.i, x.j});
    }
};
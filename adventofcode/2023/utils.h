#pragma once

#include <exception>
#include <string>
#include <ctime>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <memory>
#include <optional>

namespace utils
{

// adds string constructor capability to std::exception
class MyException : public std::runtime_error
{
public:
    MyException(const std::string &msg) : std::runtime_error(msg.c_str()) {}
};


struct FileReader {
    struct LineReader {
        LineReader(const std::string& _line) : line(_line) {}
        const std::string& get() { return line; }

        template<typename... Args>
        bool read(const char* fmt, Args... args);

     private:
        const std::string& line;
        int pos = 0;
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

  private:
    std::unordered_map<std::string, int> fwd;
    std::unordered_map<int, const std::string*> bwd;
};


inline std::vector<std::pair<int, int>> splitWork(int size);

int digits(long long n);

namespace detail
{
template <typename Arg, typename... Args>
void print(std::ostream& os, const char* nl, Arg&& arg, Args&&... args);
} // namespace detail

#define PRINT(...)  utils::detail::print(std::cout, #__VA_ARGS__, __VA_ARGS__)


static constexpr std::initializer_list<std::pair<int, int>> DIR4 = {{-1,0}, {0,-1}, {1, 0}, {0, 1}};
static constexpr std::initializer_list<std::pair<int, int>> DIR9 = {{-1,0}, {-1,-1}, {0,-1}, {1,-1}, {1, 0}, {1,1}, {0, 1}, {-1,1}};

//-----------------------------------------------------------------------------

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

const char* print_name(std::ostream& os, const char* p);

template <typename Arg, typename... Args>
void print(std::ostream& os, const char* nl, Arg&& arg, Args&&... args) {
    nl = print_name(os, nl); os << std::forward<Arg>(arg) << "\n";
    ((nl = print_name(os, nl), os << std::forward<Args>(args) << "\n"), ...);
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

} // namespace detail


template <class Container, std::enable_if_t<detail::is_stl_container<Container>::value, bool> = true>
std::ostream& operator<<(std::ostream& os, const Container& c) {
    os << "[";
    for (const auto& el : c) {
        os << el << ", ";
    }
    os << "]";
    return os;
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& p) {
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

} // namespace utils
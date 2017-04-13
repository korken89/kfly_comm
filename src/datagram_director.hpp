
#pragma once

#include <vector>
#include <algorithm>
#include <tuple>
#include <mutex>

/**
 * @brief   A class which registers and directs callbacks based on datagram
 *          types.
 */
template < typename... Datagrams >
class datagram_director
{
private:
  /**
   * @brief   Function pointer alias.
   *
   * @tparam Datagram  Type of the datagram.
   */
  template < typename Datagram >
  using function_ptr = void (*)(Datagram);

  /**
   * @brief   Helper alias to define the callback tuple elements.
   *
   * @details Each tuple element consists of a list of function pointers to
   *          functions which takes a specific datagram as argument, and each
   *          list has a corresponding mutex to make it thread safe.
   *
   * @tparam Datagram   Type of the datagram for this tuple element.
   */
  template < typename Datagram >
  using make_element =
      std::pair< std::vector< function_ptr< Datagram > >, std::mutex >;

  /**
   * @brief   The tuple which contains the lists of function pointers and
   *          mutexes.
   */
  std::tuple< make_element< Datagrams >... > _callbacks;

public:
  /**
   * @brief   Registers a function pointer to its corresponding datagram
   *          callback.
   *
   * @param[in] fun   The function pointer to register.
   *
   * @tparam Datagram   Type of the datagram for this tuple element.
   */
  template < typename Datagram >
  void register_callback(function_ptr< Datagram > fun)
  {
    std::lock_guard< std::mutex > lock(
        std::get< make_element< Datagram > >(_callbacks).second);

    std::get< make_element< Datagram > >(_callbacks).first.emplace_back(fun);
  }

  /**
   * @brief   Releases a function pointer from its corresponding datagram
   *          callback.
   *
   * @param[in] fun   The function pointer to release.
   *
   * @tparam Datagram   Type of the datagram for this tuple element.
   */
  template < typename Datagram >
  void release_callback(function_ptr< Datagram > fun)
  {
    std::lock_guard< std::mutex > lock(
        std::get< make_element< Datagram > >(_callbacks).second);

    auto &callbacks = std::get< make_element< Datagram > >(_callbacks).first;

    callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(),
                                   [&](function_ptr< Datagram > l_fun) {
                                     return fun == l_fun;
                                   }),
                    callbacks.end());
  }

  /**
   * @brief   Executes the callbacks related to a specific datagram.
   *
   * @param[in] data   The function pointer to release.
   *
   * @tparam Datagram   Type of the datagram for this tuple element.
   */
  template < typename Datagram >
  void execute_callback(const Datagram &data)
  {
    std::lock_guard< std::mutex > lock(
        std::get< make_element< Datagram > >(_callbacks).second);

    for (auto callback : std::get< make_element< Datagram > >(_callbacks).first)
      callback(data);
  }
};

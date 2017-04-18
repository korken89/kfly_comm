//          Copyright Emil Fresk 2016 - 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <vector>
#include <algorithm>
#include <tuple>
#include <mutex>

/**
 * @brief     A class which registers and directs callbacks based on datagram
 *            types.
 *
 * @details   The datagram director is based on that each registered datagram
 *            type gets its own callback vector (through a little meta
 *            programming, which makes it all happen at compile time), so each
 *            callback vector has its own mutex to only lock it and not the
 *            other callbacks. So in conclusion the data structure looks like
 *            this:
 *
 *            element = pair<
 *                            vector of callbacks for specific datagram,
 *                            access mutex to the vector
 *                          >
 *
 *            callback_list = tuple<
 *                                   element< Datagram 1 >,
 *                                   element< Datagram 2 >,
 *                                   ...,
 *                                   element< Datagram N >
 *                                 >
 *
 * @note      The class is thread safe, each callback vector is protected with
 *            a mutex.
 *
 * @tparam Datagrams    Datagram types to be registered in the datagram
 *                      director for callback handling.
 */
template < typename... Datagrams >
class datagram_director
{
private:
  /**
   * @brief   Meta functions to check if a type is registered in the datagram
   *          director.
   */
  template < typename Comp, typename T, typename... Ts >
  struct exists
      : std::integral_constant< bool, std::is_same< Comp, T >::value |
                                          exists< Comp, Ts... >::value >
  {
  };

  /**
   * @brief   Meta functions to check if a type is registered in the datagram
   *          director.
   */
  template < typename Comp, typename T >
  struct exists< Comp, T >
      : std::integral_constant< bool, std::is_same< Comp, T >::value >
  {
  };

  /**
   * @brief   Check so there is atleast one datagram registered.
   */
  static_assert(sizeof...(Datagrams) > 0,
                "There must be one or more datagrams registered");

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
  void register_callback(function_ptr< Datagram > callback)
  {
    /* Check to the Datagram exists in the tuple. */
    static_assert(exists< Datagram, Datagrams... >::value == true,
                  "The provided datagram is not registered.");

    /* Do a nullptr check. */
    if (callback == nullptr)
      return;
    else
    {
      /* Get the corresponding datagram's mutex and lock it. */
      std::lock_guard< std::mutex > lock(
          std::get< make_element< Datagram > >(_callbacks).second);

      /* Emplace the callback in the corresponding callback vector. */
      std::get< make_element< Datagram > >(_callbacks)
          .first.emplace_back(callback);
    }
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
  void release_callback(function_ptr< Datagram > callback)
  {
    /* Check to the Datagram exists in the tuple. */
    static_assert(exists< Datagram, Datagrams... >::value == true,
                  "The provided datagram is not registered.");

    /* Get the corresponding datagram's mutex and lock it. */
    std::lock_guard< std::mutex > lock(
        std::get< make_element< Datagram > >(_callbacks).second);

    /* Get the callback list, and delete the requested callback. */
    auto &callbacks = std::get< make_element< Datagram > >(_callbacks).first;

    callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(),
                                   [&](function_ptr< Datagram > l_cb) {
                                     return callback == l_cb;
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
    /* Get the corresponding datagram's mutex and lock it. */
    std::lock_guard< std::mutex > lock(
        std::get< make_element< Datagram > >(_callbacks).second);

    /* Call each callback. */
    for (auto callback : std::get< make_element< Datagram > >(_callbacks).first)
      callback(data);
  }
};

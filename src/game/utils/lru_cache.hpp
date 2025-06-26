//
// Created by andy on 6/26/2025.
//

#pragma once

#include <concepts>
#include <functional>
#include <list>
#include <unordered_map>

namespace game {

    template <typename K, typename V, typename U>
    class lru_cache {
    public:
        lru_cache(const std::size_t max_size, const std::function<V*(K, U)>& constructor,
                  const std::function<void(V*, U)>& destructor, U userdata) :
            m_max_size(max_size), m_constructor(constructor), m_destructor(destructor), m_userdata(userdata) {}

        ~lru_cache() { clear_all(); }

        lru_cache(const lru_cache& other)                = delete;
        lru_cache(lru_cache&& other) noexcept            = default;
        lru_cache& operator=(const lru_cache& other)     = delete;
        lru_cache& operator=(lru_cache&& other) noexcept = default;

        void clear_all() {
            m_value_map.clear();
            m_map.clear();

            for (const auto& [key, value] : m_cache_list) {
                m_destructor(value, m_userdata);
            }
            m_cache_list.clear();
        }

        V* get(const K& key) {
            auto it = m_map.find(key);
            if (it == m_map.end()) {
                return load_new(key);
            }

            return touch(it);
        }

        V* get_if_present(const K& key) {
            auto it = m_map.find(key);
            if (it == m_map.end()) {
                return nullptr;
            }

            return touch(it);
        }

        // doesn't affect order
        const std::unordered_map<K, V*>& get_all() const { return m_map; }

    private:
        std::list<std::pair<K, V*>>                                           m_cache_list;
        std::unordered_map<K, typename std::list<std::pair<K, V*>>::iterator> m_map;
        std::unordered_map<K, V*>                                             m_value_map;

        using map_iter = typename std::unordered_map<K, typename std::list<std::pair<K, V*>>::iterator>::iterator;

        std::function<V*(K, U)>    m_constructor;
        std::function<void(V*, U)> m_destructor;
        std::size_t                m_max_size;
        U                          m_userdata;

        V* load_new(const K& key) {
            // assumes the value is new
            V* value = m_constructor(key, m_userdata);

            if (m_cache_list.size() >= m_max_size) {
                remove_oldest();
            }

            insert_new(key, value);
            return value;
        }

        void remove_oldest() {
            auto back = m_cache_list.back();
            m_map.erase(back.first);
            m_value_map.erase(back.first);
            m_destructor(back.second, m_userdata);
        }

        void insert_new(const K& key, V* value) {
            m_value_map[key] = value;
            m_cache_list.emplace_front(key, value);
            m_map[key] = m_cache_list.begin();
        }

        V* touch(const map_iter& it) {
            auto key   = it->first;
            V*   value = it->second->second;
            m_cache_list.erase(it->second);
            m_cache_list.emplace_front(key, value);
            m_map[key] = m_cache_list.begin();
            return value;
        }
    };
} // namespace game

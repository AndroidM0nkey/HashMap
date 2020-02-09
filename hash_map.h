//
// Created by semyon on 03.02.2020.
//

#ifndef LONG_CONTEST_HASHMAP_H
#define LONG_CONTEST_HASHMAP_H

#include <memory>
#include <vector>
#include <list>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType> >
class HashMap {
public:
    using iterator = typename std::list<std::pair<const KeyType, ValueType>>::iterator;
    using const_iterator = typename std::list<std::pair<const KeyType, ValueType>>::const_iterator;
    using value_type = std::pair<const KeyType, ValueType>;
    using value_type_nonconst = std::pair<KeyType, ValueType>;
    const std::size_t default_size = 1024;

    iterator begin() noexcept {
        return nodes.begin();
    }

    const_iterator begin() const noexcept {
        return nodes.begin();
    }

    iterator end() noexcept {
        return nodes.end();
    }

    const_iterator end() const noexcept {
        return nodes.end();
    }

    HashMap(Hash hasher = Hash()) : hasher(hasher) {
        size_table = default_size;
        table.resize(default_size);
    }

    HashMap(std::initializer_list<value_type_nonconst> init, Hash hasher = Hash()) : HashMap() {
        for (auto iter : init) {
            insert(iter);
        }
    }

    template<class IteratorType>
    HashMap(IteratorType bgn, IteratorType end, Hash hasher = Hash()) : HashMap() {
        for (auto i = bgn; i != end; i++) {
            insert(*i);
        }
    }

    std::size_t size() const {
        return nodes.size();
    }

    bool empty() const {
        return nodes.size() == 0;
    }

    const Hash hash_function() const {
        return hasher;
    }

    void insert(value_type insertable) {
        KeyType user_key = insertable.first;
        if (find(user_key) != nodes.end()) {
            return;
        }
        std::size_t cur_hash = hasher(user_key);
        cur_hash %= size_table;
        nodes.push_back(insertable);
        iterator cur_it = nodes.end();
        cur_it--;
        table[cur_hash].push_back(cur_it);
        if (table[cur_hash].size() > size_table / default_size) {
            rebuilt();
        }
    }

    void erase(KeyType user_key) {
        std::size_t cur_hash = hasher(user_key);
        cur_hash %= size_table;
        if (find(user_key) != nodes.end()) {
            typename std::list<iterator>::iterator cur_it = table[cur_hash].begin();
            for (auto i : table[cur_hash]) {
                if (i->first == user_key) {
                    nodes.erase(i);
                    table[cur_hash].erase(cur_it);
                    break;
                }
                cur_it++;
            }
        }
    }

    iterator find(KeyType user_key) noexcept {
        std::size_t cur_hash = hasher(user_key);
        cur_hash %= size_table;
        for (auto iter : table[cur_hash]) {
            KeyType cur_key = iter->first;
            if (cur_key == user_key) {
                return iter;
            }
        }
        return nodes.end();
    }

    const_iterator find(KeyType user_key) const noexcept {
        std::size_t cur_hash = hasher(user_key);
        cur_hash %= size_table;
        for (auto iter : table[cur_hash]) {
            KeyType cur_key = iter->first;
            if (cur_key == user_key) {
                return iter;
            }
        }
        return nodes.end();
    }

    void clear() {
        table.clear();
        size_table = default_size;
        table.resize(default_size);
        nodes.clear();
    }

    const ValueType &at(KeyType user_key) const {
        if (find(user_key) == nodes.end()) {
            throw std::out_of_range("");
        }
        return find(user_key)->second;
    }

    ValueType &operator[](const KeyType user_key) {
        if (find(user_key) == nodes.end()) {
            static ValueType default_;
            insert(value_type_nonconst(user_key, default_));
            return find(user_key)->second;
        }
        return find(user_key)->second;
    }

    HashMap &operator=(const HashMap &user_map) {
        if (this == &user_map) {
            return *this;
        }
        clear();
        for (auto it : user_map) {
            insert(it);
        }
        return *this;
    }

    void initialisation() {
        size_table = default_size;
        table.resize(default_size);
    }


private:
    std::size_t size_table;
    Hash hasher;
    std::list<value_type> nodes;
    std::vector<std::list<iterator> > table;


    void rebuilt() {
        std::size_t sz = size_table * 2;
        table.clear();
        table.resize(sz);
        for (iterator i = nodes.begin(); i != nodes.end(); i++) {
            table[hasher(i->first) % sz].push_back(i);
        }
        size_table *= 2;
    }
};


#endif //LONG_CONTEST_HASHMAP_H

#ifndef SPEEDCIRCULARLIST_H
#define SPEEDCIRCULARLIST_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

constexpr int s_prime = 31;

inline int log2(int x) {
	int ans = 0;
	while (x != 1) {
		x /= 2;
		++ans;
	}
	return ans;
}

template<typename T, int b_prime>
class SpeedCircularLinkedList {
public:
    struct Node {
    	std::map<std::string, T> kv_map;
        int bound = 0;
        Node* next = nullptr;
        Node** fast_search_list = nullptr;
        Node* prev = nullptr;

    	Node(int bound, int fast_search_list_size) : bound(bound) {
            if (fast_search_list_size > 0) {
                fast_search_list = new Node*[fast_search_list_size]();
            }
        }

    	~Node() {
            delete[] fast_search_list;
        }
    };

private:
	Node* head = nullptr;
	int fast_search_list_size = 0;
	int list_size = 0;

	static int GetHashCode(std::string str) {
		long long ans = 0;
		for (auto& ch : str) {
			ans = (ans * s_prime + ch) % b_prime;
		}
		return static_cast<int>((ans + b_prime) % b_prime);
	}

	void BuildFastSearchList() {
        if (!head || list_size <= 0 || fast_search_list_size <= 0) {
            return;
        }
        std::vector<Node*> nodes;
        nodes.reserve(list_size);
        Node* cur = head;
        for (int i = 0; i < list_size; ++i) {
            nodes.push_back(cur);
            cur = cur->next;
        }
        for (int i = 0; i < list_size; ++i) {
            for (int k = 0; k < fast_search_list_size; ++k) {
                nodes[i]->fast_search_list[k] = nodes[(i + (1 << k)) % list_size];
            }
        }
	}

    Node* LocateNode(int code) const {
        if (!head) {
            return nullptr;
        }
        if (code <= head->bound) {
            return head;
        }
        Node* cur = head;
        for (int k = fast_search_list_size - 1; k >= 0; --k) {
            Node* jump = cur->fast_search_list[k];
            if (jump != nullptr && jump->bound < code) {
                cur = jump;
            }
        }
        return cur->next;
    }

public:
    explicit SpeedCircularLinkedList(std::vector<int> node_bounds) {
        list_size = static_cast<int>(node_bounds.size());
        fast_search_list_size = (list_size <= 1) ? 0 : log2(list_size);

        if (list_size == 0) {
            return;
        }

        std::vector<Node*> nodes;
        nodes.reserve(list_size);
        for (int bound : node_bounds) {
            nodes.push_back(new Node(bound, fast_search_list_size));
        }
        head = nodes.front();
        for (int i = 0; i < list_size; ++i) {
            nodes[i]->next = nodes[(i + 1) % list_size];
            nodes[i]->prev = nodes[(i - 1 + list_size) % list_size];
        }

        BuildFastSearchList();
    }

    ~SpeedCircularLinkedList() {
        if (!head) {
            return;
        }
        Node* cur = head;
        for (int i = 0; i < list_size; ++i) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }

    int size() const {
        return list_size;
    }

    void put(std::string str, T value) {
	    int code = GetHashCode(str);
    	// 请不要修改上面这行代码
        Node* target = LocateNode(code);
        if (target != nullptr) {
            target->kv_map[str] = value;
        }
    }

    T get(std::string str) {
    	int code = GetHashCode(str);
	    // 请不要修改上面这行代码
        Node* target = LocateNode(code);
        if (target != nullptr) {
            auto it = target->kv_map.find(str);
            if (it != target->kv_map.end()) {
                return it->second;
            }
        }

     return T();
    }

    void print() {
        if (!head) {
            return;
        }
        Node* cur = head;
        for (int i = 0; i < list_size; ++i) {
            std::cout << "[Node] Bound = " << cur->bound << ", kv_map_size = " << cur->kv_map.size() << '\n';
            cur = cur->next;
        }
    }
};

#endif //SPEEDCIRCULARLIST_H

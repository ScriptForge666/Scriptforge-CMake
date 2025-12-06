// Copyright 2025 Scriptforge
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "ScriptforgeTree.hpp"
#include "ScriptforgeErr.hpp"
#include <algorithm>
#include <memory>
#include <vector>

namespace Scriptforge::Tree {
    //ConstTreeIterator实现部分
    
        //构造函数
        template<typename TreeType>
        ConstTreeIterator<TreeType>::ConstTreeIterator(typename TreeType::nodeptr node, TreeTraversalOrder order)
            : current_node_(node), traversal_order_(order) {
            // 初始化逻辑（如果需要）
        }
        //解引用操作符
        template<typename TreeType>
        typename ConstTreeIterator<TreeType>::reference
            ConstTreeIterator<TreeType>::operator*() const {
            return current_node_->node;
        }
        //箭头操作符
        template<typename TreeType>
        typename ConstTreeIterator<TreeType>::pointer
            ConstTreeIterator<TreeType>::operator->() const {
            return &(current_node_->node);
        }
        //前置递增操作符
        template<typename TreeType>
        ConstTreeIterator<TreeType>&
            ConstTreeIterator<TreeType>::operator++() {
            // 递增逻辑（根据遍历顺序）
            return *this;
        }
        //后置递增操作符
        template<typename TreeType>
        ConstTreeIterator<TreeType>
            ConstTreeIterator<TreeType>::operator++(int) {
            ConstTreeIterator temp = *this;
            ++(*this);
            return temp;
        }
        //相等比较操作符
        template<typename TreeType>
        bool ConstTreeIterator<TreeType>::operator==(const ConstTreeIterator& other) const {
            return current_node_ == other.current_node_;
        }
        //不等比较操作符
        template<typename TreeType>
        bool ConstTreeIterator<TreeType>::operator!=(const ConstTreeIterator& other) const {
            return !(*this == other);
        }
	

	//Tree<T>实现部分
    
        //构造函数
        template<typename T, typename Alloc>
        Tree<T, Alloc>::Tree(const allocator_type & alloc)
            : alloc_(alloc) {
            m_root = create_node(T());
        }

        template<typename T, typename Alloc>
        Tree<T, Alloc>::Tree(const T & node, const allocator_type & alloc)
            : alloc_(alloc) {
            m_root = create_node(node);
        }

        template<typename T, typename Alloc>
        Tree<T, Alloc>::Tree(const Tree<T, Alloc>&other)
            : alloc_(std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.alloc_)) {
            auto deep_copy = [&](auto&& self, const nodeptr& original, const nodeptr& parent) -> nodeptr {
                if (!original) return nullptr;
                nodeptr copy_node = create_node(original->node);
                copy_node->father = parent;
                for (const auto& child : original->children) {
                    auto child_copy = self(self, child, copy_node);
                    if (child_copy) {
                        copy_node->children.push_back(child_copy);
                    }
                }
                return copy_node;
                };
            m_root = deep_copy(deep_copy, other.m_root, nullptr);
        }

        //返回根节点
        template<typename T, typename Alloc>
        typename Tree<T, Alloc>::nodeptr Tree<T, Alloc>::root() const {
            return m_root;
        }

        //删除节点
        template<typename T, typename Alloc>
        typename Tree<T, Alloc>::nodeptr Tree<T, Alloc>::del(nodeptr node) {
            if (!node) throw Scriptforge::Err::Error{ "E0002", "空节点" };
            nodeptr father = node->father.lock();
            if (!father) {
                if (node != m_root) throw Scriptforge::Err::Error{ "E0003", "孤立节点" };
                else m_root.reset();
                return nullptr;
            }
            auto& vec = father->children;
            auto it = std::find(vec.begin(), vec.end(), node);
            if (it == vec.end()) throw Scriptforge::Err::Error{ "E0001", "未发现节点" };

            vec.erase(it);
            return father;
        }

        //添加节点
        template<typename T, typename Alloc>
        typename Tree<T, Alloc>::nodeptr Tree<T, Alloc>::add(nodeptr father) {
            if (!father) throw Scriptforge::Err::Error{ "E0002", "空节点" };
            nodeptr newnode = create_node(T());
            newnode->father = father;
            father->children.push_back(newnode);
            return newnode;
        }

        template<typename T, typename Alloc>
        typename Tree<T, Alloc>::nodeptr Tree<T, Alloc>::add(nodeptr father, T & node) {
            if (!father) throw Scriptforge::Err::Error{ "E0002", "空节点" };
            nodeptr newnode = create_node(node);
            newnode->father = father;
            father->children.push_back(newnode);
            return newnode;
        }

        template<typename T, typename Alloc>
        typename Tree<T, Alloc>::nodeptr Tree<T, Alloc>::add(nodeptr father, const T & node) {
            if (!father) throw Scriptforge::Err::Error{ "E0002", "空节点" };
            nodeptr newnode = create_node(node);
            newnode->father = father;
            father->children.push_back(newnode);
            return newnode;
        }

        //返回分配器
        template<typename T, typename Alloc>
        typename Tree<T, Alloc>::allocator_type Tree<T, Alloc>::get_allocator() const noexcept {
            return alloc_;
        }

        //创建新节点
        template<typename T, typename Alloc>
        template<typename U>
        typename Tree<T, Alloc>::nodeptr
            Tree<T, Alloc>::create_node(U && value) {
            using node_allocator = typename std::allocator_traits<Alloc>
                ::template rebind_alloc<TreeNode>;
            using node_alloc_traits = std::allocator_traits<node_allocator>;
            node_allocator node_alloc(alloc_);
            TreeNode* raw = node_alloc.allocate(1);
            try {
                node_alloc_traits::construct(node_alloc, raw, std::forward<U>(value));
            }
            catch (...) {
                node_alloc.deallocate(raw, 1);
                throw;
            }
            return nodeptr(raw, [&node_alloc](TreeNode* p) noexcept {
                if (p) {
                    node_alloc_traits::destroy(node_alloc, p);
                    node_alloc.deallocate(p, 1);
                }
                });
        }
	
    //显式实例化
    template class Tree<int>;
	template class ConstTreeIterator<Tree<int>>;
}
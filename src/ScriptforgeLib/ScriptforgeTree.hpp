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
#pragma once
#ifndef SCRIPTFORGETREE_HPP
#define SCRIPTFORGETREE_HPP
#include <memory>
#include <vector>

namespace Scriptforge::Tree {
    enum class TreeTraversalOrder {
        PreOrder,
        PostOrder,
        LevelOrder
	};
    template <typename TreeType>
    class ConstTreeIterator {
    public:
		using value_type = typename TreeType::value_type;
        using reference = const value_type&;
        using pointer = const value_type*;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        ConstTreeIterator() = default;
        explicit ConstTreeIterator(typename TreeType::nodeptr node,TreeTraversalOrder order = TreeTraversalOrder::LevelOrder);
        reference operator*() const;
        pointer operator->() const;
        ConstTreeIterator& operator++();
        ConstTreeIterator operator++(int);
        bool operator==(const ConstTreeIterator& other) const;
		bool operator!=(const ConstTreeIterator& other) const;
        private:
        typename TreeType::nodeptr current_node_;
		TreeTraversalOrder traversal_order_;
    };

    template<typename T, typename Alloc = std::allocator<T>>
    class Tree {
    private:
        struct TreeNode;
    public:
        using value_type = T;
        using reference = value_type&;
        using const_reference = const value_type&;
        using size_type = std::size_t;

        using iterator = int;
		using const_iterator = ConstTreeIterator<Tree<T, Alloc>>;
        using difference_type = std::ptrdiff_t;
        using allocator_type = Alloc;
        using nodeptr = std::shared_ptr<TreeNode>;

        //构造函数
        Tree(const allocator_type& alloc = allocator_type());
        explicit Tree(const T& node, const allocator_type& alloc = allocator_type());
        Tree(const Tree<T, Alloc>& other);

        nodeptr root() const;                              //返回根节点
        nodeptr del(nodeptr node);                         //删除节点
        
        //添加节点
        nodeptr add(nodeptr father);
        nodeptr add(nodeptr father, T& node);
        nodeptr add(nodeptr father, const T& node);

        allocator_type get_allocator() const noexcept;     //返回分配器

    private:
        struct TreeNode
        {
            TreeNode() = default;
            explicit TreeNode(T& v) : node(v) {}
            explicit TreeNode(const T& v) : node(v) {}
            std::weak_ptr<TreeNode> father;
            
            std::vector<nodeptr, typename std::allocator_traits<Alloc>::template rebind_alloc<nodeptr>> children;
            T node;
        };

        nodeptr m_root;
        allocator_type alloc_;

        template<typename U>
        nodeptr create_node(U&& value);
    };
}
#endif // !SCRIPTFORGETREE_HPP
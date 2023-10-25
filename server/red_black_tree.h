

#ifndef SERVER_RED_BLACK_TREE_H
#define SERVER_RED_BLACK_TREE_H


#include "binary_search_tree.h"

template<typename tkey, typename tvalue, typename tkey_compare>
class red_black_tree final : public binary_search_tree<tkey, tvalue, tkey_compare>{
public:
    enum class color{
        red,
        black
    };
protected:
    struct node_RB : public binary_search_tree<tkey, tvalue, tkey_compare>::node{
        color _color;

        node_RB() = default;

        node_RB(const tkey& key, const tvalue& value, node_RB* left, node_RB* right, color col) : binary_search_tree<tkey, tvalue, tkey_compare>::node(key, value, left, right){
            _color = col;
        }

        virtual ~node_RB() = default;
    };
protected:
    class insert_class_RB final : public binary_search_tree<tkey, tvalue, tkey_compare>::insert_class{
    public:

        explicit insert_class_RB(red_black_tree<tkey, tvalue, tkey_compare>* tree) : binary_search_tree<tkey, tvalue, tkey_compare>::insert_class(tree){
        }

    private:
        red_black_tree<tkey, tvalue, tkey_compare>* _tree;

    private:
        size_t get_node_size() const override{
            return sizeof(node_RB);
        }

        void get_node_constructor(typename binary_search_tree<tkey, tvalue, tkey_compare>::node** x, const tkey& key, const tvalue& value) const override{
            new (*(reinterpret_cast<node_RB**>(x))) node_RB {key, value, nullptr, nullptr, color::red};
        }

        void insert_after(const tkey& key, const tvalue& value, typename binary_search_tree<tkey, tvalue, tkey_compare>::node** root, std::stack<typename binary_search_tree<tkey, tvalue, tkey_compare>::node*>* way_to_insert, logger* logger) override{
            auto** RB_root = reinterpret_cast<node_RB**>(root);
            auto* RB_way_to_insert = reinterpret_cast<std::stack<node_RB*>*>(way_to_insert);
            if (way_to_insert->size() == 1){
                RB_way_to_insert->top()->_color = color::black;
                if (logger != nullptr) logger->log("Balancing is successful", logger::severity::debug);
                return;
            } else{
                RB_way_to_insert->top()->_color = color::red;
                insert_after_inner(RB_root, RB_way_to_insert, logger);
                logger->log("Balancing is successful", logger::severity::debug);
                return;
            }
        }

        void insert_after_inner(node_RB** root, std::stack<node_RB*>* way_to_insert, logger* logger){
            node_RB* current_node = way_to_insert->top();
            way_to_insert->pop();
            if (way_to_insert->empty()){
                current_node->_color = color::black;
            }else{
                if (way_to_insert->top()->_color != color::black){
                    node_RB* dad = way_to_insert->top();
                    way_to_insert->pop();
                    node_RB* uncle;
                    if (way_to_insert->top()->left_node == dad){
                        uncle = reinterpret_cast<node_RB*>(way_to_insert->top()->right_node);
                    }else{
                        uncle = reinterpret_cast<node_RB*>(way_to_insert->top()->left_node);
                    }
                    if (uncle != nullptr && uncle->_color == color::red){
                        dad->_color = color::black;
                        uncle->_color = color::black;
                        way_to_insert->top()->_color = color::red;
                        insert_after_inner(root, way_to_insert, logger);
                    }else{
                        node_RB* grandparent = way_to_insert->top();
                        node_RB* grand_grandparent;
                        if (way_to_insert->size() > 1){
                            way_to_insert->pop();
                            grand_grandparent = way_to_insert->top();
                        }else{
                            grand_grandparent = nullptr;
                        }
                        if (grandparent->left_node == dad){
                            if (dad->right_node == current_node){
                                _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node**>(&(grandparent->left_node)));
                                if (grand_grandparent != nullptr){
                                    if (grand_grandparent->left_node == grandparent){
                                        _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node**>(&(grand_grandparent->left_node)));
                                    }else{
                                        _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node**>(&(grand_grandparent->right_node)));
                                    }
                                }else{
                                    _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node**>(root));
                                }
                                current_node->_color = color::black;
                                grandparent->_color = color::red;
                            }else{
                                if (grand_grandparent != nullptr){
                                    if (grand_grandparent->left_node == grandparent){
                                        _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node**>(&(grand_grandparent->left_node)));
                                    }else{
                                        _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node**>(&(grand_grandparent->right_node)));
                                    }
                                }else{
                                    _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node**>(root));
                                }
                                dad->_color = color::black;
                                grandparent->_color = color::red;
                            }
                        }else{
                            if (dad->left_node == current_node){
                                _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node**>(&(grandparent->right_node)));
                                if (grand_grandparent != nullptr){
                                    if (grand_grandparent->left_node == grandparent){
                                        _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node**>(&(grand_grandparent->left_node)));
                                    }else{
                                        _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node**>(&(grand_grandparent->right_node)));
                                    }
                                }else{
                                    _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node**>(root));
                                }
                                current_node->_color = color::black;
                                grandparent->_color = color::red;
                            }else{
                                if (grand_grandparent != nullptr){
                                    if (grand_grandparent->left_node == grandparent){
                                        _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node**>(&(grand_grandparent->left_node)));
                                    }else{
                                        _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node**>(&(grand_grandparent->right_node)));
                                    }
                                }else{
                                    _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node**>(root));
                                }
                                dad->_color = color::black;
                                grandparent->_color = color::red;
                            }
                        }
                    }
                }
            }
        }
    };

    class remove_class_RB final : public binary_search_tree<tkey, tvalue, tkey_compare>::remove_class{
    public:
        explicit remove_class_RB(red_black_tree<tkey, tvalue, tkey_compare>* tree) : binary_search_tree<tkey, tvalue, tkey_compare>::remove_class(tree){

        }

    private:
        red_black_tree<tkey, tvalue, tkey_compare>* _tree;

    private:
        void node_destructor(typename binary_search_tree<tkey, tvalue, tkey_compare>::node* node1) const override{
            auto* RB_node = reinterpret_cast<node_RB*>(node1);
            RB_node->~node_RB();
        }

        void additional_working(typename binary_search_tree<tkey, tvalue, tkey_compare>::node* current_node, typename binary_search_tree<tkey, tvalue, tkey_compare>::node* dad, size_t& place, size_t& color_removing) const override{
            auto* dad_RB = reinterpret_cast<node_RB*>(dad);
            auto* current_node_RB = reinterpret_cast<node_RB*>(current_node);
            if (dad_RB->left_node == current_node_RB){
                place = 0;
            }else{
                place = 1;
            }
            if (current_node_RB->_color == color::red){
                color_removing = 0;
            }else{
                color_removing = 1;
            }
        }

    public:
        void remove_after(tkey const& key, typename binary_search_tree<tkey, tvalue, tkey_compare>::node** root, std::stack<typename binary_search_tree<tkey, tvalue, tkey_compare>::node*>* way_to_remove, logger* logger, size_t& place, size_t& color_removing){
            if (logger != nullptr) logger->log("Red black tree remove after has started working", logger::severity::debug);
            auto** RB_root = reinterpret_cast<node_RB**>(root);
            auto* RB_way_to_remove = reinterpret_cast<std::stack<node_RB*>*>(way_to_remove);
            if (*RB_root == nullptr){
                return;
            }
            if ((*RB_root)->left_node == nullptr && (*RB_root)->right_node == nullptr){
                (*RB_root)->_color = color::black;
            }else{
                if (color_removing != 0){
                    if (place == 0){
                        if (reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node*>(RB_way_to_remove->top())->left_node != nullptr){
                            reinterpret_cast<node_RB*>(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node*>(RB_way_to_remove->top())->left_node)->_color = color::black;
                            if (logger != nullptr) logger->log("Red black tree remove after has finished working", logger::severity::debug);
                            return;
                        }else{
                            try{
                                remove_after_inner(RB_root, place, color_removing, RB_way_to_remove, logger);
                            }catch (const std::logic_error& er){
                                if (logger != nullptr) logger->log(er.what(), logger::severity::debug);
                            }
                            if (logger != nullptr) logger->log("Red black tree remove after has finished working", logger::severity::debug);
                            return;
                        }
                    }else{
                        if (reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node*>(RB_way_to_remove->top())->right_node != nullptr){
                            reinterpret_cast<node_RB*>(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node*>(RB_way_to_remove->top())->right_node)->_color = color::black;
                            if (logger != nullptr) logger->log("Red black tree remove after has finished working", logger::severity::debug);
                            return;
                        }else{
                            try{
                                remove_after_inner(RB_root, place, color_removing, RB_way_to_remove, logger);
                            }catch (const std::logic_error& er){
                                if (logger != nullptr) logger->log(er.what(), logger::severity::debug);
                            }
                            if (logger != nullptr) logger->log("Red black tree remove after has finished working", logger::severity::debug);
                            return;
                        }
                    }
                }
            }
            if (logger != nullptr) logger->log("Red black tree remove after has finished working", logger::severity::debug);
        }

    private:
        void remove_after_inner(node_RB** root, size_t& place, size_t& color_removing, std::stack<node_RB*>* way_to_remove, logger* logger){
            node_RB* dad = way_to_remove->top();
            way_to_remove->pop();
            node_RB* brother;
            if (place == 0){
                brother = reinterpret_cast<node_RB*>(dad->right_node);
            }else{
                brother = reinterpret_cast<node_RB*>(dad->left_node);
            }
            if (place == 0){
                if (brother->_color == color::black && dad->_color == color::red && (brother->left_node == nullptr || reinterpret_cast<node_RB*>(brother->left_node)->_color == color::black) && (brother->right_node == nullptr || reinterpret_cast<node_RB*>(brother->right_node)->_color == color::black)){
                    dad->_color = color::black;
                    brother->_color = color::red;
                }else if (brother->_color == color::black && dad->_color == color::red && brother->right_node != nullptr && reinterpret_cast<node_RB*>(brother->right_node)->_color == color::red){
                    dad->_color = color::black;
                    brother->_color = color::red;
                    reinterpret_cast<node_RB*>(brother->right_node)->_color = color::black;
                    if (!way_to_remove->empty()){
                        if (way_to_remove->top()->left_node == dad){
                            _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->left_node)));
                        }else{
                            _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->right_node)));
                        }
                    }else{
                        _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(root));
                    }

                }else if (brother->_color == color::black && dad->_color == color::red && brother->left_node != nullptr && reinterpret_cast<node_RB*>(brother->left_node)->_color == color::red){
                    reinterpret_cast<node_RB*>(brother->left_node)->_color = color::black;
                    brother->_color = color::red;
                    _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node**>(&(dad->right_node)));
                    brother = reinterpret_cast<node_RB*>(dad->right_node);
                    dad->_color = color::black;
                    brother->_color = color::red;
                    reinterpret_cast<node_RB*>(brother->right_node)->_color = color::black;
                    if (!way_to_remove->empty()){
                        if (way_to_remove->top()->left_node == dad){
                            _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->left_node)));
                        }else{
                            _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->right_node)));
                        }
                    }else{
                        _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(root));
                    }

                }else if (brother->_color == color::red && dad->_color == color::black && brother->left_node != nullptr && reinterpret_cast<node_RB*>(brother->left_node)->_color == color::black && (reinterpret_cast<node_RB*>(brother->left_node)->left_node == nullptr || reinterpret_cast<node_RB*>(reinterpret_cast<node_RB*>(brother->left_node)->left_node)->_color == color::black) && (reinterpret_cast<node_RB*>(brother->left_node)->right_node == nullptr || reinterpret_cast<node_RB*>(reinterpret_cast<node_RB*>(brother->left_node)->right_node)->_color == color::black)){
                    brother->_color = color::black;
                    reinterpret_cast<node_RB*>(brother->left_node)->_color = color::red;
                    if (!way_to_remove->empty()){
                        if (way_to_remove->top()->left_node == dad){
                            _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->left_node)));
                        }else{
                            _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->right_node)));
                        }
                    }else{
                        _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(root));
                    }

                }else if (brother->_color == color::red && dad->_color == color::black && brother->left_node != nullptr && reinterpret_cast<node_RB*>(brother->left_node)->_color == color::black && reinterpret_cast<node_RB*>(brother->left_node)->right_node != nullptr && reinterpret_cast<node_RB*>(reinterpret_cast<node_RB*>(brother->left_node)->right_node)->_color == color::red){
                    reinterpret_cast<node_RB*>(reinterpret_cast<node_RB*>(brother->left_node)->right_node)->_color = color::black;
                    _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(dad->right_node)));
                    if (!way_to_remove->empty()){
                        if (way_to_remove->top()->left_node == dad){
                            _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->left_node)));
                        }else{
                            _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->right_node)));
                        }
                    }else{
                        _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(root));
                    }

                }else if (brother->_color == color::red && dad->_color == color::black && brother->left_node != nullptr && reinterpret_cast<node_RB*>(brother->left_node)->_color == color::black && reinterpret_cast<node_RB*>(brother->left_node)->left_node != nullptr && reinterpret_cast<node_RB*>(reinterpret_cast<node_RB*>(brother->left_node)->left_node)->_color == color::red){
                    reinterpret_cast<node_RB*>(brother->left_node)->_color = color::red;
                    reinterpret_cast<node_RB*>(reinterpret_cast<node_RB*>(brother->left_node)->left_node)->_color = color::black;
                    _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(brother->left_node)));
                    reinterpret_cast<node_RB*>(reinterpret_cast<node_RB*>(brother->left_node)->right_node)->_color = color::black;
                    _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(dad->right_node)));
                    if (!way_to_remove->empty()){
                        if (way_to_remove->top()->left_node == dad){
                            _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->left_node)));
                        }else{
                            _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->right_node)));
                        }
                    }else{
                        _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(root));
                    }

                }else if (dad->_color == color::black && brother->_color == color::black && brother->left_node != nullptr && reinterpret_cast<node_RB*>(brother->left_node)->_color == color::red){
                    reinterpret_cast<node_RB*>(brother->left_node)->_color = color::black;
                    _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(dad->right_node)));
                    if (!way_to_remove->empty()){
                        if (way_to_remove->top()->left_node == dad){
                            _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->left_node)));
                        }else{
                            _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->right_node)));
                        }
                    }else{
                        _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(root));
                    }

                }else if (dad->_color == color::black && brother->_color == color::black && brother->right_node != nullptr && reinterpret_cast<node_RB*>(brother->right_node)->_color == color::red){
                    reinterpret_cast<node_RB*>(brother->right_node)->_color = color::black;
                    if (!way_to_remove->empty()){
                        if (way_to_remove->top()->left_node == dad){
                            _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->left_node)));
                        }else{
                            _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->right_node)));
                        }
                    }else{
                        _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(root));
                    }

                }else if (dad->_color == color::black && brother->_color == color::black && (brother->right_node == nullptr || reinterpret_cast<node_RB*>(brother->right_node)->_color == color::black) && (brother->left_node == nullptr || reinterpret_cast<node_RB*>(brother->left_node)->_color == color::black)){
                    brother->_color = color::red;
                    if (!way_to_remove->empty()){
                        if (way_to_remove->top()->left_node == dad){
                            place = 0;
                        }else{
                            place = 1;
                        }
                        try{
                            remove_after_inner(root, place, color_removing, way_to_remove, logger);
                        }catch (const std::logic_error& er){
                            throw er;
                        }
                    }
                }else{
                    throw std::logic_error("Wrong red black tree");
                }

            }else{

                if (brother->_color == color::black && dad->_color == color::red && (brother->left_node == nullptr || reinterpret_cast<node_RB*>(brother->left_node)->_color == color::black) && (brother->right_node == nullptr || reinterpret_cast<node_RB*>(brother->right_node)->_color == color::black)){
                    dad->_color = color::black;
                    brother->_color = color::red;

                }else if (brother->_color == color::black && dad->_color == color::red && brother->left_node != nullptr && reinterpret_cast<node_RB*>(brother->left_node)->_color == color::red){
                    dad->_color = color::black;
                    brother->_color = color::red;
                    reinterpret_cast<node_RB*>(brother->left_node)->_color = color::black;
                    if (!way_to_remove->empty()){
                        if (way_to_remove->top()->left_node == dad){
                            _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->left_node)));
                        }else{
                            _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->right_node)));
                        }
                    }else{
                        _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(root));
                    }

                }else if (brother->_color == color::black && dad->_color == color::red && brother->right_node != nullptr && reinterpret_cast<node_RB*>(brother->right_node)->_color == color::red){
                    reinterpret_cast<node_RB*>(brother->right_node)->_color = color::black;
                    brother->_color = color::red;
                    _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_compare>::node**>(&(dad->left_node)));
                    brother = reinterpret_cast<node_RB*>(dad->left_node);
                    dad->_color = color::black;
                    brother->_color = color::red;
                    reinterpret_cast<node_RB*>(brother->left_node)->_color = color::black;
                    if (!way_to_remove->empty()){
                        if (way_to_remove->top()->left_node == dad){
                            _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->left_node)));
                        }else{
                            _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->right_node)));
                        }
                    }else{
                        _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(root));
                    }

                }else if (brother->_color == color::red && dad->_color == color::black && brother->right_node != nullptr && reinterpret_cast<node_RB*>(brother->right_node)->_color == color::black && (reinterpret_cast<node_RB*>(brother->right_node)->right_node == nullptr || reinterpret_cast<node_RB*>(reinterpret_cast<node_RB*>(brother->right_node)->right_node)->_color == color::black) && (reinterpret_cast<node_RB*>(brother->right_node)->left_node == nullptr || reinterpret_cast<node_RB*>(reinterpret_cast<node_RB*>(brother->right_node)->left_node)->_color == color::black)){
                    brother->_color = color::black;
                    reinterpret_cast<node_RB*>(brother->right_node)->_color = color::red;
                    if (!way_to_remove->empty()){
                        if (way_to_remove->top()->left_node == dad){
                            _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->left_node)));
                        }else{
                            _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->right_node)));
                        }
                    }else{
                        _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(root));
                    }

                }else if (brother->_color == color::red && dad->_color == color::black && brother->right_node != nullptr && reinterpret_cast<node_RB*>(brother->right_node)->_color == color::black && reinterpret_cast<node_RB*>(brother->right_node)->left_node != nullptr && reinterpret_cast<node_RB*>(reinterpret_cast<node_RB*>(brother->right_node)->left_node)->_color == color::red){
                    reinterpret_cast<node_RB*>(reinterpret_cast<node_RB*>(brother->right_node)->left_node)->_color = color::black;
                    _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(dad->left_node)));
                    if (!way_to_remove->empty()){
                        if (way_to_remove->top()->left_node == dad){
                            _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->left_node)));
                        }else{
                            _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->right_node)));
                        }
                    }else{
                        _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(root));
                    }

                }else if (brother->_color == color::red && dad->_color == color::black && brother->right_node != nullptr && reinterpret_cast<node_RB*>(brother->right_node)->_color == color::black && (reinterpret_cast<node_RB*>(brother->right_node)->right_node != nullptr && reinterpret_cast<node_RB*>(reinterpret_cast<node_RB*>(brother->right_node)->right_node)->_color == color::red)){
                    reinterpret_cast<node_RB*>(brother->right_node)->_color = color::red;
                    reinterpret_cast<node_RB*>(reinterpret_cast<node_RB*>(brother->right_node)->right_node)->_color = color::black;
                    _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(brother->right_node)));
                    reinterpret_cast<node_RB*>(reinterpret_cast<node_RB*>(brother->right_node)->left_node)->_color = color::black;
                    _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(dad->left_node)));
                    if (!way_to_remove->empty()){
                        if (way_to_remove->top()->left_node == dad){
                            _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->left_node)));
                        }else{
                            _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->right_node)));
                        }
                    }else{
                        _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(root));
                    }

                }else if (dad->_color == color::black && brother->_color == color::black && brother->right_node != nullptr && reinterpret_cast<node_RB*>(brother->right_node)->_color == color::red){
                    reinterpret_cast<node_RB*>(brother->right_node)->_color = color::black;
                    _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(dad->left_node)));
                    if (!way_to_remove->empty()){
                        if (way_to_remove->top()->left_node == dad){
                            _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->left_node)));
                        }else{
                            _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->right_node)));
                        }
                    }else{
                        _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(root));
                    }
                    //проверенно

                }else if (dad->_color == color::black && brother->_color == color::black && brother->left_node != nullptr && reinterpret_cast<node_RB*>(brother->left_node)->_color == color::red){
                    reinterpret_cast<node_RB*>(brother->left_node)->_color = color::black;
                    if (!way_to_remove->empty()){
                        if (way_to_remove->top()->left_node == dad){
                            _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->left_node)));
                        }else{
                            _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(&(way_to_remove->top()->right_node)));
                        }
                    }else{
                        _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey,tvalue, tkey_compare>::node**>(root));
                    }

                }else if (dad->_color == color::black && brother->_color == color::black && (brother->right_node == nullptr || reinterpret_cast<node_RB*>(brother->right_node)->_color == color::black) && (brother->left_node == nullptr || reinterpret_cast<node_RB*>(brother->left_node)->_color == color::black)){
                    brother->_color = color::red;
                    if (!way_to_remove->empty()){
                        if (way_to_remove->top()->left_node == dad){
                            place = 0;
                        }else{
                            place = 1;
                        }
                        try{
                            remove_after_inner(root, place, color_removing, way_to_remove, logger);
                        }catch (const std::logic_error& er){
                            throw er;
                        }
                    }
                }else{
                    throw std::logic_error("Wrong red black tree");
                }
            }
        }
    };

private:
    void additional_data(typename binary_search_tree<tkey, tvalue, tkey_compare>::node* current_node_copy,typename binary_search_tree<tkey, tvalue, tkey_compare>::node* current_node) const{
        auto* current_node_copy_RB = reinterpret_cast<node_RB*>(current_node_copy);
        auto* current_node_RB = reinterpret_cast<node_RB*>(current_node);
        current_node_copy_RB->_color = current_node_RB->_color;
    }

    size_t node_size() const{
        return sizeof(node_RB);
    }

    void node_construct(typename binary_search_tree<tkey, tvalue, tkey_compare>::node* current_node_copy,typename binary_search_tree<tkey, tvalue, tkey_compare>::node* current_node) const{
        auto* current_node_copy_RB = reinterpret_cast<node_RB*>(current_node_copy);
        auto* current_node_RB = reinterpret_cast<node_RB*>(current_node);
        new (current_node_copy_RB) node_RB {current_node_RB->key, current_node_RB->value, reinterpret_cast<node_RB*>(this->copy_inner(current_node_RB->left_node)), reinterpret_cast<node_RB*>(this->copy_inner(current_node_RB->right_node)), current_node_RB->_color};
    }

public:
    explicit red_black_tree(memory* alloc, logger* logger) : binary_search_tree<tkey, tvalue, tkey_compare>(alloc, logger, nullptr, new typename binary_search_tree<tkey, tvalue, tkey_compare>::find_class(this), new insert_class_RB(this), new remove_class_RB(this)){

    }

    red_black_tree(const red_black_tree<tkey, tvalue, tkey_compare>& other){
        this->_allocator = other._allocator;
        this->_logger = other._logger;
        this->_class_find = new typename binary_search_tree<tkey, tvalue, tkey_compare>::find_class(this);
        this->_class_insert = new insert_class_RB(this);
        this->_class_remove = new remove_class_RB(this);
        this->_root = other.copy();
    }

    red_black_tree<tkey, tvalue, tkey_compare>& operator=(const red_black_tree<tkey, tvalue, tkey_compare>& other){
        if (this != &other){
            this->clear();
            this->_root = other.copy();
        }
        return *this;
    }

    red_black_tree(red_black_tree<tkey, tvalue, tkey_compare>&& other) noexcept {
        this->_root = other._root;
        this->_logger = other._logger;
        this->_allocator = other._allocator;
        this->_class_find = new typename binary_search_tree<tkey, tvalue, tkey_compare>::find_class(this);
        this->_class_remove = new remove_class_RB(this);
        this->_class_insert = new insert_class_RB(this);
        delete other._class_insert;
        delete other._class_find;
        delete other._class_remove;
        other._class_remove = nullptr;
        other._class_find = nullptr;
        other._class_insert = nullptr;
        other._root = nullptr;
        other._logger = nullptr;
        other._allocator = nullptr;
    }

    red_black_tree<tkey, tvalue, tkey_compare>& operator=(red_black_tree<tkey, tvalue, tkey_compare>&& other){
        if (this != &other){
            this->clear();
            this->_root = other._root;
            this->_logger = other._logger;
            this->_allocator = other._allocator;
            delete other._class_insert;
            delete other._class_find;
            delete other._class_remove;
            other._class_remove = nullptr;
            other._class_find = nullptr;
            other._class_insert = nullptr;
            other._root = nullptr;
            other._logger = nullptr;
            other._allocator = nullptr;
        }
        return *this;
    }

    ~red_black_tree(){

    }

private:
//    void debug_print(void* root) const {
//        debug_rb_tree_printing<tkey, tvalue>(reinterpret_cast<void*>(root));
//    }
};


#endif //SERVER_RED_BLACK_TREE_H

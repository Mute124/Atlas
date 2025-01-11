#pragma once
#include <iostream>
#include <functional>

#define ATLAS_DBL_LNK_LST_FUNC_OVERRIDE_DECL(T) \
	void forwardTraverseFunction(Node<T>* current) override; \
    void backwardTraverseFunction(Node<T>* current) override;

namespace Atlas {
/*    template<typename T>
    class DoublyLinkedList {
    public:
        struct Node {
            T data;
            Node* next;
            Node* prev;

            Node(T data) : data(data), next(nullptr), prev(nullptr) {}
        };

        template<typename T>
        using NodeTraverseAction = std::function<void(Node*, T)>;

        DoublyLinkedList() : head(nullptr) {}

        void pushFront(const T& data) {
            Node* new_node = new Node(data);
            if (head == nullptr) {
                head = new_node;
            } else {
                head->prev = new_node;
                new_node->next = head;
                head = new_node;
            }
        }

        void pushBack(const T& data) {
            Node* new_node = new Node(data);
            if (head == nullptr) {
                head = new_node;
            } else {
                Node* current = head;
                while (current->next != nullptr) {
                    current = current->next;
                }
                current->next = new_node;
                new_node->prev = current;
            }
        }

        void insert(int index, const T& data) {
            if (index < 0 || index > size()) {
                throw std::out_of_range("Index out of range");
            }

            Node* new_node = new Node(data);
            if (index == 0) {
                pushFront(data);
            } else if (index == size()) {
                pushBack(data);
            } else {
                Node* current = head;
                for (int i = 0; i < index - 1; i++) {
                    current = current->next;
                }
                new_node->next = current->next;
                new_node->prev = current;
                current->next->prev = new_node;
                current->next = new_node;
            }
        }

        void remove(int index) {
            if (index < 0 || index >= size()) {
                throw std::out_of_range("Index out of range");
            }

            if (index == 0) {
                Node* temp = head;
                head = head->next;
                head->prev = nullptr;
                delete temp;
            } else if (index == size() - 1) {
                Node* current = head;
                while (current->next != nullptr) {
                    current = current->next;
                }
                delete current->prev;
                current->prev = nullptr;
            } else {
                Node* current = head;
                for (int i = 0; i < index; i++) {
                    current = current->next;
                }
                current->prev->next = current->next;
                current->next->prev = current->prev;
                delete current;
            }
        }

        int size() const {
            int count = 0;
            Node* current = head;
            while (current != nullptr) {
                count++;
                current = current->next;
            }
            return count;
        }

        template<typename T>
        void forwardTraverse(NodeTraverseAction<T> action, T arg) const {
            Node* current = this->head;

            while (current != nullptr) {
				action(current, arg);
                current = current->next;
            }
        }
        
        template<typename T>
        void backwardTraverse(NodeTraverseAction<T> action, T arg ) {
            Node* current = this->tail;

            while (current != nullptr) {
                action(current, arg);

                current->prev
            }
        }

        T& operator[](int index) {
            if (index < 0 || index >= size()) {
                throw std::out_of_range("Index out of range");
            }

            Node* current = head;
            for (int i = 0; i < index; i++) {
                current = current->next;
            }
            return current->data;
        }



    private:
        Node* head;
        Node* tail;
    };

	template<typename T>
	using DoublyLinkedListNode = DoublyLinkedList<T>::Node<T>;*/

    // Node structure for the doubly linked list
   /* template<typename T>
    struct Node {
        T data;
        Node* next;
        Node* prev;

        Node(T val) : data(val), next(nullptr), prev(nullptr) {}
    };


    // Doubly linked list class
    template<typename T>
    class DoublyLinkedList {
    private:
        Node<T>* head;
        Node<T>* tail;

    public:
        DoublyLinkedList() : head(nullptr), tail(nullptr) {}

        virtual void forwardTraverseFunction(Node<T>* current) {}
        virtual void backwardTraverseFunction(Node<T>* current) {}

        // Add an element at the end of the list
        void append(T data) {
            Node<T>* newNode = new Node<T>(data);
            if (!head) {
                head = tail = newNode;
            }
            else {
                tail->next = newNode;
                newNode->prev = tail;
                tail = newNode;
            }
        }

        // Add an element at the start of the list
        void prepend(T data) {
            Node<T>* newNode = new Node<T>(data);
            if (!head) {
                head = tail = newNode;
            }
            else {
                newNode->next = head;
                head->prev = newNode;
                head = newNode;
            }
        }

        // Remove an element from the list
        void remove(T data) {
            Node<T>* current = head;
            while (current) {
                if (current->data == data) {
                    if (current->prev)
                        current->prev->next = current->next;
                    else
                        head = current->next;

                    if (current->next)
                        current->next->prev = current->prev;
                    else
                        tail = current->prev;

                    delete current;
                    return;
                }
                current = current->next;
            }
        }

        // Display the list from head to tail
        void displayForward() const {
            Node<T>* current = head;
            while (current) {
                forwardTraverseFunction(current)
                current = current->next;
            }
        }

        // Display the list from tail to head
        void displayBackward() const {
            Node<T>* current = tail;
            while (current) {
                backwardTraverseFunction(current);
                current = current->prev;
            }
        }

        // Destructor to clean up memory
        ~DoublyLinkedList() {
            Node<T>* current = head;
            while (current) {
                Node<T>* next = current->next;
                delete current;
                current = next;
            }
        }
    };

    template<typename T, typename ...Args>
    class FancyDoublyLinkedList : public DoublyLinkedList<T, Args> {
    public:
        FancyDoublyLinkedList() : DoublyLinkedList<T>() {}

        ~FancyDoublyLinkedList() {
			DoublyLinkedList<T, Args>::~DoublyLinkedList();
        }
    };*/
}
#include<iostream>
#include<random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int>distrib(0, 100);

struct ListNode{
    int val;
    ListNode* next = nullptr;
    ListNode* prev = nullptr;
};

struct List {
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
};

//list operations
void createList(List& list, int n){
    for (int i = 0; i<n; i++){
        ListNode* newElem = new ListNode {distrib(gen)};
        if (list.head == nullptr){
            list.head = list.tail = newElem;
        } else {
            list.tail->next = newElem;
            newElem->prev = list.tail;
            list.tail = newElem;
            }
        }
}

void printList(const List& list){
    if (list.head == nullptr) {
        std::cout<<"List is empty\n";
        return;
    }

    //Head->Tail
    std::cout<<"From head to tail: ";
    for(ListNode* p = list.head; p != nullptr; p = p->next){
        std::cout<< p->val;
        if (p->next != nullptr) std::cout<<" => ";
    }
    std::cout<<'\n';

    //Tail->Head
    std::cout<<"From tail to head: ";
    for(ListNode* p = list.tail; p != nullptr; p = p->prev){
        std::cout<< p->val;
        if (p->prev != nullptr) std::cout<<" => ";
    }
    std::cout<<'\n';
}

void deleteList(List& list){
    if(list.head == nullptr){
        std::cout<<"List does not exist. Nothing to delete\n";
        return;
    }
    while(list.head != nullptr){
        ListNode* toDelete = list.head;
        list.head = list.head->next;
        delete toDelete;
    }
    list.tail = nullptr;
    std::cout<<"List was deleted \n";
}

void insertNode(List& list, int position, int value){

    ListNode* newElem = new ListNode{value};

    if(list.head == nullptr){
        list.head = list.tail = newElem;
        std::cout<<"List does not exist. New element will be First\n";
        return;
    }

    if (position == 1){
        newElem->next = list.head;
        list.head->prev = newElem;
        list.head = newElem;
        return;
    }

    ListNode* p = list.head;
    for(int i = 1; i < position; i++){
        if (p -> next == nullptr){
            std::cout<<"Position does not exist. New element pushed back\n";
            list.tail->next = newElem;
            newElem->prev = list.tail;
            list.tail = newElem;
            return;
        }
        p = p->next;
    }
    p->prev->next = newElem;
    newElem->prev = p->prev;

    newElem->next = p;
    p->prev = newElem;
    
}

void deleteNode(List& list, int position){
    if (list.head == nullptr){
        std::cout<<"List does not exist. Nothing to delete\n";
        return;
    }

    ListNode* p = list.head;
    for (int i = 1; i < position; i++){
        if (p->next == nullptr){
            std::cout<<"Position "<<position<< " does not exist. Deleting last node\n";
            break;
        }
        p = p->next;
    }

    if (p->prev != nullptr){
        p->prev->next = p->next;
    } else {
        list.head = p->next;
    }

    if(p->next != nullptr){
        p->next->prev = p->prev;
    } else {
        list.tail = p->prev;
    }

    delete p;
}

void removeDuplicates(List& list){
    if (list.head == nullptr){
        std::cout<<"List is empty\n";
        return;
    }
    bool hasDuplicates = false;

    for (ListNode* p = list.head; p != nullptr; p = p->next){
        ListNode* j = p->next;
        while(j != nullptr){
            ListNode* next = j->next;
            if(j->val == p->val){
                hasDuplicates = true;

                if(j->prev != nullptr){
                    j->prev->next = j->next;
                } else {
                    list.head = j->next;
                }

                if (j->next != nullptr){
                    j->next->prev = j->prev;
                } else {
                    list.tail = j->prev;
                }
                
                delete j;
            }
            j = next;
        }
    }

    if (!hasDuplicates){
        std::cout<<"Duplicates not founded\n";
    } else {
        std::cout<<"Duplicates deleted\n";
    }
    printList(list);
}

//enter data
int enterSize() {
    int n = 0;
    while(n < 2 || n > 100000){
        std::cout<<"Enter size of list[2...100000]: ";
        std::cin>>n;
        if (n < 2){
            std::cout<<"Size must be >= 2\n";
        } else if (n > 100000){
            std::cout<<"Size must be <= 100000\n";
        } 
    }
    return n;
}

int enterDeletingElement() {
    int position = 0;
    while(position < 1){
        std::cout<<"Enter element to delete Position (from 1): ";
        std::cin>>position;
        if(position < 1){
            std::cout<<"Position starts from 1. Enter another position\n";
        }
    }
    return position;
}

std::pair<int, int> enterNewElemData(){
    int position = 0, value;
    
    while(position < 1){
        std::cout<<"Enter position: ";
        std::cin>>position;
        if (position < 1) {
            std::cout<<"Positions start from 1, enter another\n";
        }
    }
    std::cout<<"Enter value: ";
    std::cin>>value;

    std::pair<int, int> mypair = {position, value};
    return mypair;
}

int main(){

    int action = 0;
    List list;

    while(action != 7){
        std::cout<<std::endl;
        std::cout<<"1. Create List with random elements\n";
        std::cout<<"2. Print list\n";
        std::cout<<"3. Delete List\n";
        std::cout<<"4. Insert node\n";
        std::cout<<"5. Delete node\n";
        std::cout<<"6. Task №2\n";
        std::cout<<"7. Quit \n";
        std::cout<<"Enter action: ";
        std::cin>>action;

        switch(action){
            case 1:
                if (list.head == nullptr){
                    int n = enterSize();
                    createList(list, n);
                    printList(list);
                } else {
                    int act = 0;
                    std::cout<<"List is already created. Rewrite it?\n";
                    std::cout<<"1. YES \n";
                    std::cout<<"2. NO \n";
                    std::cout<<"Enter action: ";
                    std::cin>>act;
                    if (act == 1){
                        deleteList(list);
                        int n = enterSize();
                        createList(list, n);
                        printList(list);
                    }
                }
                break;

            case 2: 
                printList(list);
                break;

            case 3:
                deleteList(list);
                break;

            case 4: {
                std::pair<int, int> newElement = enterNewElemData();
                insertNode(list, newElement.first, newElement.second);
                printList(list);
                break;
            }
            case 5: {
                int pos = enterDeletingElement();
                deleteNode(list, pos);
                printList(list);
                break;
            }
            case 6:
                removeDuplicates(list);
                break;
            case 7:
                deleteList(list);
                std::cout<<"Goodbye!";
                return 0;
            default:
                std::cout<<"No action, enter another number \n";
                break;
        }
    }

    deleteList(list);
    return 0;
}
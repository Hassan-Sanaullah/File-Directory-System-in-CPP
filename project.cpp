#include <filesystem>
#include <stack>
#include <vector>
#include <iostream>
#include <queue>
#include <cstring>

using namespace std;
using namespace std::filesystem;

class node{
public:
    path name;
    vector<node *> children;

    node(path name){
        this->name = name;
    }

    ~node()
    {
        for (node *child : children)
        {
            delete child;
        }
    }
};

node *cd_recursion(stack<node *> &directory_stack, path new_directory);
void change_directory(stack<node *> &directory_stack);

path mkdir_recursion(path new_directory, node **current);
void make_directory(node *current);

void print_work_directory(node *current);



void print_work_directory(node *current){
    cout << current->name << endl;
}


path mkdir_recursion(path new_directory, node **current){
    
    if(exists(new_directory)){
        return new_directory;
    }
    else if(!exists(new_directory)){

        mkdir_recursion(new_directory.parent_path(), current);

        node *new_node = new node(new_directory);     
        (*current)->children.push_back(new_node);

        // cout << "Testing: did it make file? : " << create_directory(new_node->name) << endl;
        create_directory(new_node->name);

        for (node *temp: (*current)->children){
            if(temp->name == new_directory)
                (*current) = temp;
        }
        return new_directory;
    }

}

void make_directory(node *current){

    cout << "Enter folder name: " << endl;
    string input_directory;
    cin >> input_directory;

    path new_directory = current->name / input_directory;

    //NOTE: cannot pass non existing path to function, must convert to string
    mkdir_recursion(new_directory.string(), &current);
    
}

void displayDirectoryStructure(node* root, int depth = 0) {
        for (int i = 0; i < depth; ++i) {
            cout << "  "; // Add indentation based on the depth
        }
        cout << "|-- " << root->name.filename() << endl;
        
        for (node* child : root->children) {
            displayDirectoryStructure(child, depth + 1);
        }
}

node * cd_recursion(stack<node*>& directory_stack, path new_directory){
    
    if(directory_stack.top()->name == new_directory){
        return directory_stack.top();
    }
    else{

        cd_recursion(directory_stack, new_directory.parent_path());

        for(node * temp: directory_stack.top()->children){
            if(temp->name == new_directory){
                directory_stack.push(temp);
            }
        }
    }
}

void change_directory(stack<node*>& directory_stack){
    cout << "Change directory to: " << endl;
    string input_directory;
    cin >> input_directory;

    path new_directory = directory_stack.top()->name / input_directory;

    if(new_directory.filename() == ".."){

        while(new_directory.filename() == ".."){
            
            new_directory = new_directory.parent_path();
            directory_stack.pop();
        }
    }
    else{

        cd_recursion(directory_stack, new_directory);
        
    }

}

void scan_all(node *current){
    //incomplete function
    for(directory_entry dir: directory_iterator(current->name)){

    }
}

void copy(node *current){
    string source, destination;
    cin >> source;
    cin >> destination;

    // copy(source, destination, std::filesystem::copy_options::recursive);

}

void search(node *root){
    string search_directory;
    cin >> search_directory;

    queue<node *> BFS_queue;
    BFS_queue.push(root);


    while(!BFS_queue.empty()){

        node *temp = BFS_queue.front();
        if(temp->name.filename() == search_directory){
            cout << "Directory found:" << endl << temp->name << endl;
            break;
        }

        BFS_queue.pop();

        for(node *iteration_node: temp->children){

            BFS_queue.push(iteration_node);
        }
    }
}

void list(node *current){
    cout << "name:" << "\t" << "extension" << endl;
    for(node *temp: current->children){
        cout << temp->name.filename() << "\t" << temp->name.extension() << endl;
    }
}

int main()
{
    path current_directory = current_path() / "root";
    node *root = new node(current_directory);

    node *current = root;

    string choice;
    cout << "Start" << endl;

    stack<node *> directory_stack;
    directory_stack.push(current);
    
    do
    {
        cin >> choice;
        if (choice == "mkdir")
        {
            make_directory(current);
        }
        else if (choice == "pwd"){
            print_work_directory(current);
        }
        else if (choice == "exit")
        {
            break;
        }
        else if (choice == "display")
        {
            displayDirectoryStructure(root, 0);
        }
        else if(choice == "cd"){
            change_directory(directory_stack);
            current = directory_stack.top();
        }
        else if(choice == "clear"){
            system("CLS");
        }
        else if(choice == "ls"){
            list(current);
        }
        else if(choice == "locate"){
            search(root);
        }
        else{
            cout << "Invalid input" << endl;
            continue;
        }   
    } while (choice != "exit");

    return 0;
}

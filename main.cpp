#include <iostream>
#include <vector>

using namespace std;

vector<string> splitPath(string path){
    vector<string> pathList ;
    string temp ;
    for(int i=1; path[i]!='\0'; i++){
        if(path[i]=='/'){
            pathList.push_back(temp) ;
            temp = "" ;
        }
        else
            temp += path[i] ;
    }
    return pathList ;
}

struct Node{
    string name;
    vector<Node*> child ;
    Node(string n) : name(n) {}
};

class Directory{
public:
    Node* root = NULL ;
    Directory(Node* root=NULL){
        this->root = root ;
    }
    Node* traverse(vector<string> pathList);
    void cmd_mkdir(string path, string name) ;
    void cmd_rm(string path) ;
    void cmd_cp(string srcPath, string dstPath) ;
    void cmd_mv(string srcPath, string dstPath) ;
    int cmd_find(string path) ;
    int calculateSub(Node* t) ;
};

Node* Directory::traverse(vector<string> pathList){
    Node* temp = this->root ;
    int pathLen = pathList.size() ;
    int i = 0;
    while(i<pathLen){
        int cLen = temp->child.size() ;
        int j = 0 ;
        while(j<cLen){
            if(pathList[i] == temp->child[j]->name){
                temp = temp->child[j] ;
                break ;
            }
            j++ ;
        }
        i++ ;
    }
    return temp ;
}
void Directory::cmd_mkdir(string path, string name){
    vector<string> pathList = splitPath(path);
    Node* newNode = new Node(name) ;
    Node* temp = traverse(pathList);
    temp->child.push_back(newNode) ;
    cout<<"Node added"<<endl ;
    cout<<"Subdirectories inside given path"<<endl ;
    for(auto s:temp->child){
        cout<<s->name<<" " ;
    }
    cout<<endl ;
}

void Directory::cmd_cp(string srcPath, string dstPath){
    vector<string> srcPathList = splitPath(srcPath), dstPathList = splitPath(dstPath) ;
    Node* copyNode = traverse(srcPathList) ;
    Node* addtoNode = traverse(dstPathList) ;
    Node* nodeToAdd = new Node(copyNode->name);
    nodeToAdd->child = copyNode->child ;
    addtoNode->child.push_back(nodeToAdd) ;
    cout<<"Node copied"<<endl ;
    cout<<"Subdirectories inside parent destination"<<endl ;
    for(auto s:addtoNode->child){
        cout<<s->name<<" ";
    }
    cout<<endl ;
}

void Directory::cmd_rm(string path){
    vector<string> pathList = splitPath(path) ;
    vector<string> pathParent;
    copy(pathList.begin(), pathList.end()-1, back_inserter(pathParent)) ;
    Node* parent = traverse(pathParent);
    Node* childNode = traverse(pathList);
    int cLen = parent->child.size() ;
    int i;
    for(i=0; i<cLen; i++){
        if(parent->child[i]==childNode){
            cout<<"removing node "<<parent->child[i]->name<<endl ;
            parent->child.erase(parent->child.begin()+i) ;
            break;
        }
    }

}

void Directory::cmd_mv(string srcPath, string dstPath){
    vector<string> srcPathList = splitPath(srcPath), dstPathList = splitPath(dstPath) ;
    Node* copyNode = traverse(srcPathList) ;
    cout<<"copying "<<copyNode->name<<endl ;
    Node* addtoNode = traverse(dstPathList) ;
    cout<<"moving to "<<addtoNode->name<<endl ;
    Node* nodeToAdd = new Node(copyNode->name);
    nodeToAdd->child = copyNode->child ;
    addtoNode->child.push_back(nodeToAdd) ;
    cmd_rm(srcPath);
    cout<<"Node Moved"<<endl ;
    cout<<"Subdirectories inside parent destination"<<endl ;
    for(auto s:addtoNode->child){
        cout<<s->name<<" ";
    }
    cout<<endl ;
}

int Directory::calculateSub(Node* t){
    if(!t) return 0 ;
    int subDirs = 0 ;
    subDirs+= t->child.size();

    for(int i=0; i<t->child.size(); i++){
        subDirs+= calculateSub(t->child[i]) ;
    }
    return subDirs ;
}

int Directory::cmd_find(string path){
    vector<string> pathList = splitPath(path) ;
    Node* t = traverse(pathList) ;
    // for each node , add the length of child node
    return calculateSub(t) ;
}

void menu(){
    cout<<"1. Create Directory "<<endl ;
    cout<<"2. Copy Directory "<<endl ;
    cout<<"3. Move Directory "<<endl ;
    cout<<"4. Remove Directory "<<endl ;
    cout<<"5. Find number of subdirectories "<<endl ;
    cout<<"-1 End the program"<<endl ;
}




int main()
{
    //initialize root directory
    Node rootNode = Node("/");
    Directory dir(&rootNode) ;
    menu() ;
    int choice;
    while(1){
        cout<<"Enter choice: " ;
        cin>>choice ;
        if(choice == -1) break ;
        switch(choice){
        case 1:
            {
            string path, name ;
            cout<<"Enter Parent path: " ;
            cin>>path ;
            cout<<"Enter directory name: " ;
            cin>>name ;
            dir.cmd_mkdir(path, name) ;
            break ;
            }

        case 2:
            {
            string srcPath, dstPath ;
            cout<<"Enter source path: ";
            cin>>srcPath ;
            cout<<"Enter destination path: ";
            cin>>dstPath ;
            dir.cmd_cp(srcPath, dstPath) ;
            break ;
            }
        case 3:
            {
            string srcPath, dstPath ;
            cout<<"Enter source path: ";
            cin>>srcPath ;
            cout<<"Enter destination path: ";
            cin>>dstPath ;
            dir.cmd_mv(srcPath, dstPath) ;
            break ;
            }
        case 4:
            {
            string path ;
            cout<<"Enter Path of dir to remove " ;
            cin>>path ;
            dir.cmd_rm(path);
            break;
            }
        case 5:
            {
            string path ;
            cout<<"Enter the path: "<<endl ;
            cin>>path ;
            cout<<"No. of Subdirectories: " ;
            cout<<dir.cmd_find(path)<<endl ;
            break ;
            }
        default:
            {
            cout<<"Invalid choice"<<endl ;
            break;
            }
        }
    }

}

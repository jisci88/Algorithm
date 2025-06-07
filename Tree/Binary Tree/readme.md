# Binary Tree
**unique_ptr을 이용해 메모리 누수 없이 구현**



## Example
```
#include <iostream>
#include "binary_tree.hpp"
using namespace std;

int main() {
    int cnt; cin >> cnt;
    BinaryTree<int> btree{};
    
    while (cnt--) {
        int node; cin >> node;
        btree.insert(node);
    }
    return 0;
}
```


----

## 현재 기능
1. insert() -> 값 추가

## 추가 예정
1. 탐색
2. 삭제

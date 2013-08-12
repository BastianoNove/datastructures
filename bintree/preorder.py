from bintree import BinaryTree

def preorder(node):
    if node is None:
        return
    print(node.key, end= ' ')
    preorder(node.left)
    preorder(node.right)

def preorder_iterative(node):
    stack = [node]
    while stack:
        curr_item = stack.pop()
        print(curr_item.key, end=' ')
        if curr_item.right:
            stack.append(curr_item.right) 
        if curr_item.left:
            stack.append(curr_item.left)

if __name__ == '__main__':
    root = BinaryTree('F')
    root.left = BinaryTree('B')
    root.right = BinaryTree('G')
    root.left.left = BinaryTree('A')
    root.left.right = BinaryTree('D')
    root.left.right.left = BinaryTree('C')
    root.left.right.right = BinaryTree('E')
    root.right.right = BinaryTree('I')
    root.right.right.left = BinaryTree('H')
    preorder(root)
    print()
    preorder_iterative(root)
    


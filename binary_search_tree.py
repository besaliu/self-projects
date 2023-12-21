# -*- coding: utf-8 -*-
"""
# Programming Assignment 5, Problem A, CSE30-02, Winter 2023
## README
* Please **carefully** follow the instructions given in this template
  code
* Only edit within the allowed sections as instructed by the
  comments; you are risking your own assignment grade if you make
  edits outside those sections or deviate from the instructions
* The testing script provided at the end only performs some basic
  tests; it DOES NOT imply anything about your assignment grade
  * You may add your own tesing code in the main function while working
    on your assignment
  * **However**, you will need to remove your own testing code, and
    re-run the original testing script right before you submit the
    code, to ensure your code is free from syntax errors
* If you are using the Colab notebook version, after you finished your work, please
download this notebook as **python** file
  (not notebook file)
  * on menu bar (Colab's menu bar, NOT your browser's menu bar), go to File -> 
Download -> Download .py
"""

#===== Import section begins
# In this assignment, No external libraries are allowed
# DO NOT ADD any import statements
# keep this section as is
#===== Import section ends

#===== Existing classes section begins
# These class(es) are provided as part of the assignment
# DO NOT change or add anything in this section
# keep this section as is
class BinarySearchTree:
    def __init__(self, val: int, left: 'Union[BinarySearchTree, None]'=None, right:
'Union[BinarySearchTree, None]'=None) -> None:
        '''
            Constructor for a node of a BST
            - Parameters:
                - val: the value stored in the respective node
                - left: the left branch of the tree. defaults to None if no branch 
is given
                - right: the right branch of the tree. defaults to None if no 
branch is given
            - Returns:
                - None
        '''
        self.val = val
        self.left = left
        self.right = right
    def __str__(self):
        """
        Str getter function which prints the BST in a String form for visual aid
            - Parameters:
            -None
            - Returns:
            - str representation of BST
        """
        return '{} [{}] [{}]'.format(self.val, self.left, self.right)
pass
#===== Existing classes section ends

#===== Helper classes/functions section begins
# You may add your own classes or functions within this section
# **class** and **function** only!
# any statement that is not encapsulated inside a class or function
# may result in 0 grade
#===== Helper classes/functions section ends

#===== Problem A function begins
# follow the instruction below
# DO NOT change the function signature!
def get_closest_numbers(tree: BinarySearchTree, f: float, p: int) -> 'List[int]': 
#please use a function with this naming convention for consistent function calls 
    def inorder(node):
        nonlocal vals
        if node is None:
            return
        inorder(node.left)
        vals.append(node.val)
        inorder(node.right)
        
    # Store all values in the BST in a list by in-order traversal
    vals = []
    inorder(tree)
    
    # Sort the values by their distance from f
    vals.sort(key=lambda x: abs(f - x))
    
    # Return the first p values in the sorted list
    return vals[:p]

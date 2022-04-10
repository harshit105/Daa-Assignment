#include<bits/stdc++.h>
#define ll long long
using namespace std;

/* Nived Damodaran - 2018B5A71003H
Yash Amin - 2018B5A71044H
Harshit Bansal - 2018B5A70601H
Geet Sahitya Dulam - 2018B4A70797H*/

int points=0,remaining=0,counter=0;

/**
 * @brief defination of a point
 * 
 */
struct point
{
    ///x coordinate 
    double x;
    ///y coordinate
    double y;
    ///check if it's a left end point
    int left;
    ///check if it's a right end point
    int right;
    ///check if it's an intersection point
    int intersection;
    ///index of line segment from the array of line segments which it belongs to.
    int ind1;
    ///in case of an intersection storing the index of second line segment which it belongs to.
    int ind2;
};

/**
 * @brief Segment of a line passing through (x1,y1) and (x2,y2)
 * 
 */
struct segment
{
    ///x coordinate of left end point
    double x1;
    ///y coordinate of left end point
    double y1;
    ///x coordinate of right end point
    double x2;
    ///y coordinate of right end point
    double y2;
    ///point data structure of left end point.
    point p1;
    ///point data structure of right end point.
    point p2;
    ///y coordinate of point of intersection of line segment with current sweep line.
    double y_inter;
};


/**
 * @brief class for status data structure
 * 
 */
class StatusNode
{
    public:
    ///segment data structure of line segment of the node
    segment line;
    ///left pointer.
    StatusNode *left;
    ///right pointer.
    StatusNode *right;
    ///depth of node in the tree.
    int depth;
};

///for storing the latest line segment inserted.
StatusNode* inserted;
///for storing the latest line segment deleted.
StatusNode* deleted;

/**
 * @brief class for queue data structure
 * 
 */
class Node
{
    public:
    ///point data structure of point in the node.
    point p;
    ///left pointer.
    Node *left;
    ///right pointer.
    Node *right;
    ///depth of node.
    int depth;
    int counter;
};

///Root of the tree for status queue.
StatusNode *root2=NULL;
///Root of the tree for event queue.
Node *root1=NULL;
StatusNode* pred;
StatusNode* succ;

/**
 * @brief inorder traversal of status
 * 
 * @param root 
 */
void inOrder1(StatusNode *root);

/**
 * @brief To find y intercept of any line with the sweep line.
 * 
 * @param s segment whose intercept is to be found.
 * @param x x coordinate of segment.
 * @return double 
 */
double finder(segment s,double x)
{
    double y_intercept;
    if(s.x1==s.x2)
    {
        y_intercept=s.y1;
    }
    else
    {
        double m=(s.y2-s.y1)/(s.x2-s.x1);
        double c=s.y1-(m*s.x1);
        y_intercept=(m*x)+c;
    }
    return y_intercept;
}

/**
 * @brief Finding depth of a node in the tree. depth is 1 for leaf
 * 
 * @param t node whose depth is to be found. 
 * @return int 
 */
int depth(Node *t)
{
    if(t==NULL)
    {
        return 0;
    }
    else
    {
        return t->depth;
    }
}

/**
 * @brief Create a node
 * 
 * @param p 
 * @return Node* 
 */
Node* create(point p)
{
    Node* temp=new Node();
    temp->p=p;
    temp->depth=1;
    temp->counter=1;
    temp->left=NULL;
    temp->right=NULL;
    points+=1;
    return temp;
}

/**
 * @brief Left rotation for balancing tree
 * 
 * @param t Left rotate this node.
 * @return Node* 
 */
Node* LeftRotate(Node *t)
{
    Node *temp1=t->right;
    Node *temp2=t->right->left;

    temp1->left=t;
    t->right=temp2;

    t->depth = max(1+depth(t->right),1+depth(t->left));
    temp1->depth = max(depth(temp1->left)+1,depth(temp1->right)+1);
    return temp1;
}

/**
 * @brief Right rotation for balancing
 * 
 * @param t Right rotate this node.
 * @return Node* 
 */
Node* RotateRight(Node *t)
{
    Node *temp1= t->left;
    Node *temp2= t->left->right;

    temp1->right = t;
    t->left = temp2;

    t->depth = max(1+depth(t->right),1+depth(t->left));
    temp1->depth = max(depth(temp1->left)+1,depth(temp1->right)+1);
    return temp1;
}

/**
 * @brief Check if the node is balanced or not
 * 
 * @param t Node whose balance is to be found.
 * @return int 
 */
int check( Node *t)
{
    if(t==NULL)
    {
        return 0;
    }
    else
    {
        return depth(t->left)-depth(t->right);
    }
}

/**
 * @brief insert function
 * 
 * @param t 
 * @param p 
 * @return Node* 
 */
Node* insert(Node *t,point p)
{
    if(t==NULL)
    {
        return create(p);
    }
    if(p.x<t->p.x)
    {
        t->left = insert(t->left,p);
    }
    else if(p.x==t->p.x)
    {
        if(p.y==t->p.y)
        {
            //t->counter++;
            if(t->p.intersection==0)
            {
                t->p.intersection=1;
                t->p.ind2=p.ind1;
                if(t->p.left==1 || p.left==1)
                {
                    t->p.left=1;
                }
                else
                {
                    t->p.left=0;
                }
                if(t->p.right==1 || p.right==1)
                {
                    t->p.right=1;
                }
                else
                {
                    t->p.right=0;
                }
            }
        }
        else
        {
            if(p.y>t->p.y)
            {
                t->left = insert(t->left,p);
            }
            else
            {
                t->right = insert(t->right,p);
            }
        }
        return t;
    }
    else
    {
        t->right = insert(t->right,p);
    }

    t->depth = max(depth(t->left),depth(t->right))+1;

    int balanced = check(t);
    if(balanced>1 && p.x<t->left->p.x)
    {
        return RotateRight(t);
    }

    if(balanced<-1 && p.x>t->right->p.x)
    {
        return LeftRotate(t);
    }

    if(balanced>1 && p.x>t->left->p.x)
    {
        t->left=LeftRotate(t->left);
        return RotateRight(t);
    }

    if(balanced<-1 && p.x<t->right->p.x)
    {
        t->right=RotateRight(t->right);
        return LeftRotate(t);
    }
    return t;
}

/**
 * @brief Finding the minimum node in a subtree
 * 
 * @param t Find min node in t's subtree
 * @return Node* 
 */
Node* minNode(Node* t)
{
    Node* temp=t;
    while(temp->left!=NULL)
    {
        temp=temp->left;
    }
    return temp;
}

/**
 * @brief Finding the maximum node in a subtree
 * 
 * @param t Find max node in t's subtree
 * @return Node* 
 */
Node* maxNode(Node* t)
{
    Node* temp=t;
    while(temp->right!=NULL)
    {
        temp=temp->right;
    }
    return temp;
}

/**
 * @brief Node delete function
 * 
 * @param root 
 * @param p 
 * @return Node* 
 */
Node* deleteNode(Node* root,point p)
{
    if(root==NULL)
    {
        return root;
    }
    if(p.x<root->p.x)
    {
        root->left=deleteNode(root->left,p);
    }
    else if(p.x>root->p.x)
    {
        root->right=deleteNode(root->right,p);
    }
    else
    {
        if(p.y==root->p.y)
        {
            if(root->left==NULL || root->right==NULL)
            {
                Node* temp=NULL;
                if(root->left!=NULL)
                {
                    temp=root->left;
                }
                if(root->right!=NULL)
                {
                    temp=root->right;
                }
                if(temp==NULL)
                {
                    temp=root;
                    root=NULL;
                }
                else
                {
                    *root=*temp;
                }
                free(temp);
            }
            else
            {
                Node *temp=minNode(root->right);
                root->p=temp->p;
                root->counter=temp->counter;
                root->right=deleteNode(root->right,temp->p);
            }
        }
        else
        {
            if(p.y>root->p.y)
            {
                root->left=deleteNode(root->left,p);
            }
            else
            {
                root->right=deleteNode(root->right,p);
            }
        }
    }
    if(root==NULL)
    {
        return root;
    }
    root->depth=1+max(depth(root->left),depth(root->right));
    int balanced=check(root);

    if(balanced>1 && check(root->left)>=0)
    {
        return RotateRight(root);
    }
    if(balanced>1 && check(root->left)<0)
    {
        root->left=LeftRotate(root->left);
        return RotateRight(root);
    }
    if(balanced<-1 && check(root->right)<=0)
    {
        return LeftRotate(root);
    }
    if(balanced<-1 && check(root->right)>0)
    {
        root->right=RotateRight(root->right);
        return LeftRotate(root);
    }
    return root;
}

/**
 * @brief Find the next higher node for the given node
 * 
 * @param root root of the tree.
 * @param t find t's successor.
 * @return Node* 
 */
Node* successor(Node* root,Node* t)
{
    if(root==NULL)
    {
        return root;
    }
    if(t->right!=NULL)
    {
        return minNode(t->right);
    }
    Node* temp=NULL;
    while(root!=NULL)
    {
        if(t->p.x<root->p.x)
        {
            temp=root;
            root=root->left;
        }
        else if(t->p.x>root->p.x)
        {
            root=root->right;
        }
        else
        {
            break;
        }
    }
    return temp;
}

/**
 * @brief Find the next lower node for the given node
 * 
 * @param root root of the tree.
 * @param t find t's predecessor.
 * @param pred store the predecessor.
 * @return Node* 
 */
Node* predecessor(Node* root, Node* t,Node * pred)
{
    if(root==NULL)
    {
        return pred;
    }
    if(root->p.x==t->p.x)
    {
        if(root->left!=NULL)
        {
            return maxNode(root->left);
        }
    }
    else if(t->p.x<root->p.x)
    {
        return predecessor(root->left,t,pred);
    }
    else
    {
        pred=root;
        return predecessor(root->right,t,pred);
    }
    return pred;
}

/**
 * @brief depth for status tree
 * 
 * @param t Node whose depth is to be found.
 * @return int 
 */
int depth1(StatusNode *t)
{
    if(t==NULL)
    {
        return 0;
    }
    else
    {
        return t->depth;
    }
}

/**
 * @brief Find intersection point of two line segments
 * 
 * @param s1 segment 1
 * @param s2 segment 2
 */
void inter(segment s1, segment s2)
{
    point p;
    if(s1.x1==s1.x2 && s2.x1==s2.x2)
    {
        if(s1.x1==s2.x1)
        {
            if(s1.y2==s2.y1)
            {
                p.x=s1.x1;
                p.y=s1.y2;
                p.intersection=1;
                p.ind1=s1.p1.ind1;
                p.ind2=s2.p1.ind1;
                p.left=1;
                p.right=1;
                root1=insert(root1,p);
            }
        }
    }
    else if(s1.x1==s1.x2)
    {
        double m=(s2.y2-s2.y1)/(s2.x2-s2.x1);
        double c=(s2.y1)-(m*s2.x1);
        double y=(m*s1.x1)+c;
        double mi1,mi2,ma1,ma2;
        if(s1.y1<s1.y2)
        {
            mi1=s1.y1;
            ma1=s1.y2;
        }
        else
        {
            mi1=s1.y2;
            ma1=s1.y1;
        }
        if(s2.y1<s2.y2)
        {
            mi2=s2.y1;
            ma2=s2.y2;
        }
        else
        {
            mi2=s2.y2;
            ma2=s2.y1;
        }
        if((y<=ma1 && y>=mi1) && (y<=ma2 && y>=mi2))
        {
            p.x=s1.x1;
            p.y=y;
            p.intersection=1;
            p.left=0;
            p.right=0;
            if((p.x==s1.x1 && p.y==s1.y1) || (p.x==s2.x1 && p.x==s2.y1))
            {
                p.left=1;
            }
            if((p.x==s1.x2 && p.y==s1.y2) || (p.x==s2.x2 && p.x==s2.y2))
            {
                p.right=1;
            }
            p.ind1=s1.p1.ind1;
            p.ind2=s2.p1.ind1;
            root1=insert(root1,p);
        }
    }
    else if(s2.x1==s2.x2)
    {
        double m=(s1.y2-s1.y1)/(s1.x2-s1.x1);
        double c=(s1.y1)-(m*s1.x1);
        double y=(m*s2.x1)+c;
        double mi1,mi2,ma1,ma2;
        if(s1.y1<s1.y2)
        {
            mi1=s1.y1;
            ma1=s1.y2;
        }
        else
        {
            mi1=s1.y2;
            ma1=s1.y1;
        }
        if(s2.y1<s2.y2)
        {
            mi2=s2.y1;
            ma2=s2.y2;
        }
        else
        {
            mi2=s2.y2;
            ma2=s2.y1;
        }
        if((y<=ma1 && y>=mi1) && (y<=ma2 && y>=mi2))
        {
            p.x=s2.x1;
            p.y=y;
            p.intersection=1;
            p.left=0;
            p.right=0;
            if((p.x==s1.x1 && p.y==s1.y1) || (p.x==s2.x1 && p.x==s2.y1))
            {
                p.left=1;
            }
            if((p.x==s1.x2 && p.y==s1.y2) || (p.x==s2.x2 && p.x==s2.y2))
            {
                p.right=1;
            }
            p.ind1=s1.p1.ind1;
            p.ind2=s2.p1.ind1;
            root1=insert(root1,p);
        }
    }
    else
    {
        double m1=(s1.y2-s1.y1)/(s1.x2-s1.x1);
        double m2=(s2.y2-s2.y1)/(s2.x2-s2.x1);
        double c1=s1.y1-(m1*s1.x1);
        double c2=s2.y1-(m2*s2.x1);
        if(m1!=m2)
        {
            double x=(c2-c1)/(m1-m2);
            double y=(m1*x)+c1;
            double mi1,mi2,mi3,mi4,ma1,ma2,ma3,ma4;
            if(s1.y1<s1.y2)
            {
                mi1=s1.y1;
                ma1=s1.y2;
            }
            else
            {
                mi1=s1.y2;
                ma1=s1.y1;
            }
            if(s2.y1<s2.y2)
            {
                mi2=s2.y1;
                ma2=s2.y2;
            }
            else
            {
                mi2=s2.y2;
                ma2=s2.y1;
            }
            if(s1.x1<s1.x2)
            {
                mi3=s1.x1;
                ma3=s1.x2;
            }
            else
            {
                mi3=s1.x2;
                ma3=s1.x1;
            }
            if(s2.x1<s2.x2)
            {
                mi4=s2.x1;
                ma4=s2.x2;
            }
            else
            {
                mi4=s2.x2;
                ma4=s2.x1;
            }
            if((y<=ma1 && y>=mi1) && (y<=ma2 && y>=mi2) && (x<=ma3 && x>=mi3) && (x<=ma4 && x>=mi4))
            {
                p.x=x;
                p.y=y;
                p.intersection=1;
                p.left=0;
                p.right=0;
                if((p.x==s1.x1 && p.y==s1.y1) || (p.x==s2.x1 && p.x==s2.y1))
                {
                    p.left=1;
                }
                if((p.x==s1.x2 && p.y==s1.y2) || (p.x==s2.x2 && p.x==s2.y2))
                {
                    p.right=1;
                }
                p.ind1=s1.p1.ind1;
                p.ind2=s2.p1.ind1;
                root1=insert(root1,p);
            }
        }
    }
    return;
}

/**
 * @brief status create
 * 
 * @param ls 
 * @param x 
 * @param y 
 * @return StatusNode* 
 */
StatusNode* create1(segment ls,double x,double y)
{
    StatusNode* temp=new StatusNode();
    temp->line=ls;
    temp->depth=1;
    temp->left=NULL;
    temp->right=NULL;
    return temp;
}

/**
 * @brief left rotation for balancing
 * 
 * @param t Left rotate node t.
 * @return StatusNode* 
 */
StatusNode* LeftRotate1(StatusNode *t)
{
    StatusNode *temp1=t->right;
    StatusNode *temp2=t->right->left;

    temp1->left=t;
    t->right=temp2;

    t->depth = max(1+depth1(t->right),1+depth1(t->left));
    temp1->depth = max(depth1(temp1->left)+1,depth1(temp1->right)+1);
    return temp1;
}

/**
 * @brief right rotation for balancing
 * 
 * @param t right rotate node t.
 * @return StatusNode* 
 */
StatusNode* RotateRight1(StatusNode *t)
{
    StatusNode *temp1= t->left;
    StatusNode *temp2= t->left->right;

    temp1->right = t;
    t->left = temp2;

    t->depth = max(1+depth1(t->right),1+depth1(t->left));
    temp1->depth = max(depth1(temp1->left)+1,depth1(temp1->right)+1);
    return temp1;
}

/**
 * @brief returns if the node is balanced
 * 
 * @param t find balance of node t.
 * @return int 
 */
int check1(StatusNode *t)
{
    if(t==NULL)
    {
        return 0;
    }
    else
    {
        return depth1(t->left)-depth1(t->right);
    }
}

/**
 * @brief Insert function
 * 
 * @param t 
 * @param ls 
 * @param x 
 * @param y 
 * @return StatusNode* 
 */
StatusNode* insert1(StatusNode *t,segment ls,double x,double y)
{
    if(t==NULL)
    {
        inserted=create1(ls,x,y);
        return inserted;
    }
    if(t->line.x1==t->line.x2)
    {
        t->line.y_inter=t->line.y1;
        if(ls.x1==ls.x2)
        {
            t->left=insert1(t->left,ls,x,y);
        }
        else
        {
            if(y>=t->line.y1)
            {
                t->right=insert1(t->right,ls,x,y);
            }
            else
            {
                t->left=insert1(t->left,ls,x,y);
            }
        }
    }
    else
    {
        double y_intersecter=finder(t->line,x);
        t->line.y_inter=y_intersecter;
        if(abs(y-y_intersecter)<1e-6)
        {
            if(ls.x1==ls.x2)
            {
                t->left=insert1(t->left,ls,x,y);
            }
            else
            {
                double m1=(ls.y1-ls.y2)/(ls.x1-ls.x2);
                double m2=(t->line.y1-t->line.y2)/(t->line.x1-t->line.x2);
                int right=0;
                if(t->line.y2==y && t->line.x2==x)
                {
                    right=1;
                }
                if(right==0)
                {
                    if(m1>m2)
                    {
                        t->right=insert1(t->right,ls,x,y);
                    }
                    else
                    {
                        t->left=insert1(t->left,ls,x,y);
                    }
                }
                else
                {
                    if(m1<m2)
                    {
                        t->right=insert1(t->right,ls,x,y);
                    }
                    else
                    {
                        t->left=insert1(t->left,ls,x,y);
                    }
                }
            }
        }
        else if(y<y_intersecter)
        {
            t->left = insert1(t->left,ls,x,y);
        }
        else
        {
            if(ls.x1==ls.x2)
            {
                if(t->line.x2==x && t->line.y2==y)
                {
                    t->left=insert1(t->left,ls,x,y);
                }
                else
                {
                    t->right=insert1(t->right,ls,x,y);
                }
            }
            else
            {
                t->right = insert1(t->right,ls,x,y);
            }
        }
    }
    t->depth = max(depth1(t->left),depth1(t->right))+1;
    int balanced = check1(t);
    double y1,y2;
    if(balanced>1)
    {
        double y3;
        if(t->line.x1==t->line.x2)
        {
            y3=t->line.y1;
        }
        else
        {
            y3=finder(t->line,x);
        }
        if(t->left->line.x1==t->left->line.x2)
        {
            y1=t->left->line.y1;
        }
        else
        {
            y1=finder(t->left->line,x);
        }
        if(abs(y-y1)<1e-6)
        {
            double slope1,slope2;
            if(t->line.x1==t->line.x2)
            {
                return RotateRight1(t);
            }
            else if(t->left->line.x1==t->left->line.x2)
            {
                t->left=LeftRotate1(t->left);
                return RotateRight1(t);
            }
            else
            {
                slope2=(t->left->line.y1-t->left->line.y2)/(t->left->line.x1-t->left->line.x2);
                slope1=(t->line.y1-t->line.y2)/(t->line.x1-t->line.x2);
                if((slope1>0 && slope2>0 && slope1<slope2) || (slope1==0 && slope2>0) || (slope1<0 && slope2>=0) || (slope1<0 && slope2<0 && abs(slope1)<abs(slope2)))
                {
                    return RotateRight1(t);
                }
                else
                {
                    t->left=LeftRotate1(t->left);
                    return RotateRight1(t);
                }
            }
        }
        if(y<y1)
        {
            return RotateRight1(t);
        }
        else if(y>=y1)
        {
            t->left=LeftRotate1(t->left);
            return RotateRight1(t);
        }
    }
    if(balanced<-1)
    {
        if(t->right->line.x1==t->right->line.x2)
        {
            y2=t->left->line.y2;
        }
        else
        {
            if(t->right!=NULL)
            {
                y2=finder(t->right->line,x);
            }
        }
        if(abs(y-y2)<1e-6)
        {
            double slope1,slope2;
            if(t->line.x1==t->line.x2)
            {
                t->right=RotateRight1(t->right);
                return LeftRotate1(t);
            }
            else if(t->right->line.x1==t->right->line.x2)
            {
                return LeftRotate1(t);
            }
            else
            {
                slope2=(t->right->line.y1-t->right->line.y2)/(t->right->line.x1-t->right->line.x2);
                slope1=(t->line.y1-t->line.y2)/(t->line.x1-t->line.x2);
                if((slope1>0 && slope2>0 && slope1<slope2) || (slope1==0 && slope2>0) || (slope1<0 && slope2>=0) || (slope1<0 && slope2<0 && slope1<slope2))
                {
                    t->right=RotateRight1(t->right);
                    return LeftRotate1(t);
                }
                else
                {
                    return LeftRotate1(t);
                }
            }
        }
        else if(y>y2)
        {
            return LeftRotate1(t);
        }
        else if(y<y2)
        {
            t->right=RotateRight1(t->right);
            return LeftRotate1(t);
        }
    }
    return t;
}

/**
 * @brief find the minimum node in a subtree
 * 
 * @param t find min node in t's subtree.
 * @return StatusNode* 
 */
StatusNode* minNode1(StatusNode* t)
{
    StatusNode* temp=t;
    while(temp->left!=NULL)
    {
        temp=temp->left;
    }
    return temp;
}

/**
 * @brief find the maximum node in a subtree
 * 
 * @param t find max node in t's subtree.
 * @return StatusNode* 
 */
StatusNode* maxNode1(StatusNode* t)
{
    StatusNode* temp=t;
    while(temp->right!=NULL)
    {
        temp=temp->right;
    }
    return temp;
}

/**
 * @brief find the next higher node
 * 
 * @param root root of the tree.
 * @param t find t's successor.
 * @param x 
 * @param y 
 * @return StatusNode* 
 */
StatusNode* successor1(StatusNode* root,StatusNode* t,double x,double y)
{
    if(t->right!=NULL)
    {
        return minNode1(t->right);
    }
    StatusNode* succ=NULL;
    if(abs(t->line.x1-t->line.x2)<1e-6)
    {
        t->line.y_inter=t->line.y1;
    }
    else
    {
        t->line.y_inter=finder(t->line,x);
    }
    int c=0;
    while(root!=NULL && c++<10)
    {
        if(abs(t->line.x1-t->line.x2)<1e-6)
        {
            root->line.y_inter=root->line.y1;
        }
        else
        {
            root->line.y_inter=finder(root->line,x);
        }
        if(abs(t->line.y_inter-root->line.y_inter)<1e-6)
        {
            if(root->line.x1==t->line.x1 && root->line.y1==t->line.y1 && root->line.x2==t->line.x2 && root->line.y2==t->line.y2)
            {
                break;
            }
            else
            {
                if(root->left==NULL)
                {
                    root=root->right;
                }
                else if(root->right==NULL)
                {
                    succ=root;
                    root=root->left;
                }
                else
                {
                    if(root->left->line.x1==root->left->line.x2)
                    {
                        root->left->line.y_inter=root->left->line.y1;
                    }
                    else
                    {
                        root->left->line.y_inter=finder(root->left->line,x);   
                    }
                    if(root->right->line.x1==root->right->line.x2)
                    {
                        root->right->line.y_inter=root->right->line.y1;
                    }
                    else
                    {
                        root->right->line.y_inter=finder(root->right->line,x);   
                    }
                    if(root->left->line.y_inter==t->line.y_inter)
                    {
                        succ=root;
                        root=root->left;
                    }
                    else if(root->right->line.y_inter==t->line.y_inter)
                    {
                        root=root->right;
                    }
                }
            }
        }
        else if(t->line.y_inter < root->line.y_inter)
        {
            succ=root;
            root=root->left;
        }
        else if(t->line.y_inter>root->line.y_inter)
        {
            root=root->right;
        }
    }
    return succ;
}

/**
 * @brief find the next lower node
 * 
 * @param root root of the tree.
 * @param t Find t's predecessor.
 * @param pred store predecessor.
 * @param x 
 * @param y 
 * @return StatusNode* 
 */
StatusNode* predecessor1(StatusNode* root, StatusNode* t,StatusNode * pred,double x, double y)
{

    if(root==NULL)
    {
        return pred;
    }
    if(abs(t->line.x1-t->line.x2)<1e-6)
    {
        t->line.y_inter=t->line.y1;
    }
    else
    {
        t->line.y_inter=finder(t->line,x);
    }

    if(abs(root->line.x1-root->line.x2)<1e-6)
    {
        root->line.y_inter=root->line.y1;
    }
    else
    {
        root->line.y_inter=finder(root->line,x);
    }
    if(abs(t->line.y_inter-root->line.y_inter)<1e-6)
    {
        if(root->line.x1==t->line.x1 && root->line.y1==t->line.y1 && root->line.x2==t->line.x2 && root->line.y2==t->line.y2)
        {
            if(root->left!=NULL)
            {
                return maxNode1(root->left);
            }
        }
        else
        {
            if(root->left!=NULL)
            {
                return predecessor1(root->left,t,pred,x,y);
            }
            else if(root->right!=NULL)
            {
                pred=root;
                return predecessor1(root->right,t,pred,x,y);
            }
        }
    }
    else if(t->line.y_inter<root->line.y_inter)
    {
        return predecessor1(root->left,t,pred,x,y);
    }
    else
    {
        pred=root;
        return predecessor1(root->right,t,pred,x,y);
    }
    return pred;
}


/**
 * @brief inorder traversal of status
 * 
 * @param root 
 */
void inOrder1(StatusNode *root)
{
    if(root!=NULL)
    {
        inOrder1(root->left);
        cout<<"Inorder1 "<<root->line.x1<<" "<<root->line.y1<<" "<<root->line.x2<<" "<<root->line.y2<<" "<<root->depth<<"\n";
        inOrder1(root->right);
    }
}

/**
 * @brief inorder traversal for queue
 * 
 * @param root 
 */
void inOrder(Node *root)
{
    if(root!=NULL)
    {
        inOrder(root->left);
        cout<<"Inorder "<<root->p.x<<" "<<root->p.y<<" "<<root->counter<<"\n";
        inOrder(root->right);
    }
}

/**
 * @brief deletes a status node
 * 
 * @param root 
 * @param ls 
 * @param x 
 * @param y 
 * @return StatusNode* 
 */
StatusNode* deleteStatusNode1(StatusNode* root,segment ls, double x, double y)
{
    double yf;
    StatusNode* pred=NULL;
    StatusNode* succ=NULL;
    if(ls.x1==ls.x2)
    {
        yf=ls.y1;
    }
    else
    {
        yf=finder(ls,x);
    }
    if(root==NULL)
    {
        return root;
    }
    if(root->line.x1==root->line.x2)
    {
        if(ls.x1==root->line.x1 && ls.y1==root->line.y1 && ls.x2==root->line.x2 && ls.y2==root->line.y2)
        {
            if(root->left==NULL || root->right==NULL)
            {
                StatusNode* temp=NULL;
                if(root->left!=NULL)
                {
                    temp=root->left;
                }
                if(root->right!=NULL)
                {
                    temp=root->right;
                }
                if(temp==NULL)
                {
                    temp=root;
                    pred=predecessor1(root2,root,NULL,x,y);
                    succ=successor1(root2,root,x,y);
                    root=NULL;
                }
                else
                {
                    pred=predecessor1(root2,root,NULL,x,y);
                    succ=successor1(root2,root,x,y);
                    *root=*temp;
                }
                if(pred!=NULL && succ!=NULL)
                {
                    inter(pred->line,succ->line);
                }
                free(temp);
            }
            else
            {
                StatusNode *temp=minNode1(root->right);
                root->line=temp->line;
                root->right=deleteStatusNode1(root->right,temp->line,x,y);
            }
        }
        if(root!=NULL)
        {
            if(yf>=root->line.y2)
            {
                root->right=deleteStatusNode1(root->right,ls,x,y);
            }
            else
            {
                root->left=deleteStatusNode1(root->left,ls,x,y);
            }
        }
        else
        {
            return root;
        }
    }
    else
    {
        double y_intersecter=finder(root->line,x);
        if(abs(yf-y_intersecter)<1e-6)
        {
            if(ls.x1==root->line.x1 && ls.y1==root->line.y1 && ls.x2==root->line.x2 && ls.y2==root->line.y2)
            {
                if(root->left==NULL || root->right==NULL)
                {
                    StatusNode* temp=NULL;
                    if(root->left!=NULL)
                    {
                        temp=root->left;
                    }
                    if(root->right!=NULL)
                    {
                        temp=root->right;
                    }
                    if(temp==NULL)
                    {
                        temp=root;
                        pred=predecessor1(root2,root,NULL,x,y);
                        succ=successor1(root2,root,x,y);
                        root=NULL;
                    }
                    else
                    {
                        pred=predecessor1(root2,root,NULL,x,y);
                        succ=successor1(root2,root,x,y);
                        *root=*temp;
                    }
                    deleted=temp;
                    if(pred!=NULL && succ!=NULL)
                    {
                        inter(pred->line,succ->line);
                    }
                    free(temp);
                }
                else
                {
                    StatusNode *temp=minNode1(root->right);
                    root->line=temp->line;
                    root->right=deleteStatusNode1(root->right,temp->line,x,y);
                }
            }
            else
            {
                if(ls.x1==ls.x2)
                {
                    if(root->line.x2==x && root->line.y2==yf)
                    {
                        root->left=deleteStatusNode1(root->left,ls,x,y);
                    }
                    else
                    {
                        root->right=deleteStatusNode1(root->right,ls,x,y);
                    }
                }
                else
                {
                    double m1=(root->line.y1-root->line.y2)/(root->line.x1-root->line.x2);
                    double m2=(ls.y1-ls.y2)/(ls.x1-ls.x2);
                    if((m1>0 && m2>0 && m1<m2) || (m1==0 && m2>0) || (m1<0 && m2>=0) || (m1<0 && m2<0 && m1<m2))
                    {
                        root->left=deleteStatusNode1(root->left,ls,x,y);
                    }
                    else
                    {
                        root->right=deleteStatusNode1(root->right,ls,x,y);
                    }
                }
            }
        }
        else if(yf<y_intersecter)
        {
            root->left=deleteStatusNode1(root->left,ls,x,y);
        }
        else if(yf>y_intersecter)
        {
            root->right=deleteStatusNode1(root->right,ls,x,y);
        }
    }
    if(root!=NULL)
    {
        root->depth=1+max(depth1(root->left),depth1(root->right));
        int balanced=check1(root);

        StatusNode* temproot=NULL;

        if(balanced>1 && check1(root->left)>=0)
        {
            temproot=RotateRight1(root);
        }
        if(balanced>1 && check1(root->left)<0)
        {
            root->left=LeftRotate1(root->left);
            temproot = RotateRight1(root);
        }
        if(balanced<-1 && check1(root->right)<=0)
        {
            temproot = LeftRotate1(root);
        }
        if(balanced<-1 && check1(root->right)>0)
        {
            root->right=RotateRight1(root->right);
            temproot = LeftRotate1(root);
        }
        if(temproot!=NULL)
        {
            root=temproot;
        }
        return root;
    }
    return root;
}



/**
 * @brief main function
 * 
 * @return int 
 */
int main()
{
    ofstream output_file("output_file7.txt");
    ifstream input_file("input_file.txt");
    int n,i;
    double prevx,prevy;
    input_file>>n;
    output_file<<n<<endl;
    int a[n];
    segment line_list[n];
    for(i=0;i<n;i++){
        input_file>>line_list[i].x1>>line_list[i].y1>>line_list[i].x2>>line_list[i].y2;
        output_file<<line_list[i].x1<<" "<<line_list[i].y1<<" "<<line_list[i].x2<<" "<<line_list[i].y2<<"\n";
        if(line_list[i].x1>line_list[i].x2)
        {
            double temp=line_list[i].x1;
            double temp2=line_list[i].y1;
            line_list[i].x1=line_list[i].x2;
            line_list[i].y1=line_list[i].y2;
            line_list[i].x2=temp;
            line_list[i].y2=temp2;
        }
        else if(line_list[i].x1==line_list[i].x2)
        {
            if(line_list[i].y1<line_list[i].y2)
            {
                double temp=line_list[i].y1;
                line_list[i].y1=line_list[i].y2;
                line_list[i].y2=temp;
            }
        }
        point p1,p2;
        p1.x=line_list[i].x1;
        p1.y=line_list[i].y1;
        p1.left=1;
        p1.right=0;
        p1.intersection=0;
        p1.ind1=i;
        line_list[i].p1=p1;
        p2.x=line_list[i].x2;
        p2.y=line_list[i].y2;
        p2.left=0;
        p2.right=1;
        p2.intersection=0;
        p2.ind1=i;
        line_list[i].p2=p2;
        root1=insert(root1,p1);
        root1=insert(root1,p2);
    }
    remaining=points;
    int first=0;
    while(remaining>0)
    {
        Node* temp=minNode(root1);
        if(first==0)
        {
            remaining-=1;
            first=1;
            prevx=temp->p.x;
            prevy=temp->p.y;
            if(temp->p.left==1)
            {
                if(temp->p.intersection==1)
                {
                    if(line_list[temp->p.ind1].x1==temp->p.x && line_list[temp->p.ind1].y1==temp->p.y)
                    {
                        line_list[temp->p.ind1].y_inter=temp->p.y;
                        root2=insert1(root2,line_list[temp->p.ind1],temp->p.x,temp->p.y);
                        pred=predecessor1(root2,inserted,NULL,temp->p.x,temp->p.y);
                        succ=successor1(root2,inserted,temp->p.x,temp->p.y);
                        if(pred!=NULL)
                        {
                            inter(pred->line,inserted->line);
                        }
                        if(succ!=NULL)
                        {
                            inter(succ->line,inserted->line);
                        }
                    }
                    if(line_list[temp->p.ind2].x1==temp->p.x && line_list[temp->p.ind2].y1==temp->p.y)
                    {
                        line_list[temp->p.ind2].y_inter=temp->p.y;
                        root2=insert1(root2,line_list[temp->p.ind2],temp->p.x,temp->p.y);
                        StatusNode* pred=predecessor1(root2,inserted,NULL,temp->p.x,temp->p.y);
                        StatusNode* succ=successor1(root2,inserted,temp->p.x,temp->p.y);
                        if(pred!=NULL)
                        {
                            inter(pred->line,inserted->line);
                        }
                        if(succ!=NULL)
                        {
                            inter(succ->line,inserted->line);
                        }
                    }
                }
                else
                {
                    line_list[temp->p.ind1].y_inter=temp->p.y;
                    root2=insert1(root2,line_list[temp->p.ind1],temp->p.x,temp->p.y);
                    StatusNode* pred=predecessor1(root2,inserted,NULL,temp->p.x,temp->p.y);
                    StatusNode* succ=successor1(root2,inserted,temp->p.x,temp->p.y);
                    if(pred!=NULL)
                    {
                        inter(pred->line,inserted->line);
                    }
                    if(succ!=NULL)
                    {
                        inter(succ->line,inserted->line);
                    }
                }
            }
            if(temp->p.right==1)
            {
                if(temp->p.intersection==1)
                {
                    if(line_list[temp->p.ind1].x2==temp->p.x && line_list[temp->p.ind1].y2==temp->p.y)
                    {
                        root2=deleteStatusNode1(root2,line_list[temp->p.ind1],temp->p.x,temp->p.y);
                    }
                    if(line_list[temp->p.ind2].x2==temp->p.x && line_list[temp->p.ind2].y2==temp->p.y)
                    {
                        root2=deleteStatusNode1(root2,line_list[temp->p.ind2],temp->p.x,temp->p.y);
                    }
                }
                else
                {
                    root2=deleteStatusNode1(root2,line_list[temp->p.ind1],temp->p.x,temp->p.y);
                }
            }
            if(temp->p.intersection==1)
            {
                int c1=0,c2=0;
                if((line_list[temp->p.ind2].x1!=temp->p.x || line_list[temp->p.ind2].y1!=temp->p.y) && (line_list[temp->p.ind2].x2!=temp->p.x || line_list[temp->p.ind2].y2!=temp->p.y))
                {
                    c1=1;
                }
                if((line_list[temp->p.ind1].x1!=temp->p.x || line_list[temp->p.ind1].y1!=temp->p.y) && (line_list[temp->p.ind1].x2!=temp->p.x || line_list[temp->p.ind1].y2!=temp->p.y))
                {
                    c2=1;
                }
                if(c1==1 && c2==1)
                {
                    remaining+=1;
                    root2=deleteStatusNode1(root2,line_list[temp->p.ind1],temp->p.x,temp->p.y);
                    root2=deleteStatusNode1(root2,line_list[temp->p.ind2],temp->p.x,temp->p.y);
                    root2=insert1(root2,line_list[temp->p.ind1],temp->p.x,temp->p.y);
                    pred=predecessor1(root2,inserted,NULL,temp->p.x,temp->p.y);
                    succ=successor1(root2,inserted,temp->p.x,temp->p.y);
                    if(pred!=NULL)
                    {
                        inter(pred->line,inserted->line);
                    }
                    if(succ!=NULL)
                    {
                        inter(succ->line,inserted->line);
                    }
                    root2=insert1(root2,line_list[temp->p.ind2],temp->p.x,temp->p.y);
                    pred=predecessor1(root2,inserted,NULL,temp->p.x,temp->p.y);
                    succ=successor1(root2,inserted,temp->p.x,temp->p.y);
                    if(pred!=NULL)
                    {
                        cout<<pred->line.x1<<" pred "<<pred->line.y1<<endl;
                        inter(pred->line,inserted->line);
                    }
                    if(succ!=NULL)
                    {
                        inter(succ->line,inserted->line);
                    }
                }
                counter+=1;
                cout<<"Lines "<<temp->p.ind1+1<<" and "<<temp->p.ind2+1<<" intersect at ( "<<temp->p.x<<" , "<<temp->p.y<<" )"<<endl;
                output_file<<"Lines "<<temp->p.ind1+1<<" and "<<temp->p.ind2+1<<" intersect at ( "<<temp->p.x<<" , "<<temp->p.y<<" )"<<endl;
            }
        }
        else
        {
            if(temp->p.x>prevx || (temp->p.x==prevx && temp->p.y<prevy))
            {
                remaining-=1;
                prevx=temp->p.x;
                prevy=temp->p.y;
                if(temp->p.left==1)
                {
                    if(temp->p.intersection==1)
                    {
                        if(line_list[temp->p.ind1].x1==temp->p.x && line_list[temp->p.ind1].y1==temp->p.y)
                        {
                            line_list[temp->p.ind1].y_inter=temp->p.y;
                            root2=insert1(root2,line_list[temp->p.ind1],temp->p.x,temp->p.y);
                            pred=predecessor1(root2,inserted,NULL,temp->p.x,temp->p.y);
                            succ=successor1(root2,inserted,temp->p.x,temp->p.y);
                            if(pred!=NULL)
                            {
                                inter(pred->line,inserted->line);
                            }
                            if(succ!=NULL)
                            {
                                inter(succ->line,inserted->line);
                            }
                        }
                        if(line_list[temp->p.ind2].x1==temp->p.x && line_list[temp->p.ind2].y1==temp->p.y)
                        {
                            line_list[temp->p.ind2].y_inter=temp->p.y;
                            root2=insert1(root2,line_list[temp->p.ind2],temp->p.x,temp->p.y);
                            StatusNode* pred=predecessor1(root2,inserted,NULL,temp->p.x,temp->p.y);
                            StatusNode* succ=successor1(root2,inserted,temp->p.x,temp->p.y);
                            if(pred!=NULL)
                            {
                                inter(pred->line,inserted->line);
                            }
                            if(succ!=NULL)
                            {
                                inter(succ->line,inserted->line);
                            }
                        }
                    }
                    else
                    {
                        line_list[temp->p.ind1].y_inter=temp->p.y;
                        root2=insert1(root2,line_list[temp->p.ind1],temp->p.x,temp->p.y);
                        StatusNode* pred=predecessor1(root2,inserted,NULL,temp->p.x,temp->p.y);
                        StatusNode* succ=successor1(root2,inserted,temp->p.x,temp->p.y);
                        if(pred!=NULL)
                        {
                            inter(pred->line,inserted->line);
                        }
                        if(succ!=NULL)
                        {
                            inter(succ->line,inserted->line);
                        }
                    }
                }
                if(temp->p.right==1)
                {
                    if(temp->p.intersection==1)
                    {
                        if(line_list[temp->p.ind1].x2==temp->p.x && line_list[temp->p.ind1].y2==temp->p.y)
                        {
                            root2=deleteStatusNode1(root2,line_list[temp->p.ind1],temp->p.x,temp->p.y);
                        }
                        if(line_list[temp->p.ind2].x2==temp->p.x && line_list[temp->p.ind2].y2==temp->p.y)
                        {
                            root2=deleteStatusNode1(root2,line_list[temp->p.ind2],temp->p.x,temp->p.y);
                        }
                    }
                    else
                    {
                        root2=deleteStatusNode1(root2,line_list[temp->p.ind1],temp->p.x,temp->p.y);
                    }
                }
                if(temp->p.intersection==1)
                {
                    int c1=0,c2=0;
                    if((line_list[temp->p.ind2].x1!=temp->p.x || line_list[temp->p.ind2].y1!=temp->p.y) && (line_list[temp->p.ind2].x2!=temp->p.x || line_list[temp->p.ind2].y2!=temp->p.y))
                    {
                        c1=1;
                    }
                    if((line_list[temp->p.ind1].x1!=temp->p.x || line_list[temp->p.ind1].y1!=temp->p.y) && (line_list[temp->p.ind1].x2!=temp->p.x || line_list[temp->p.ind1].y2!=temp->p.y))
                    {
                        c2=1;
                    }
                    if(c1==1 && c2==1)
                    {
                        remaining+=1;
                        root2=deleteStatusNode1(root2,line_list[temp->p.ind1],temp->p.x,temp->p.y);
                        root2=deleteStatusNode1(root2,line_list[temp->p.ind2],temp->p.x,temp->p.y);
                        root2=insert1(root2,line_list[temp->p.ind1],temp->p.x,temp->p.y);
                        pred=predecessor1(root2,inserted,NULL,temp->p.x,temp->p.y);
                        succ=successor1(root2,inserted,temp->p.x,temp->p.y);
                        if(pred!=NULL)
                        {
                            inter(pred->line,inserted->line);
                        }
                        if(succ!=NULL)
                        {
                            inter(succ->line,inserted->line);
                        }
                        root2=insert1(root2,line_list[temp->p.ind2],temp->p.x,temp->p.y);
                        pred=predecessor1(root2,inserted,NULL,temp->p.x,temp->p.y);
                        succ=successor1(root2,inserted,temp->p.x,temp->p.y);
                        if(pred!=NULL)
                        {
                            inter(pred->line,inserted->line);
                        }
                        if(succ!=NULL)
                        {
                            inter(succ->line,inserted->line);
                        }
                    }
                    counter+=1;
                    cout<<"Lines "<<temp->p.ind1+1<<" and "<<temp->p.ind2+1<<" intersect at ( "<<temp->p.x<<" , "<<temp->p.y<<" )"<<endl;
                    output_file<<"Lines "<<temp->p.ind1+1<<" and "<<temp->p.ind2+1<<" intersect at ( "<<temp->p.x<<" , "<<temp->p.y<<" )"<<endl;
                }
            }
        }
        root1=deleteNode(root1,temp->p);
    }
    if(counter==1)
    {
        cout<<"There is 1 intersection point."<<endl;
        output_file<<"There is 1 intersection point."<<endl;
    }
    else
    {
        cout<<"There are "<<counter<<" intersection points."<<endl;
        output_file<<"There are "<<counter<<" intersection points."<<endl;
    }
    return 0;
}
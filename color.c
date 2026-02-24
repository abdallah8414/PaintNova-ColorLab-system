#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =====================================================
   STRUCTURES (KEEP YOUR ORIGINAL DESIGN)
===================================================== */

struct User
{
    int id;
    char username[30];
    char password[30];
    int role;
};

struct Product
{
    int id;
    char name[50];
    char category[30];
    int stock;
    float price;
    struct Product *next;
};

struct CustomerOrder
{
    int id;
    char customerName[50];
    char phone[20];
    char productName[50];
    int quantity;
    int priority;
    struct CustomerOrder *next;
};

/* =====================================================
   GLOBAL VARIABLES
===================================================== */

struct User users[50];
int userCount = 0;

struct User *currentUser = NULL;

struct Product *productHead = NULL;
struct CustomerOrder *orderHead = NULL;

/* Counters */

int userCounter = 1;
int productCounter = 1001;
int orderCounter = 1;

/* =====================================================
   ADMIN DEFAULT
===================================================== */

void initializeAdmin()
{
    users[0].id = userCounter++;

    strcpy(users[0].username,"admin");
    strcpy(users[0].password,"admin123");

    users[0].role = 3;

    userCount = 1;
}

/* =====================================================
   LOGIN
===================================================== */

int login()
{
    char u[30], p[30];

    printf("Username: ");
    scanf("%s",u);

    printf("Password: ");
    scanf("%s",p);

    for(int i=0;i<userCount;i++)
    {
        if(!strcmp(users[i].username,u) &&
           !strcmp(users[i].password,p))
        {
            currentUser = &users[i];
            return 1;
        }
    }

    printf("Login Failed\n");
    return 0;
}

/* =====================================================
   USER REGISTRATION
===================================================== */

void registerUser()
{
    struct User u;

    u.id = userCounter++;

    printf("Username: ");
    scanf("%s",u.username);

    printf("Password: ");
    scanf("%s",u.password);

    printf("Role 1 Worker 2 Finance 3 Admin: ");
    scanf("%d",&u.role);

    users[userCount++] = u;

    printf("User Registered ID=%d\n",u.id);
}

/* =====================================================
   PRODUCT MANAGEMENT
===================================================== */

void addProduct()
{
    struct Product *p = malloc(sizeof(struct Product));

    p->id = productCounter++;

    printf("Product Name: ");
    scanf("%s",p->name);

    printf("Category: ");
    scanf("%s",p->category);

    printf("Stock: ");
    scanf("%d",&p->stock);

    printf("Price: ");
    scanf("%f",&p->price);

    p->next = productHead;
    productHead = p;

    printf("Product Added ID=%d\n",p->id);
}

/* =====================================================
   VIEW PRODUCTS
===================================================== */

void viewProducts()
{
    struct Product *t = productHead;

    if(!t)
    {
        printf("No Products Available\n");
        return;
    }

    while(t)
    {
        printf("ID:%d Name:%s Stock:%d Price:%.2f\n",
               t->id,t->name,t->stock,t->price);

        t = t->next;
    }
}

/* =====================================================
   FIND PRODUCT
===================================================== */

struct Product *findProduct(int id)
{
    struct Product *t = productHead;

    while(t)
    {
        if(t->id == id)
            return t;

        t = t->next;
    }

    return NULL;
}

/* =====================================================
   CUSTOMER ORDER (WORKER)
===================================================== */

void addCustomerOrder()
{
    struct CustomerOrder *order =
        malloc(sizeof(struct CustomerOrder));

    order->id = orderCounter++;

    printf("Customer Name: ");
    scanf("%s",order->customerName);

    printf("Phone Number: ");
    scanf("%s",order->phone);

    printf("Product Name: ");
    scanf("%s",order->productName);

    struct Product *p =
        findProduct(productCounter-1);

    if(!p)
    {
        printf("Product Not Found\n");
        return;
    }

    printf("Quantity: ");
    scanf("%d",&order->quantity);

    if(p->stock < order->quantity)
    {
        printf("Stock Not Enough\n");
        return;
    }

    printf("Priority (1 VIP 2 Normal): ");
    scanf("%d",&order->priority);

    order->next = orderHead;
    orderHead = order;

    printf("Order Recorded ID=%d\n",order->id);
}

/* =====================================================
   MAIN MENU
===================================================== */

int main()
{
    initializeAdmin();

    int choice;

    while(1)
    {
        printf("\n1.Login 2.Exit\nChoice: ");
        scanf("%d",&choice);

        if(choice==2) exit(0);

        if(!login()) continue;

        if(currentUser->role==3)
        {
            while(1)
            {
                printf("\nADMIN MENU\n");
                printf("1.Register User\n");
                printf("2.Add Product\n");
                printf("3.View Products\n");
                printf("4.Logout\nChoice: ");

                scanf("%d",&choice);

                if(choice==4) break;

                if(choice==1) registerUser();
                if(choice==2) addProduct();
                if(choice==3) viewProducts();
            }
        }

        else if(currentUser->role==1)
        {
            while(1)
            {
                printf("\nWORKER MENU\n");
                printf("1.Add Customer Order\n");
                printf("2.View Products\n");
                printf("3.Logout\nChoice: ");

                scanf("%d",&choice);

                if(choice==3) break;

                if(choice==1) addCustomerOrder();
                if(choice==2) viewProducts();
            }
        }
    }
}
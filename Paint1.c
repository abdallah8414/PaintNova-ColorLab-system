#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User
{
    int id;
    char username[30];
    char password[30];
    int role; // 1 Worker 2 Finance 3 Admin
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
    int status;
    float price;
    struct CustomerOrder *next;
};

struct User users[50];
int userCount = 0;

struct User *currentUser = NULL;

struct Product *productHead = NULL;
struct CustomerOrder *orderHead = NULL;

int userCounter = 1;
int productCounter = 1001;
int orderCounter = 1;

//admin
void initializeAdmin()
{
    users[0].id = userCounter++;
    strcpy(users[0].username, "admin");
    strcpy(users[0].password, "admin123");
    users[0].role = 3;
    userCount = 1;
}

//login
int login()
{
    char u[30], p[30];

    printf("Username: ");
    scanf("%s", u);

    printf("Password: ");
    scanf("%s", p);

    for (int i = 0; i < userCount; i++)
    {
        if (!strcmp(users[i].username, u) &&
            !strcmp(users[i].password, p))
        {
            currentUser = &users[i];
            return 1;
        }
    }

    printf("Login Failed\n");
    return 0;
}

//user
void registerUser()
{
    struct User u;

    u.id = userCounter++;

    printf("Username: ");
    scanf("%s", u.username);

    printf("Password: ");
    scanf("%s", u.password);

    printf("Role (1 Worker 2 Finance 3 Admin): ");
    scanf("%d", &u.role);

    users[userCount++] = u;

    printf("User Registered ID=%d\n", u.id);
}

//product
void addProduct()
{
    struct Product *p = malloc(sizeof(struct Product));

    p->id = productCounter++;

    printf("Product Name: ");
    scanf("%s", p->name);

    printf("Category: ");
    scanf("%s", p->category);

    printf("Stock: ");
    scanf("%d", &p->stock);

    printf("Price: ");
    scanf("%f", &p->price);

    p->next = productHead;
    productHead = p;

    printf("Product Added ID=%d\n", p->id);
}

//find product
struct Product *findProduct(char name[])
{
    struct Product *t = productHead;

    while (t)
    {
        if (!strcmp(t->name, name))
            return t;

        t = t->next;
    }

    return NULL;
}

//view product
void viewProducts()
{
    struct Product *t = productHead;

    if (!t)
    {
        printf("No Products\n");
        return;
    }

    while (t)
    {
        printf("Name:%s Category:%s Stock:%d Price:%.2f\n",
               t->name, t->category, t->stock, t->price);

        t = t->next;
    }
}

//worker
void addCustomerOrder()
{
    struct CustomerOrder *o =
        malloc(sizeof(struct CustomerOrder));

    o->id = orderCounter++;

    printf("Customer Name: ");
    scanf("%s", o->customerName);

    printf("Phone: ");
    scanf("%s", o->phone);

    printf("Product Name: ");
    scanf("%s", o->productName);

    struct Product *p =
        findProduct(o->productName);

    if (!p)
    {
        printf("Product Not Found\n");
        return;
    }

    printf("Quantity: ");
    scanf("%d", &o->quantity);

    if (p->stock < o->quantity)
    {
        printf("Stock Safety Protection\n");
        return;
    }

    printf("Priority (1 VIP 2 Normal): ");
    scanf("%d", &o->priority);

    o->price = p->price;
    o->status = 0;

//insert order if priority is equal to 1(VIP) go to head  and if normal go to tail(FIFO)
    o->next = NULL;

    if (o->priority == 1)
    {
//Inserted at head(VIP)
        o->next = orderHead;
        orderHead = o;
    }
    else
    {
//insert at tail to preserve FIFO(normal)
        if (orderHead == NULL)
        {
            orderHead = o;
        }
        else
        {
            struct CustomerOrder *t = orderHead;
            while (t->next)
                t = t->next;
            t->next = o;
        }
    }

    printf("Order Sent To Finance ID=%d\n", o->id);
}

//worker processes a completed
void workerProcessOrder()
{
    int id;

    printf("Order ID to process: ");
    scanf("%d", &id);

    struct CustomerOrder *t = orderHead;
    struct CustomerOrder *prev = NULL;

    while (t)
    {
        if (t->id == id)
        {
            if (t->status == 2 || t->status == 1)
            {
                struct Product *p = findProduct(t->productName);
                if (!p)
                {
                    printf("Product Not Found in inventory\n");
                    return;
                }

                if (p->stock < t->quantity)
                {
                    printf("Insufficient stock to complete order\n");
                    return;
                }

                p->stock -= t->quantity;
                /* remove order from list */
                if (prev == NULL)
                    orderHead = t->next;
                else
                    prev->next = t->next;

                printf("Order %d processed and stock reduced.\n", t->id);
                free(t);
                return;
            }
            else if (t->status == 0)
            {
                printf("Order is still pending approval/payment\n");
                return;
            }
            else if (t->status == -1)
            {
                printf("Order was rejected and cannot be processed\n");
                return;
            }
            else if (t->status == 3)
            {
                printf("Order already completed\n");
                return;
            }
        }

        prev = t;
        t = t->next;
    }

    printf("Order Not Found\n");
}

//worker to view order
void workerViewOrders()
{
    struct CustomerOrder *t = orderHead;

    if (!t)
    {
        printf("No Orders\n");
        return;
    }

    while (t)
    {
        printf("OrderID:%d Customer:%s Product:%s Qty:%d Status:",
               t->id,
               t->customerName,
               t->productName,
               t->quantity);

        if (t->status == 0)
            printf("Pending");
        else if (t->status == 1)
            printf("Approved");
        else if (t->status == 2)
            printf("Paid");
        else if (t->status == 3)
            printf("Completed");
        else if (t->status == -1)
            printf("Rejected");

        printf("\n");

        t = t->next;
    }
}


//approve order
void approveOrder()
{
    int id;

    printf("Order ID: ");
    scanf("%d", &id);

    struct CustomerOrder *t = orderHead;

    while (t)
    {
        if (t->id == id && t->status == 0)
        {
            t->status = 1;
            printf("Order Approved\n");
            return;
        }

        t = t->next;
    }

    printf("Order Not Found\n");
}


//reject order
void rejectOrder()
{
    int id;

    printf("Order ID: ");
    scanf("%d", &id);

    struct CustomerOrder *t = orderHead;

    while (t)
    {
        if (t->id == id && t->status == 0)
        {
            t->status = -1;
            printf("Order Rejected\n");
            return;
        }

        t = t->next;
    }
}

//view order
void viewOrders()
{
    workerViewOrders();
}


//main function
int main()
{
    printf(".........PAINTNOVA COLORLAB SYSYTEM..........");
    initializeAdmin();

    int choice;

    while (1)
    {
        printf("\n1.Login\n2.Exit\nChoice: ");
        scanf("%d", &choice);

        if (choice == 2)
            exit(0);

        if (!login())
            continue;

//admin
        if (currentUser->role == 3)
        {
            while (1)
            {
                printf("\n..ADMIN MENU..\n");
                printf("1.Register User\n");
                printf("2.Add Product\n");
                printf("3.View Products\n");
                printf("4.Logout\nChoice: ");

                scanf("%d", &choice);

                if (choice == 4)
                    break;

                if (choice == 1)
                    registerUser();
                if (choice == 2)
                    addProduct();
                if (choice == 3)
                    viewProducts();
            }
        }

//worker
        else if (currentUser->role == 1)
        {
            while (1)
            {
                printf("\n..WORKER MENU..\n");
                printf("1.Add Customer Order\n");
                printf("2.View Orders\n");
                printf("3.View Products\n");
                printf("4.Process Order\n");
                printf("5.Logout\nChoice: ");

                scanf("%d", &choice);

                if (choice == 5)
                    break;

                if (choice == 1)
                    addCustomerOrder();
                if (choice == 2)
                    workerViewOrders();
                if (choice == 3)
                    viewProducts();
                if (choice == 4)
                    workerProcessOrder();
            }
        }

//financial controller
        else if (currentUser->role == 2)
        {
            while (1)
            {
                printf("\nFINANCIAL CONTROLLER MENU\n");
                printf("1.View Orders\n");
                printf("2.Approve Order\n");
                printf("3.Reject Order\n");
                printf("5.Logout\nChoice: ");

                scanf("%d", &choice);

                if (choice == 5)
                    break;

                if (choice == 1)
                    viewOrders();
                if (choice == 2)
                    approveOrder();
                if (choice == 3)
                    rejectOrder();

            }
        }
    }
}
